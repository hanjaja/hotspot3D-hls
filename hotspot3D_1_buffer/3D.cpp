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

#define TILE_WIDTH 512   // Full width
#define TILE_HEIGHT 16 + 2  // Height of each tile + halo elements
#define TILE_DEPTH 8     // Full depth

#define BUFFER_SIZE (TILE_WIDTH * TILE_HEIGHT * TILE_DEPTH)

#define IS_BOUNDARY 1
#define NOT_BOUNDARY 0

// load in with halo
void load_in_with_halo(float *in, float local_in[BUFFER_SIZE], int tileOffset, bool isBoundary) {
    int globalIdx, localIdx;
    for (int z = 0; z < TILE_DEPTH; z++) {
        for (int y = -1; y <= TILE_HEIGHT; y++) { // Halo cells included
            for (int x = 0; x < TILE_WIDTH; x++) {
                localIdx = (y + 1) * TILE_WIDTH + z * TILE_WIDTH * TILE_HEIGHT + x;

                // Calculate global index, adjust for halo cells
                if (y >= 0 && y < TILE_HEIGHT) {
                    globalIdx = tileOffset + (y * NX + z * NX * NY) + x;
                } else {
                    // For halo cells, mirror the edge cells
                    int haloY = 0;
                    if (isBoundary) {
                        haloY = (y == -1 && tileOffset == 0) ? 0 : (y == TILE_HEIGHT && tileOffset + TILE_HEIGHT >= NY) ? TILE_HEIGHT - 1 : y;
                    } else {
                        haloY = (y == -1) ? y + 1 : (y == TILE_HEIGHT) ? y - 1 : y;
                    }
                    globalIdx = tileOffset + (haloY * NX + z * NX * NY) + x;
                }

                local_in[localIdx] = in[globalIdx];
            }
        }
    }
}


// Combined load function with halo handling
void load(float *pIn, float *tIn, float local_pIn[BUFFER_SIZE], float local_tIn[BUFFER_SIZE], int tileOffset, bool isBoundary) {
    #pragma HLS DATAFLOW
    load_in_with_halo(pIn, local_pIn, tileOffset, isBoundary);
    load_in_with_halo(tIn, local_tIn, tileOffset, isBoundary);
}

// Store function
void store(float* output, float local_buffer[BUFFER_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        //#pragma HLS PIPELINE
        output[i] = local_buffer[i];
    }
}

void compute(float local_pIn[BUFFER_SIZE], 
             float local_tIn[BUFFER_SIZE], 
             float local_tOut[BUFFER_SIZE],
             float Cap, float Rx, float Ry, float Rz, float dt, 
             int numiter) {
    
    float ce, cw, cn, cs, ct, cb, cc;
    float stepDivCap = dt / Cap;
    ce = cw = stepDivCap / Rx;
    cn = cs = stepDivCap / Ry;
    ct = cb = stepDivCap / Rz;
    cc = 1.0 - (2.0 * ce + 2.0 * cn + 3.0 * ct);

    for (int z = 0; z < TILE_DEPTH; z++) {
        for (int y = 1; y < TILE_HEIGHT - 1; y++) {  // Exclude halo cells
            for (int x = 0; x < TILE_WIDTH; x++) {
                //#pragma HLS PIPELINE
                int c = x + y * TILE_WIDTH + z * TILE_WIDTH * TILE_HEIGHT;
                int w = c - 1;
                int e = c + 1;
                int n = c - TILE_WIDTH;
                int s = c + TILE_WIDTH;
                int b = c - TILE_WIDTH * TILE_HEIGHT;
                int t = c + TILE_WIDTH * TILE_HEIGHT;

                local_tOut[c] = local_tIn[c] * cc + local_tIn[n] * cn + local_tIn[s] * cs +
                                local_tIn[e] * ce + local_tIn[w] * cw + local_tIn[t] * ct +
                                local_tIn[b] * cb + (dt / Cap) * local_pIn[c];
            }
        }
    }
}

void hotspot(float *pIn, float* tIn, float *tOut, float Cap, float Rx, float Ry, float Rz, float dt, int numiter) {
    float local_pIn[BUFFER_SIZE];
    float local_tIn[BUFFER_SIZE];
    float local_tOut[BUFFER_SIZE];

    for (int iter = 0; iter < numiter; iter++) {
        for (int yTile = 0; yTile < NY; yTile += TILE_HEIGHT) {
            bool boundaryFlag = NOT_BOUNDARY;
            if (yTile == 0 || yTile + TILE_HEIGHT >= NY) {
                boundaryFlag = IS_BOUNDARY;
            }

            int tileOffset = yTile * NX * NZ;
            int tileSize = TILE_WIDTH * TILE_HEIGHT * TILE_DEPTH;

            // Load data for each tile with boundary handling
            load(&pIn[tileOffset], &tIn[tileOffset], local_pIn, local_tIn, tileOffset, boundaryFlag);

            // Compute temperatures for each tile
            compute(local_pIn, local_tIn, local_tOut, TILE_WIDTH, TILE_HEIGHT, TILE_DEPTH, Cap, Rx, Ry, Rz, dt, numiter);

            // Store only the computational region of each tile
            store(&tOut[tileOffset], local_tOut, tileSize);
        }

        // Swap input and output buffers for the next iteration
        float* temp = tIn;
        tIn = tOut;
        tOut = temp;
    }
}