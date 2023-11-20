#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h> 
#include <math.h> 
#include <sys/time.h>
#include <string.h>

#define BUFFER_SIZE ... // Maximum buffer size

/* ambient temperature, assuming no package at all	*/
float amb_temp = 80.0;

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

void compute(float local_pIn[BUFFER_SIZE], float local_tIn[BUFFER_SIZE], float local_tOut[BUFFER_SIZE], 
             int nx, int ny, int nz, float Cap, float Rx, float Ry, float Rz, float dt, int numiter) {
    float ce, cw, cn, cs, ct, cb, cc;
    float stepDivCap = dt / Cap;
    ce = cw = stepDivCap / Rx;
    cn = cs = stepDivCap / Ry;
    ct = cb = stepDivCap / Rz;
    cc = 1.0 - (2.0 * ce + 2.0 * cn + 3.0 * ct);

    for (int iter = 0; iter < numiter; iter++) {
        for (int z = 0; z < nz; z++) {
            for (int y = 0; y < ny; y++) {
                for (int x = 0; x < nx; x++) {
                    //#pragma HLS PIPELINE
                    int c = x + y * nx + z * nx * ny;
                    int w = (x == 0) ? c : c - 1;
                    int e = (x == nx - 1) ? c : c + 1;
                    int n = (y == 0) ? c : c - nx;
                    int s = (y == ny - 1) ? c : c + nx;
                    int b = (z == 0) ? c : c - nx * ny;
                    int t = (z == nz - 1) ? c : c + nx * ny;

                    local_tOut[c] = local_tIn[c] * cc + local_tIn[n] * cn + local_tIn[s] * cs + 
                                    local_tIn[e] * ce + local_tIn[w] * cw + local_tIn[t] * ct + 
                                    local_tIn[b] * cb + (dt / Cap) * local_pIn[c] + ct * amb_temp;
                }
            }
        }

        // Swap buffers: Copy local_tOut to local_tIn for next iteration
        for (int i = 0; i < BUFFER_SIZE; i++) {
            //#pragma HLS PIPELINE
            local_tIn[i] = local_tOut[i];
        }
    }
}

void hotspot(float *pIn, float* tIn, float *tOut, int nx, int ny, int nz, float Cap, float Rx, float Ry, float Rz, float dt, int numiter) {
    // TODO:
    // Implement halo handling

    // Tile dimensions (to be calculated)
    int tileWidth, tileHeight, tileDepth;

    // Calculate tile dimensions based on dataset size and buffer constraints
    // Example calculation (adjust as needed based on BUFFER_SIZE and dataset properties)
    tileWidth = nx;  // Full width
    tileDepth = nz;  // Full depth
    tileHeight = BUFFER_SIZE / (tileWidth * tileDepth * sizeof(float)); // Calculate height

    // Ensure tileHeight is within dataset bounds
    if (tileHeight > ny) {
        tileHeight = ny;
    }

    float local_pIn[tileWidth * tileHeight * tileDepth];
    float local_tIn[tileWidth * tileHeight * tileDepth];
    float local_tOut[tileWidth * tileHeight * tileDepth];

    // Iterate over tiles in the y-dimension
    for (int yTile = 0; yTile < ny; yTile += tileHeight) {
        int actualTileHeight = (yTile + tileHeight <= ny) ? tileHeight : ny - yTile;
        int tileOffset = yTile * nx * nz;
        int tileSize = tileWidth * actualTileHeight * tileDepth;

        // Load, compute, store for each tile
        load(&pIn[tileOffset], &tIn[tileOffset], local_pIn, local_tIn, tileSize);
        compute(local_pIn, local_tIn, local_tOut, tileWidth, actualTileHeight, tileDepth, Cap, Rx, Ry, Rz, dt, numiter);
        store(&tOut[tileOffset], local_tOut, tileSize);
    }
}
