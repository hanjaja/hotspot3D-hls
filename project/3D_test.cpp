#include <omp.h>
#include <cmath>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include "my_timer.h"

#include "mm.h"

#define NI 32
#define NJ 32
#define NK 32
#define buff_size 16

/* Array initialization. */
static
void init_array(float C[NI*NJ], float A[NI*NK], float B[NK*NJ])
{
  int i, j;

  for (i = 0; i < NI; i++)
    for (j = 0; j < NJ; j++)
      C[i*NJ+j] = (float)((i*j+1) % NI) / NI;
  for (i = 0; i < NI; i++)
    for (j = 0; j < NK; j++)
      A[i*NK+j] = (float)(i*(j+1) % NK) / NK;
  for (i = 0; i < NK; i++)
    for (j = 0; j < NJ; j++)
      B[i*NJ+j] = (float)(i*(j+2) % NJ) / NJ;
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(float C[NI*NJ])
{
  int i, j;

  for (i = 0; i < NI; i++)
    for (j = 0; j < NJ; j++)
      printf("C[%d][%d] = %f\n", i, j, C[i*NJ+j]);
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array_sum(float C[NI*NJ])
{
  int i, j;

  float sum = 0.0;
  
  for (i = 0; i < NI; i++)
    for (j = 0; j < NJ; j++)
      sum += C[i*NJ+j];

  printf("sum of C array = %f\n", sum);
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_gemm(float C[NI*NJ], float A[NI*NK], float B[NK*NJ], float alpha, float beta)
{
  int i, j, k; // used for the three outer loops
  int ii, jj, kk; // used for the three inner loops
  int N, b; // b is the block size
  b = 32;
  N = ceil(NI*1.0/b); // we can use NI=NJ=NK

  printf("Block size = %d elements\n", b);

// => Form C := alpha*A*B + beta*C,
//A is NIxNK
//B is NKxNJ
//C is NIxNJ


  #pragma omp parallel for private(i, j, ii, jj) collapse(2) num_threads(10)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      for (ii = 0; ii < std::min(b,NI-i*b); ii++){ // we add the min() incase the last block has less than bxb elements
        #pragma omp simd
        for (jj = 0; jj < std::min(b,NJ-j*b); jj++){
          C[(i*b+ii)*NI+(j*b+jj)] *= beta; // Since it is a 1D array we have to do some calculations to ensure proper indexing 
        }
      }
    }
  }

  #pragma omp parallel for private(i, j, k, ii, jj, kk) num_threads(10)
  for (i = 0; i < N; i++) {
    for (k = 0; k < N; k++) {
      for (j = 0; j < N; j++) {
        for (ii = 0; ii < std::min(b,NI-i*b); ii++){
          for (kk = 0; kk < std::min(b,NK-k*b); kk++){
            #pragma omp simd
            for (jj = 0; jj < std::min(b,NJ-j*b); jj++){
              C[(i*b+ii)*NI+(j*b+jj)] += alpha * A[(i*b+ii)*NI+(k*b+kk)] * B[(k*b+kk)*NK+(j*b+jj)];
            }
          }
        }
      }
    }
  }
}

int main(int argc, char** argv)
{
  /* Variable declaration/allocation. */
  float *A_sw = (float *)malloc(NI*NK*sizeof(float));
  float *B_sw = (float *)malloc(NK*NJ*sizeof(float));
  float *C_sw = (float *)malloc(NI*NJ*sizeof(float));


  float *A_hw = (float *)malloc(NI*NK*sizeof(float));
  float *B_hw = (float *)malloc(NK*NJ*sizeof(float));
  float *C_hw = (float *)malloc(NI*NJ*sizeof(float));

  /* Initialize array(s). */
  init_array (C_sw, A_sw, B_sw);
  init_array (C_hw, A_hw, B_hw);


  /* Start timer. */
  timespec timer = tic();

  /* Run kernel. */
  kernel_gemm (C_sw, A_sw, B_sw, 1.5, 2.5);

  /* Stop and print timer. */
  toc(&timer, "kernel execution");

    /* Start timer. */
  timer = tic();

  /* Run kernel. */
  mm (C_hw, A_hw, B_hw, 1.5, 2.5);

  /* Stop and print timer. */
  toc(&timer, "kernel execution");
  
  /* Print results. */
  print_array_sum (C_sw);
  print_array_sum (C_hw);

  //Check results
  for (int i = 0; i < NI; i++) {
    for(int j = 0; j < NI; j++)
      if(C_sw[i*NI + j] != C_hw[i*NI + j]) {
        printf("TEST FAILED, results not matching, output_sw[%d][%d] = %f, output_hw[%d][%d] = %f.\n",
	      i,j, C_sw[i*NI + j], i,j, C_hw[i*NI + j]);
        return -1;
    }
  }


  /* free memory for A, B, C */
  free(A_sw);
  free(B_sw);
  free(C_sw);

  free(A_hw);
  free(B_hw);
  free(C_hw);
  
  printf("TEST PASSED!\n");
  return 0;
}
