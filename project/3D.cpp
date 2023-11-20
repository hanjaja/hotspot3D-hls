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
#define TILE_HEIGHT 64   // Height of each tile, as an example
#define TILE_DEPTH 8     // Full depth

#define BUFFER_SIZE (TILE_WIDTH * TILE_HEIGHT * TILE_DEPTH)

// load in
void load_in(float *in, float local_in[BUFFER_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        //#pragma HLS PIPELINE
        local_in[i] = in[i];
    }
}

// Combined load function
void load(float *pIn, float *tIn, float local_pIn[BUFFER_SIZE], float local_tIn[BUFFER_SIZE], int size) {
    #pragma HLS DATAFLOW
    load_in(pIn, local_pIn, size);
    load_in(tIn, local_tIn, size);
}

// Store function
void store(float* output, float local_buffer[BUFFER_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        //#pragma HLS PIPELINE
        output[i] = local_buffer[i];
    }
}

void compute(float local_pIn[TILE_WIDTH * TILE_HEIGHT * TILE_DEPTH], 
             float local_tIn[TILE_WIDTH * TILE_HEIGHT * TILE_DEPTH], 
             float local_tOut[TILE_WIDTH * TILE_HEIGHT * TILE_DEPTH],
             float Cap, float Rx, float Ry, float Rz, float dt, int numiter) {
    
    float amb_temp = 80.0;
    float ce, cw, cn, cs, ct, cb, cc;
    float stepDivCap = dt / Cap;
    ce = cw = stepDivCap / Rx;
    cn = cs = stepDivCap / Ry;
    ct = cb = stepDivCap / Rz;
    cc = 1.0 - (2.0 * ce + 2.0 * cn + 3.0 * ct);

    for (int iter = 0; iter < numiter; iter++) {
        for (int z = 0; z < TILE_DEPTH; z++) {
            for (int y = 0; y < TILE_HEIGHT; y++) {
                for (int x = 0; x < TILE_WIDTH; x++) {
                    //#pragma HLS PIPELINE
                    int c = x + y * TILE_WIDTH + z * TILE_WIDTH * TILE_HEIGHT;
                    int w = (x == 0) ? c : c - 1;
                    int e = (x == TILE_WIDTH - 1) ? c : c + 1;
                    int n = (y == 0) ? c : c - TILE_WIDTH;
                    int s = (y == TILE_HEIGHT - 1) ? c : c + TILE_WIDTH;
                    int b = (z == 0) ? c : c - TILE_WIDTH * TILE_HEIGHT;
                    int t = (z == TILE_DEPTH - 1) ? c : c + TILE_WIDTH * TILE_HEIGHT;

                    local_tOut[c] = local_tIn[c] * cc + local_tIn[n] * cn + local_tIn[s] * cs +
                                    local_tIn[e] * ce + local_tIn[w] * cw + local_tIn[t] * ct +
                                    local_tIn[b] * cb + (dt / Cap) * local_pIn[c] + ct * amb_temp;
                }
            }
        }
        // Swap buffers: Copy local_tOut to local_tIn for next iteration
        for (int i = 0; i < TILE_WIDTH * TILE_HEIGHT * TILE_DEPTH; i++) {
            //#pragma HLS PIPELINE
            local_tIn[i] = local_tOut[i];
        }
    }
}

void hotspot(float *pIn, float* tIn, float *tOut, float Cap, float Rx, float Ry, float Rz, float dt, int numiter) {
    // TODO:
    // Implement halo handling
    float local_pIn[BUFFER_SIZE];
    float local_tIn[BUFFER_SIZE];
    float local_tOut[BUFFER_SIZE];

    // Iterate over tiles in the y-dimension
    for (int yTile = 0; yTile < NY; yTile += TILE_HEIGHT) {
        int actualTileHeight = (yTile + TILE_HEIGHT <= NY) ? TILE_HEIGHT : NY - yTile;
        int tileOffset = yTile * NX * NZ;
        int tileSize = TILE_WIDTH * actualTileHeight * TILE_DEPTH;

        // Load, compute, store for each tile
        load(&pIn[tileOffset], &tIn[tileOffset], local_pIn, local_tIn, tileSize);
        compute(local_pIn, local_tIn, local_tOut, TILE_WIDTH, actualTileHeight, TILE_DEPTH, Cap, Rx, Ry, Rz, dt, numiter);
        store(&tOut[tileOffset], local_tOut, tileSize);
    }
}