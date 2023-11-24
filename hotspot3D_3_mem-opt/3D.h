#ifndef _H_3D_H_
#define _H_3D_H_

#include <ap_int.h>
#include "mars_wide_bus.h"

#define DWIDTH 512
#define INTERFACE_WIDTH ap_uint<DWIDTH>
const int WIDTH_FACTOR = DWIDTH/32;

#define NX 1024  // Fixed x-dimension
#define NY 1024  // Fixed y-dimension
#define NZ 8    // Fixed z-dimension

#define NUMITER 100

void hotspot(INTERFACE_WIDTH pIn[NX*NY*NZ/WIDTH_FACTOR], INTERFACE_WIDTH tIn[NX*NY*NZ/WIDTH_FACTOR], INTERFACE_WIDTH tOut[NX*NY*NZ/WIDTH_FACTOR], 
             float stepDivCap, float ce, float cw, float cn, float cs, float ct, float cb, float cc);

#endif // _H_3D_H_
