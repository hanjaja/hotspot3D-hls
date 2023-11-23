#ifndef _H_3D_H_
#define _H_3D_H_

#define DWIDTH 512
#define INTERFACE_WIDTH ap_uint<DWIDTH>
const int WIDTH_FACTOR = DWIDTH/32;

#define NX 1024  // Fixed x-dimension
#define NY 1024  // Fixed y-dimension
#define NZ 8    // Fixed z-dimension

#define NUMITER 100

void hotspot(float *pIn, float* tIn, float *tOut, float Cap, 
        float Rx, float Ry, float Rz, 
        float dt);

#endif // _H_3D_H_
