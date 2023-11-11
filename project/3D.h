#ifndef _H_3D_H_
#define _H_3D_H_


void computeTempFPGA(float *pIn, float* tIn, float *tOut, 
        int nx, int ny, int nz, float Cap, 
        float Rx, float Ry, float Rz, 
        float dt, int numiter);

#endif // _H_3D_H_
