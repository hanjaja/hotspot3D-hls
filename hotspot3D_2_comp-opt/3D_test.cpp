#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h> 
#include <math.h> 
#include <sys/time.h>
#include <string.h>

#include "3D.h"

#define STR_SIZE (256)
#define MAX_PD	(3.0e6)
/* required precision in degrees	*/
#define PRECISION	0.001
#define SPEC_HEAT_SI 1.75e6
#define K_SI 100
/* capacitance fitting factor	*/
#define FACTOR_CHIP	0.5

#define HPERERA "/localhome/hperera/hotspot3D-hls/data/power_1024x8"
#define HPERERA2 "/localhome/hperera/hotspot3D-hls/data/temp_1024x8"
#define HPERERA3 "/localhome/hperera/hotspot3D-hls/data/output.out"
#define HPERERA4 "/localhome/hperera/hotspot3D-hls/data/output_cpu.out"

#define DRASHID "/localhome/drashid/ENSC453/hotspot3D-hls/data/power_1024x8"
#define DRASHID2 "/localhome/drashid/ENSC453/hotspot3D-hls/data/temp_1024x8"
#define DRASHID3 "/localhome/drashid/ENSC453/hotspot3D-hls/data/output.out"
#define DRASHID4 "/localhome/drashid/ENSC453/hotspot3D-hls/data/output_cpu.out"

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

    for (k=0; k < layers; k++)
       for (i=0; i < grid_rows; i++)       
          for (j=0; j < grid_cols; j++)
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

void computeTempFPGA(float *pIn, float* tIn, float *tOut, float stepDivCap, float ce, float cw, float cn, float cs, float ct, float cb, float cc) {
    hotspot(pIn, tIn, tOut, stepDivCap, ce, cw, cn, cs, ct, cb, cc);
}

void usage(int argc, char **argv)
{
    fprintf(stderr, "Usage: %s <rows/cols> <layers> <iterations> <powerFile> <tempFile> <outputFile>\n", argv[0]);
    fprintf(stderr, "\t<rows/cols>  - number of rows/cols in the grid (positive integer)\n");
    fprintf(stderr, "\t<layers>  - number of layers in the grid (positive integer)\n");

    fprintf(stderr, "\t<iteration> - number of iterations\n");
    fprintf(stderr, "\t<powerFile>  - name of the file containing the initial power values of each cell\n");
    fprintf(stderr, "\t<tempFile>  - name of the file containing the initial temperature values of each cell\n");
    fprintf(stderr, "\t<outputFile - output file\n");
    exit(1);
}

int main(int argc, char** argv)
{
    /*if (argc != 7)
    {
        usage(argc,argv);
    }*/

    //char *pfile, *tfile, *ofile;// *testFile;
    int iterations = NUMITER;

    char pfile[] = DRASHID;
    char tfile[] = DRASHID2;
    char ofile[] = DRASHID3;
    char ofile_cpu[] = DRASHID4;
    //testFile = argv[7];
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

    // cout << Rx << " " << Ry << " " << Rz << endl;
    float max_slope = MAX_PD / (FACTOR_CHIP * t_chip * SPEC_HEAT_SI);
    float dt = PRECISION / max_slope;

    float ce, cw, cn, cs, ct, cb, cc;
    float stepDivCap = dt / Cap;
    ce = cw = stepDivCap / Rx;
    cn = cs = stepDivCap / Ry;
    ct = cb = stepDivCap / Rz;
    cc = 1.0 - (2.0 * ce + 2.0 * cn + 3.0 * ct);

    float *powerIn, *tempOut, *tempIn, *tempCopy;// *pCopy;
    //    float *d_powerIn, *d_tempIn, *d_tempOut;
    int size = numCols * numRows * layers;

    powerIn = (float*)calloc(size, sizeof(float));
    tempCopy = (float*)malloc(size * sizeof(float));
    tempIn = (float*)calloc(size,sizeof(float));
    tempOut = (float*)calloc(size, sizeof(float));
    //pCopy = (float*)calloc(size,sizeof(float));
    float* answer = (float*)calloc(size, sizeof(float));

    // outCopy = (float*)calloc(size, sizeof(float));
    readinput(powerIn,numRows, numCols, layers,pfile);
    readinput(tempIn, numRows, numCols, layers, tfile);

    memcpy(tempCopy,tempIn, size * sizeof(float));

    struct timeval start, stop;
    float time, CPU_time;

    // FPGA execution
    gettimeofday(&start,NULL);
    computeTempFPGA(powerIn, tempIn, tempOut, stepDivCap, ce, cw, cn, cs, ct, cb, cc);
    gettimeofday(&stop,NULL);
    time = (stop.tv_usec - start.tv_usec) * 1.0e-6 + stop.tv_sec - start.tv_sec;

    // CPU execution
    gettimeofday(&start,NULL);
    computeTempCPU(powerIn, tempCopy, answer, numCols, numRows, layers, Cap, Rx, Ry, Rz, dt, iterations);
    gettimeofday(&stop,NULL);
    CPU_time = (stop.tv_usec - start.tv_usec) * 1.0e-6 + stop.tv_sec - start.tv_sec;

    // Accuracy and timing comparison
    float acc = accuracy(tempOut, answer, numRows * numCols * layers);
    printf("FPGA Time: %.3f (s)\n", time);
    printf("CPU Time: %.3f (s)\n", CPU_time);
    printf("Accuracy: %e\n", acc);

    // Write output and cleanup
    writeoutput(tempOut, numRows, numCols, layers, ofile);
    writeoutput(answer, numRows, numCols, layers, ofile_cpu);
    free(tempIn); free(tempOut); free(powerIn); free(answer); free(tempCopy);

    return 0;
}	
