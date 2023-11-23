#include "3D.h"

#define TILE_X NX   // Full width
#define TILE_Y 64
#define TILE_Z NZ     // Full depth

#define HALO_BUFFER_SIZE (TILE_X * (TILE_Y + 2) * TILE_Z)
#define BUFFER_SIZE (TILE_X * TILE_Y * TILE_Z)

#define NOT_BOUNDARY 0
#define FIRST_TILE 1
#define LAST_TILE 2

#define AMB_TEMP 80.0

void load_power(INTERFACE_WIDTH *pIn, float local_pIn[BUFFER_SIZE], int tileOffset) {
    for (int z = 0; z < TILE_Z; z++) {
        for (int y = 0; y < TILE_Y; y++) {
            int localIdx = y * TILE_X + z * TILE_X * TILE_Y;
            int globalIdx = tileOffset + (y * NX + z * NX * NY);
            //local_pIn[localIdx] = pIn[globalIdx];   

            memcpy_wide_bus_read_float(local_pIn + localIdx, pIn + globalIdx, 0, sizeof(float)*TILE_X);             
        }
    }
}

// load in with halo
void load_in_with_halo(INTERFACE_WIDTH *in, float local_in[HALO_BUFFER_SIZE], int tileOffset, int isBoundary) {
    int globalIdx, localIdx;
    for (int z = 0; z < TILE_Z; z++) {
        for (int y = -1; y <= TILE_Y; y++) { // Halo cells included
            localIdx = (y + 1) * TILE_X + z * TILE_X * (TILE_Y + 2);

            int haloY;

            if (isBoundary == FIRST_TILE) {
                haloY = (y == -1) ? 0 : y;
            } else if (isBoundary == LAST_TILE) {
                haloY = (y == TILE_Y) ? TILE_Y - 1 : y;
            } else {
                haloY = y;
            }

            globalIdx = tileOffset + (haloY * NX + z * NX * NY) + x;

            //local_in[localIdx] = in[globalIdx];

            memcpy_wide_bus_read_float(local_in + localIdx, in + globalIdx, 0, sizeof(float)*TILE_X);
        }
    }
}

// Combined load function with halo handling
void load(float *pIn, float *tIn, float local_pIn[BUFFER_SIZE], float local_tIn[HALO_BUFFER_SIZE], int tileOffset, int isBoundary) {
    //#pragma HLS INLINE OFF
    //#pragma HLS DATAFLOW
    load_power(pIn, local_pIn, tileOffset);
    load_in_with_halo(tIn, local_tIn, tileOffset, isBoundary);
}

void compute(float local_pIn[BUFFER_SIZE], 
             float local_tIn[HALO_BUFFER_SIZE], 
             float local_tOut[BUFFER_SIZE],
             float Cap, float Rx, float Ry, float Rz, float dt) {
   // #pragma HLS INLINE OFF
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
                
                int w = (x == 0) ? c      : c - 1;
                int e = (x == TILE_X - 1) ? c : c + 1;
                int n = c - TILE_X;
                int s = c + TILE_X;
                int b = (z == 0) ? c      : c - TILE_X * (TILE_Y + 2);
                int t = (z == NZ - 1) ? c : c + TILE_X * (TILE_Y + 2);

                int c_no_halo = x + (y - 1) * TILE_X + z * TILE_X * TILE_Y;

                local_tOut[c_no_halo] = local_tIn[c] * cc + local_tIn[n] * cn + local_tIn[s] * cs +
                                local_tIn[e] * ce + local_tIn[w] * cw + local_tIn[t] * ct +
                                local_tIn[b] * cb + (dt / Cap) * local_pIn[c_no_halo] + ct * AMB_TEMP;
            }
        }
    }
}

// Store function
void store(INTERFACE_WIDTH* tOut, float local_tOut[BUFFER_SIZE], int tileOffset) {
   // #pragma HLS INLINE OFF
    for (int z = 0; z < TILE_Z; z++) {
        for (int y = 0; y < TILE_Y; y++) {
            int localIdx = y * TILE_X + z * TILE_X * TILE_Y + x;
            int globalIdx = tileOffset + (y * NX + z * NX * NY) + x;                
            //tOut[globalIdx] = local_tOut[localIdx];           
            memcpy_wide_bus_write_float(tOut + globalIdx, local_tOut + localIdx, 0, sizeof(float)*TILE_X);
        }
    }
}

void hotspot(INTERFACE_WIDTH pIn[NX*NY*NZ/WIDTH_FACTOR], INTERFACE_WIDTH tIn[NX*NY*NZ/WIDTH_FACTOR], INTERFACE_WIDTH tOut[NX*NY*NZ/WIDTH_FACTOR], float Cap, float Rx, float Ry, float Rz, float dt) {
    float local_pIn[TILE_Z][TILE_Y][TILE_X];
    #pragma hls array_partition variable=local_pIn cyclic factor=3 dim=1
    #pragma hls array_partition variable=local_pIn cyclic factor=3 dim=2
    #pragma hls array_partition variable=local_pIn complete dim=3

    float local_tIn[TILE_Z][TILE_Y + 2][TILE_X];
    #pragma hls array_partition variable=local_tIn cyclic factor=3 dim=1
    #pragma hls array_partition variable=local_tIn cyclic factor=3 dim=2
    #pragma hls array_partition variable=local_tIn complete dim=3

    float local_tOut[TILE_Z][TILE_Y][TILE_X];
    #pragma hls array_partition variable=local_tOut cyclic factor=3 dim=1
    #pragma hls array_partition variable=local_tOut cyclic factor=3 dim=2
    #pragma hls array_partition variable=local_tOut complete dim=3

    for (int iter = 0; iter < NUMITER; iter++) {
        for (int yTile = 0; yTile < NY; yTile += TILE_Y) {
            int boundaryFlag = NOT_BOUNDARY;
            if (yTile == 0 ) {
                boundaryFlag = FIRST_TILE;
            } else if (yTile == NY - TILE_Y) {
                boundaryFlag = LAST_TILE;
            }

            int tileOffset = yTile * NX;

            // Load data for each tile with boundary handling
            load(pIn, tIn, local_pIn, local_tIn, tileOffset, boundaryFlag);

            // Compute temperatures for each tile
            compute(local_pIn, local_tIn, local_tOut, Cap, Rx, Ry, Rz, dt);

            // Store only the computational region of each tile
            store(tOut, local_tOut, tileOffset);
        }

        // Swap input and output buffers for the next iteration
        float* temp = tIn;
        tIn = tOut;
        tOut = temp;
    }
}