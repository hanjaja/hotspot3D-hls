#ifndef _H_MM_H_
#define _H_MM_H_

#define NI 32
#define NJ 32
#define NK 32
#define buff_size 16

void mm(float C[NI*NJ], float A[NI*NK], float B[NK*NJ], float alpha, float beta);

#endif // _H_MM_H_
