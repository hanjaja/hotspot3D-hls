#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h> 
#include <math.h> 
#include <sys/time.h>
#include <string.h>

#define NX 512  // Fixed x-dimension
#define NY 512  // Fixed y-dimension
#define NZ 8    // Fixed z-dimension

#define TILE_X 512   // Full width
#define TILE_Y 16
#define TILE_Z 8     // Full depth

#define BUFFER_SIZE (TILE_X * (TILE_Y + 2) * TILE_Z)
#define POWER_BUFFER_SIZE (TILE_X * TILE_Y * TILE_Z)

#define NOT_BOUNDARY 0
#define FIRST_TILE 1
#define LAST_TILE 2

void load_power(float *pIn, float local_pIn[POWER_BUFFER_SIZE], int tileOffset) {
    for (int z = 0; z < TILE_Z; z++) {
        for (int y = 0; y < TILE_Y; y++) {
            for (int x = 0; x < TILE_X; x++) {
                int localIdx = y * TILE_X + z * TILE_X * TILE_Y + x;
                int globalIdx = tileOffset + (y * NX + z * NX * NY) + x;
                local_pIn[localIdx] = pIn[globalIdx];
            }
        }
    }
}

// load in with halo
void load_in_with_halo(float *in, float local_in[BUFFER_SIZE], int tileOffset, bool isBoundary) {
    int globalIdx, localIdx;
    for (int z = 0; z < TILE_Z; z++) {
        for (int y = -1; y <= TILE_Y; y++) { // Halo cells included
            for (int x = 0; x < TILE_X; x++) {
                localIdx = (y + 1) * TILE_X + z * TILE_X * TILE_Y + x;

                int haloY;

                if (isBoundary == FIRST_TILE) {
                    haloY = (y == -1) ? 0 : y;
                } else if (isBoundary == LAST_TILE) {
                    haloY = (y == TILE_Y) ? TILE_Y - 1 : y;
                } else {
                    haloY = y;
                }

                globalIdx = tileOffset + (haloY * NX + z * NX * NY) + x;

                local_in[localIdx] = in[globalIdx];
            }
        }
    }
}

// Combined load function with halo handling
void load(float *pIn, float *tIn, float local_pIn[POWER_BUFFER_SIZE], float local_tIn[BUFFER_SIZE], int tileOffset, bool isBoundary) {
    #pragma HLS DATAFLOW
    load_power(pIn, local_pIn, tileOffset);
    load_in_with_halo(tIn, local_tIn, tileOffset, isBoundary);
}

// Store function
void store(float* output, float local_buffer[BUFFER_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        //#pragma HLS PIPELINE
        output[i] = local_buffer[i];
    }
}

void compute(float local_pIn[POWER_BUFFER_SIZE], 
             float local_tIn[BUFFER_SIZE], 
             float local_tOut[BUFFER_SIZE],
             float Cap, float Rx, float Ry, float Rz, float dt) {
    
    float ce, cw, cn, cs, ct, cb, cc;
    float stepDivCap = dt / Cap;
    ce = cw = stepDivCap / Rx;
    cn = cs = stepDivCap / Ry;
    ct = cb = stepDivCap / Rz;
    cc = 1.0 - (2.0 * ce + 2.0 * cn + 3.0 * ct);

    for (int z = 0; z < TILE_Z; z++) {
        for (int y = 1; y < TILE_Y + 1; y++) {  // Exclude halo cells
            for (int x = 0; x < TILE_X; x++) {
                //#pragma HLS PIPELINE
                int c = x + y * TILE_X + z * TILE_X * (TILE_Y + 2);
                int w = c - 1;
                int e = c + 1;
                int n = c - TILE_X;
                int s = c + TILE_X;
                int b = c - TILE_X * (TILE_Y + 2);
                int t = c + TILE_X * (TILE_Y + 2);

                local_tOut[c] = local_tIn[c] * cc + local_tIn[n] * cn + local_tIn[s] * cs +
                                local_tIn[e] * ce + local_tIn[w] * cw + local_tIn[t] * ct +
                                local_tIn[b] * cb + (dt / Cap) * local_pIn[c];
            }
        }
    }
}

void hotspot(float *pIn, float* tIn, float *tOut, float Cap, float Rx, float Ry, float Rz, float dt, int numiter) {
    float local_pIn[POWER_BUFFER_SIZE];
    float local_tIn[BUFFER_SIZE];
    float local_tOut[BUFFER_SIZE];

    for (int iter = 0; iter < numiter; iter++) {
        for (int yTile = 0; yTile < NY; yTile += TILE_Y) {
            int boundaryFlag = NOT_BOUNDARY;
            if (yTile == 0 ) {
                boundaryFlag = FIRST_TILE;
            } else if (yTile == NY - TILE_Y) {
                boundaryFlag = LAST_TILE;
            }

            int tileOffset = yTile * NX * NZ;
            int tileSize = TILE_X * TILE_Y * TILE_Z;

            // Load data for each tile with boundary handling
            load(&pIn[tileOffset], &tIn[tileOffset], local_pIn, local_tIn, tileOffset, boundaryFlag);

            // Compute temperatures for each tile
            compute(local_pIn, local_tIn, local_tOut, TILE_X, TILE_Y, TILE_Z, Cap, Rx, Ry, Rz, dt);

            // Store only the computational region of each tile
            store(&tOut[tileOffset], local_tOut, tileSize);
        }

        // Swap input and output buffers for the next iteration
        float* temp = tIn;
        tIn = tOut;
        tOut = temp;
    }
}