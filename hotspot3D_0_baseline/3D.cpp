#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h> 
#include <math.h> 
#include <sys/time.h>
#include <string.h>

#include "3D.h"


void computeTempFPGA(float *pIn, float* tIn, float *tOut, 
        int nx, int ny, int nz, float Cap, 
        float Rx, float Ry, float Rz, 
        float dt, int numiter) 
{   float ce, cw, cn, cs, ct, cb, cc;
    float stepDivCap = dt / Cap;
    float amb_temp = 80.0;
    ce = cw =stepDivCap/ Rx;
    cn = cs =stepDivCap/ Ry;
    ct = cb =stepDivCap/ Rz;

    cc = 1.0 - (2.0*ce + 2.0*cn + 3.0*ct);

    int c,w,e,n,s,b,t;
    int x,y,z;
    int i = 0;
    do{
        #pragma HLS loop_tripcount min=1 max=100
        for(z = 0; z < nz; z++)
        #pragma HLS loop_tripcount min=1 max=8
            for(y = 0; y < ny; y++)
            #pragma HLS loop_tripcount min=1 max=1024
                for(x = 0; x < nx; x++)
                #pragma HLS loop_tripcount min=1 max=1024
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








