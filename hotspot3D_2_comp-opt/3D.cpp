#include "3D.h"

#define TILE_X NX   // Full width
#define TILE_Y 64
#define TILE_Z NZ     // Full depth

#define HALO_BUFFER_SIZE (TILE_X * (TILE_Y + 2) * TILE_Z)
#define BUFFER_SIZE (TILE_X * TILE_Y * TILE_Z)

enum TileStatus {
    NOT_BOUNDARY = 0,
    FIRST_TILE = 1,
    LAST_TILE = 2
};

#define AMB_TEMP 80.0

void load_power(float *pIn, float local_pIn[BUFFER_SIZE], int tileOffset) {
    LOAD_POWER_LOOP:
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
void load_in_with_halo(float *in, float local_in[HALO_BUFFER_SIZE], int tileOffset, TileStatus isBoundary) {
    int globalIdx, localIdx;
    LOAD_HALO_LOOP:
    for (int z = 0; z < TILE_Z; z++) {
        for (int y = -1; y <= TILE_Y; y++) { // Halo cells included
            for (int x = 0; x < TILE_X; x++) {
                localIdx = (y + 1) * TILE_X + z * TILE_X * (TILE_Y + 2) + x;

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
void load(float *pIn, float *tIn, float local_pIn[BUFFER_SIZE], float local_tIn[HALO_BUFFER_SIZE], int tileOffset, TileStatus isBoundary) {
    #pragma HLS inline off
    //#pragma HLS DATAFLOW
    load_power(pIn, local_pIn, tileOffset);
    load_in_with_halo(tIn, local_tIn, tileOffset, isBoundary);
}

void compute(float local_pIn[BUFFER_SIZE], 
             float local_tIn[HALO_BUFFER_SIZE], 
             float local_tOut[BUFFER_SIZE], float stepDivCap,
             float ce, float cw, float cn, float cs, float ct, float cb, float cc) {
    #pragma HLS inline off
    COMPUTE_LOOP_Z:
    for (int z = 0; z < TILE_Z; z++) {
        COMPUTE_LOOP_X:
        for (int x = 0; x < TILE_X; x++) {  // Exclude halo cells
            #pragma HLS loop_flatten 
            #pragma HLS PIPELINE II=1
            COMPUTE_LOOP_Y:
            for (int y = 1; y < TILE_Y + 1; y++) {
                #pragma HLS UNROLL
                int c = x + y * TILE_X + z * TILE_X * (TILE_Y + 2);
                
                int w = (x == 0) ? c      : c - 1;
                int e = (x == TILE_X - 1) ? c : c + 1;
                int n = c - TILE_X;
                int s = c + TILE_X;
                int b = (z == 0) ? c      : c - TILE_X * (TILE_Y + 2);
                int t = (z == NZ - 1) ? c : c + TILE_X * (TILE_Y + 2);

                int c_no_halo = x + (y - 1) * TILE_X + z * TILE_X * TILE_Y;

                local_tOut[c_no_halo] = local_tIn[c] * cc + local_tIn[n] * cn + local_tIn[s] * cs +
                                local_tIn[e] * ce + local_tIn[w] * cw + local_tIn[t] * ct +
                                local_tIn[b] * cb + stepDivCap * local_pIn[c_no_halo] + ct * AMB_TEMP;
            }
        }
    }
}

// Store function
void store(float* tOut, float local_tOut[BUFFER_SIZE], int tileOffset) {
    #pragma HLS inline off
    STORE_LOOP:
    for (int z = 0; z < TILE_Z; z++) {
        for (int y = 0; y < TILE_Y; y++) {
            for (int x = 0; x < TILE_X; x++) {
                int localIdx = y * TILE_X + z * TILE_X * TILE_Y + x;
                int globalIdx = tileOffset + (y * NX + z * NX * NY) + x;                
                tOut[globalIdx] = local_tOut[localIdx];           
            }
        }
    }
}


void hotspot(float *pIn, float* tIn, float *tOut, float stepDivCap,
             float ce, float cw, float cn, float cs, float ct, float cb, float cc) {

    float local_pIn[TILE_Z][TILE_Y][TILE_X];
    #pragma hls array_partition variable=local_pIn cyclic factor=3 dim=1
    #pragma hls array_partition variable=local_pIn complete  dim=2
    #pragma hls array_partition variable=local_pIn cyclic factor=3 dim=3

    float local_tIn[TILE_Z][TILE_Y + 2][TILE_X];
    #pragma hls array_partition variable=local_tIn cyclic factor=3 dim=1
    #pragma hls array_partition variable=local_tIn complete  dim=2
    #pragma hls array_partition variable=local_tIn cyclic factor=3 dim=3

    float local_tOut[TILE_Z][TILE_Y][TILE_X];
    #pragma hls array_partition variable=local_tOut cyclic factor=3 dim=1
    #pragma hls array_partition variable=local_tOut complete  dim=2
    #pragma hls array_partition variable=local_tOut cyclic factor=3 dim=3

    ITERATION_LOOP:
    for (int iter = 0; iter < NUMITER/2; iter++) {
        TILE_LOOP:
        for (int yTile = 0; yTile < NY; yTile += TILE_Y) {
            TileStatus boundaryFlag = NOT_BOUNDARY;
            if (yTile == 0 ) {
                boundaryFlag = FIRST_TILE;
            } else if (yTile == NY - TILE_Y) {
                boundaryFlag = LAST_TILE;
            }

            int tileOffset = yTile * NX;

            // Load data for each tile with boundary handling
            load(pIn, tIn, &local_pIn[0][0][0], &local_tIn[0][0][0], tileOffset, boundaryFlag);

            // Compute temperatures for each tile
            compute(&local_pIn[0][0][0], &local_tIn[0][0][0], &local_tOut[0][0][0], stepDivCap, ce, cw, cn, cs, ct, cb, cc);

            // Store only the computational region of each tile
            store(tOut, &local_tOut[0][0][0], tileOffset);
        }

        for (int yTile = 0; yTile < NY; yTile += TILE_Y) {
            TileStatus boundaryFlag = NOT_BOUNDARY;
            if (yTile == 0 ) {
                boundaryFlag = FIRST_TILE;
            } else if (yTile == NY - TILE_Y) {
                boundaryFlag = LAST_TILE;
            }

            int tileOffset = yTile * NX;

            // Load data for each tile with boundary handling
            load(pIn, tOut, &local_pIn[0][0][0], &local_tIn[0][0][0], tileOffset, boundaryFlag);

            // Compute temperatures for each tile
            compute(&local_pIn[0][0][0], &local_tIn[0][0][0], &local_tOut[0][0][0], stepDivCap, ce, cw, cn, cs, ct, cb, cc);

            // Store only the computational region of each tile
            store(tIn, &local_tOut[0][0][0], tileOffset);
        }
    }
}