#include "3D.h"

#define TILE_X NX   // Full width
#define TILE_Y 16
#define TILE_Z NZ     // Full depth

#define HALO_BUFFER_SIZE (TILE_X * (TILE_Y + 2) * TILE_Z)
#define BUFFER_SIZE (TILE_X * TILE_Y * TILE_Z)

enum TileStatus {
    NOT_BOUNDARY = 0,
    FIRST_TILE = 1,
    LAST_TILE = 2
};

#define AMB_TEMP 80.0

void load_power(INTERFACE_WIDTH *pIn, float local_pIn[TILE_Z][TILE_Y][TILE_X], int tileOffset) {
    LOAD_POWER_LOOP:
    for (int z = 0; z < TILE_Z; z++) {
        for ( int y = 0; y < TILE_Y; y++){
            int globalIdx = tileOffset + (y * NX + z * NX * NY);
            memcpy_wide_bus_read_float(local_pIn[z][y], (class ap_uint<LARGE_BUS> *)(pIn + globalIdx/WIDTH_FACTOR), 0, sizeof(float) * TILE_X);             
        }
    }
}

// load in with halo
void load_in_with_halo(INTERFACE_WIDTH *in, float local_in[TILE_Z][TILE_Y + 2][TILE_X], int tileOffset, TileStatus isBoundary) {
    int globalIdx, localIdx;
    LOAD_HALO_LOOP:
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

            globalIdx = tileOffset + (haloY * NX + z * NX * NY);

            memcpy_wide_bus_read_float(local_in[z][y+1], (class ap_uint<LARGE_BUS> *)(in + globalIdx/WIDTH_FACTOR), 0, sizeof(float)*TILE_X);
        }
    }
}

// Combined load function with halo handling
void load(INTERFACE_WIDTH *pIn, INTERFACE_WIDTH *tIn, float local_pIn[TILE_Z][TILE_Y][TILE_X], float local_tIn[TILE_Z][TILE_Y + 2][TILE_X], int tileOffset, TileStatus isBoundary) {
    #pragma HLS INLINE OFF
    #pragma HLS DATAFLOW
    load_power(pIn, local_pIn, tileOffset);
    load_in_with_halo(tIn, local_tIn, tileOffset, isBoundary);
}

void compute(float local_pIn[TILE_Z][TILE_Y][TILE_X], 
             float local_tIn[TILE_Z][TILE_Y + 2][TILE_X], 
             float local_tOut[TILE_Z][TILE_Y][TILE_X], float stepDivCap,
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
                
                int w = (x == 0) ? 0  : -1;
                int e = (x == TILE_X - 1) ? 0 : 1;
                int b = (z == 0) ? 0      : -1;
                int t = (z == NZ - 1) ? 0 : 1;

                int c_no_halo = x + (y - 1) * TILE_X + z * TILE_X * TILE_Y;

                local_tOut[z][y-1][x] = local_tIn[z][y][x] * cc + local_tIn[z][y-1][x] * cn + local_tIn[z][y+1][x] * cs +
                                local_tIn[z][y][x+e] * ce + local_tIn[z][y][x+w] * cw + local_tIn[z+t][y][x] * ct +
                                local_tIn[z+b][y][x] * cb + stepDivCap * local_pIn[z][y-1][x] + ct * AMB_TEMP;
            }
        }
    }
}

// Store function
void store(INTERFACE_WIDTH* tOut, float local_tOut[TILE_Z][TILE_Y][TILE_X], int tileOffset) {
    #pragma HLS INLINE OFF
    STORE_LOOP:
    for (int z = 0; z < TILE_Z; z++) {
        for ( int y = 0; y < TILE_Y; y++){
            int globalIdx = tileOffset + (y * NX + z * NX * NY);  
            memcpy_wide_bus_write_float((class ap_uint<LARGE_BUS> *)(tOut + globalIdx/WIDTH_FACTOR),local_tOut[z][y], 0, sizeof(float) * TILE_X);             
        }
    }
}

void hotspot(INTERFACE_WIDTH pIn[NX*NY*NZ/WIDTH_FACTOR], INTERFACE_WIDTH tIn[NX*NY*NZ/WIDTH_FACTOR], INTERFACE_WIDTH tOut[NX*NY*NZ/WIDTH_FACTOR], 
             float stepDivCap, float ce, float cw, float cn, float cs, float ct, float cb, float cc) {
    
    float local_pIn[TILE_Z][TILE_Y][TILE_X];    
    #pragma hls array_partition variable=local_pIn cyclic factor=3 dim=1
    #pragma hls array_partition variable=local_pIn complete  dim=2
    #pragma hls array_partition variable=local_pIn cyclic factor=6 dim=3

    float local_tIn[TILE_Z][TILE_Y + 2][TILE_X];
    #pragma hls array_partition variable=local_tIn cyclic factor=3 dim=1
    #pragma hls array_partition variable=local_tIn complete  dim=2
    #pragma hls array_partition variable=local_tIn cyclic factor=6 dim=3

    float local_tOut[TILE_Z][TILE_Y][TILE_X];
    #pragma HLS BIND_STORAGE variable=local_pIn type=RAM_1P impl=URAM
    #pragma hls array_partition variable=local_tOut cyclic factor=3 dim=1
    #pragma hls array_partition variable=local_tOut complete  dim=2
    #pragma hls array_partition variable=local_tOut cyclic factor=6 dim=3

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
            load(pIn, tIn, local_pIn, local_tIn, tileOffset, boundaryFlag);

            // Compute temperatures for each tile
            compute(local_pIn, local_tIn, local_tOut, stepDivCap, ce, cw, cn, cs, ct, cb, cc);

            // Store only the computational region of each tile
            store(tOut, local_tOut, tileOffset);
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
            load(pIn, tOut, local_pIn, local_tIn, tileOffset, boundaryFlag);

            // Compute temperatures for each tile
            compute(local_pIn, local_tIn, local_tOut, stepDivCap, ce, cw, cn, cs, ct, cb, cc);

            // Store only the computational region of each tile
            store(tIn, local_tOut, tileOffset);
        }
    }
}