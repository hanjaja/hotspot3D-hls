#ifndef _H_3D_H_
#define _H_3D_H_

#include <ap_int.h>
#include "mars_wide_bus.h"

#define DWIDTH 512
#define INTERFACE_WIDTH ap_uint<DWIDTH>
#define WIDTH_FACTOR (DWIDTH/32)

#define NX 32  // Fixed x-dimension
#define NY 32  // Fixed y-dimension
#define NZ 4    // Fixed z-dimension

#define NUMITER 2

extern "C"{
void hotspot(INTERFACE_WIDTH pIn[NX*NY*NZ/WIDTH_FACTOR], INTERFACE_WIDTH tIn[NX*NY*NZ/WIDTH_FACTOR], INTERFACE_WIDTH tOut[NX*NY*NZ/WIDTH_FACTOR], 
             float stepDivCap, float ce, float cw, float cn, float cs, float ct, float cb, float cc);

#endif // _H_3D_H_
}
