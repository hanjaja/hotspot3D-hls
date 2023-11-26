/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

/*******************************************************************************

Description:

    Vitis matrix multiply example which showcases how the host code works.

*******************************************************************************/
// OpenCL utility layer include
#include "xcl2.hpp"
#include <vector>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h> 
#include <math.h> 
#include <sys/time.h>
#include <string.h>

#include "hotspot3D.h"
#include "ap_int.h"

#define STR_SIZE (256)
#define MAX_PD	(3.0e6)
/* required precision in degrees	*/
#define PRECISION	0.001
#define SPEC_HEAT_SI 1.75e6
#define K_SI 100
/* capacitance fitting factor	*/
#define FACTOR_CHIP	0.5

/* chip parameters	*/
float t_chip = 0.0005;
float chip_height = 0.016; float chip_width = 0.016; 
/* ambient temperature, assuming no package at all	*/
float amb_temp = 80.0;

void fatal(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
}

void readinput(float *vect, int grid_rows, int grid_cols, int layers, char *file) {
    int i,j,k;
    FILE *fp;
    char str[STR_SIZE];
    float val;
    char err_msg_1[] = "The file was not opened";
    char err_msg_2[] = "Error reading file\n";
    char err_msg_3[] = "not enough lines in file";
    char err_msg_4[] = "invalid file format";

    if( (fp  = fopen(file, "r" )) ==0 )
      fatal(err_msg_1 );


    for (i=0; i <= grid_rows-1; i++) 
      for (j=0; j <= grid_cols-1; j++)
        for (k=0; k <= layers-1; k++)
          {
            if (fgets(str, STR_SIZE, fp) == NULL) fatal(err_msg_2);
            if (feof(fp))
              fatal(err_msg_3);
            if ((sscanf(str, "%f", &val) != 1))
              fatal(err_msg_4);
            vect[i*grid_cols+j+k*grid_rows*grid_cols] = val;
          }

    fclose(fp);	

}

void writeoutput(float *vect, int grid_rows, int grid_cols, int layers, char *file) {

    int i,j,k, index=0;
    FILE *fp;
    char str[STR_SIZE];

    if( (fp = fopen(file, "w" )) == 0 )
      printf( "The file was not opened\n" );

    for (i=0; i < grid_rows; i++) 
      for (j=0; j < grid_cols; j++)
        for (k=0; k < layers; k++)
          {
            sprintf(str, "%d\t%g\n", index, vect[i*grid_cols+j+k*grid_rows*grid_cols]);
            fputs(str,fp);
            index++;
          }

    fclose(fp);	
}

void computeTempCPU(float *pIn, float* tIn, float *tOut, 
        int nx, int ny, int nz, float Cap, 
        float Rx, float Ry, float Rz, 
        float dt, int numiter) 
{   float ce, cw, cn, cs, ct, cb, cc;
    float stepDivCap = dt / Cap;
    ce = cw =stepDivCap/ Rx;
    cn = cs =stepDivCap/ Ry;
    ct = cb =stepDivCap/ Rz;

    cc = 1.0 - (2.0*ce + 2.0*cn + 3.0*ct);

    int c,w,e,n,s,b,t;
    int x,y,z;
    int i = 0;
    do{
        for(z = 0; z < nz; z++)
            for(y = 0; y < ny; y++)
                for(x = 0; x < nx; x++)
                {
                    c = x + y * nx + z * nx * ny;

                    w = (x == 0) ? c      : c - 1;
                    e = (x == nx - 1) ? c : c + 1;
                    n = (y == 0) ? c      : c - nx;
                    s = (y == ny - 1) ? c : c + nx;
                    b = (z == 0) ? c      : c - nx * ny;
                    t = (z == nz - 1) ? c : c + nx * ny;


                    tOut[c] = tIn[c]*cc + tIn[n]*cn + tIn[s]*cs + tIn[e]*ce + tIn[w]*cw + tIn[t]*ct + tIn[b]*cb + (dt/Cap) * pIn[c] + ct*amb_temp;
                }
        float *temp = tIn;
        tIn = tOut;
        tOut = temp; 
        i++;
    }
    while(i < numiter);

}

float accuracy(float *arr1, float *arr2, int len)
{
    float err = 0.0; 
    int i;
    for(i = 0; i < len; i++)
    {
        err += (arr1[i]-arr2[i]) * (arr1[i]-arr2[i]);
    }

    return (float)sqrt(err/len);


}



int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string binaryFile = argv[1];


    size_t matrix_size_bytes = sizeof(float) * NX * NY * NZ;
    cl_int err;
    cl::CommandQueue q;
    cl::Context context;
    cl::Kernel krnl_hotspot3D;

    int iterations = NUMITER;

    char pfile[] = "../../data/power_1024x8";
    char tfile[] = "../../data/temp_1024x8";
    char ofile[] = "../../data/output.out";
    char ofile_cpu[] = "../../data/output_cpu.out";

    int numCols = NY;
    int numRows = NX;
    int layers = NZ;

    /* calculating parameters*/

    float dx = chip_height/numRows;
    float dy = chip_width/numCols;
    float dz = t_chip/layers;

    float Cap = FACTOR_CHIP * SPEC_HEAT_SI * t_chip * dx * dy;
    float Rx = dy / (2.0 * K_SI * t_chip * dx);
    float Ry = dx / (2.0 * K_SI * t_chip * dy);
    float Rz = dz / (K_SI * dx * dy);

    float max_slope = MAX_PD / (FACTOR_CHIP * t_chip * SPEC_HEAT_SI);
    float dt = PRECISION / max_slope;

    float ce, cw, cn, cs, ct, cb, cc;
    float stepDivCap = dt / Cap;
    ce = cw = stepDivCap / Rx;
    cn = cs = stepDivCap / Ry;
    ct = cb = stepDivCap / Rz;
    cc = 1.0 - (2.0 * ce + 2.0 * cn + 3.0 * ct);

    int size = numCols * numRows * layers;



    printf("Allocating Memory..\n");
    /* Variable declaration/allocation. */

    std::vector<float, aligned_allocator<float> > powerIn_f(size);
    std::vector<float, aligned_allocator<float> > tempOut_f(size);
    std::vector<float, aligned_allocator<float> > tempIn_f(size);
    std::vector<float, aligned_allocator<float> > tempCopy_f(size);
    std::vector<float, aligned_allocator<float> > answer_f(size);


        /* Initialize array(s). */
    readinput(powerIn_f.data(),numRows, numCols, layers, pfile);
    readinput(tempIn_f.data(), numRows, numCols, layers, tfile);
    readinput(tempCopy_f.data(), numRows, numCols, layers, tfile);
    //memcpy(tempCopy_f.data(),tempIn_f.data(), size * sizeof(float));
    printf("Initialized Arrays..\n");

    //memcpy_wide_bus_write_float((class ap_uint<LARGE_BUS> *)powerIn, powerIn_f.data(), 0, sizeof(float)*size);
    //memcpy_wide_bus_write_float((class ap_uint<LARGE_BUS> *)tempIn, tempIn_f.data(), 0, sizeof(float)*size);
    //memcpy_wide_bus_write_float((class ap_uint<LARGE_BUS> *)tempOut, tempOut_f.data(), 0, sizeof(float)*size);    

    // OPENCL HOST CODE AREA START
    auto devices = xcl::get_xil_devices();
    // read_binary_file() is a utility API which will load the binaryFile
    // and will return the pointer to file buffer.
    auto fileBuf = xcl::read_binary_file(binaryFile);
    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    bool valid_device = false;
    for (unsigned int i = 0; i < devices.size(); i++) {
        auto device = devices[i];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
        OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &err));

        std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << i << "]: program successful!\n";
            OCL_CHECK(err, krnl_hotspot3D = cl::Kernel(program, "hotspot3D", &err));
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    // Allocate Buffer in Global Memory
    OCL_CHECK(err, cl::Buffer buffer_tempOut(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes,
                                         ( class ap_uint<512> *)tempOut_f.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_powerIn(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, matrix_size_bytes,
                                         ( class ap_uint<512> *)powerIn_f.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_tempIn(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, matrix_size_bytes,
                                         ( class ap_uint<512> *)tempIn_f.data(), &err));


    OCL_CHECK(err, err = krnl_hotspot3D.setArg(0, buffer_powerIn));
    OCL_CHECK(err, err = krnl_hotspot3D.setArg(1, buffer_tempIn));
    OCL_CHECK(err, err = krnl_hotspot3D.setArg(2, buffer_tempOut));
    OCL_CHECK(err, err = krnl_hotspot3D.setArg(3, stepDivCap));
    OCL_CHECK(err, err = krnl_hotspot3D.setArg(4, ce));
    OCL_CHECK(err, err = krnl_hotspot3D.setArg(5, cw));
    OCL_CHECK(err, err = krnl_hotspot3D.setArg(6, cn));
    OCL_CHECK(err, err = krnl_hotspot3D.setArg(7, cs));
    OCL_CHECK(err, err = krnl_hotspot3D.setArg(8, ct));
    OCL_CHECK(err, err = krnl_hotspot3D.setArg(9, cb));
    OCL_CHECK(err, err = krnl_hotspot3D.setArg(10, cc));

    // Copy input data to device global memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_powerIn, buffer_tempIn, buffer_tempOut}, 0 /* 0 means from host*/));
    q.finish();

    struct timeval start, stop;
    float time, CPU_time;

    printf("Running FPGA Kernal..\n");
    gettimeofday(&start,NULL);

    // Launch the Kernel
    OCL_CHECK(err, err = q.enqueueTask(krnl_hotspot3D));
    q.finish();

    gettimeofday(&stop,NULL);
    time = (stop.tv_usec - start.tv_usec) * 1.0e-6 + stop.tv_sec - start.tv_sec;

    // Copy Result from Device Global Memory to Host Local Memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_tempOut}, CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();

    // OPENCL HOST CODE AREA END

    // Get output results from software run
    printf("Running CPU Kernal..\n");
    gettimeofday(&start,NULL);
    computeTempCPU(powerIn_f.data(), tempCopy_f.data(), answer_f.data(), numCols, numRows, layers, Cap, Rx, Ry, Rz, dt, iterations);
    gettimeofday(&stop,NULL);
    CPU_time = (stop.tv_usec - start.tv_usec) * 1.0e-6 + stop.tv_sec - start.tv_sec;

    //memcpy_wide_bus_read_float(tempOut_f.data(), (class ap_uint<LARGE_BUS> *)tempOut, 0, sizeof(float)*size);

    // Compare the results of the Device to the simulation
    float acc = accuracy(tempOut_f.data(), answer_f.data(), numRows * numCols * layers);
    printf("FPGA Time: %.3f (s)\n", time);
    printf("CPU Time: %.3f (s)\n", CPU_time);
    printf("Accuracy: %e\n", acc);
   
    writeoutput(tempOut_f.data(), numRows, numCols, layers, ofile);
    writeoutput(answer_f.data(), numRows, numCols, layers, ofile_cpu);
    //free(tempIn_f); free(tempOut_f); free(powerIn_f); free(answer_f); free(tempCopy_f);

    return (0);
}
