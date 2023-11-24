#ifndef _H_3D_H_
#define _H_3D_H_

#define NX 1024  // Fixed x-dimension
#define NY 1024  // Fixed y-dimension
#define NZ 8    // Fixed z-dimension

#define NUMITER 100

void hotspot(float *pIn, float* tIn, float *tOut, float stepDivCap,
             float ce, float cw, float cn, float cs, float ct, float cb, float cc);

#endif // _H_3D_H_
