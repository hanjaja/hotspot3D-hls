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
    #pragma HLS INLINE OFF
    LOAD_POWER_LOOP:
    for (int z = 0; z < TILE_Z; z++) {
        int globalIdx = tileOffset + z * NX * NY;
        memcpy_wide_bus_read_float((float*)local_pIn[z], (class ap_uint<LARGE_BUS> *)(pIn + globalIdx/WIDTH_FACTOR), 0, sizeof(float) * TILE_X * TILE_Y);             
    }
    return;
}

// load in with halo
void load_in_with_halo(INTERFACE_WIDTH *in, float local_in[TILE_Z][TILE_Y + 2][TILE_X], int tileOffset, TileStatus isBoundary) {
    #pragma HLS INLINE OFF
    int globalIdx, localIdx;
    LOAD_HALO_LOOP_Z:
    for (int z = 0; z < TILE_Z; z++) {
        LOAD_HALO_LOOP_Y:
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
    return;
}

// Combined load function with halo handling
void load(int flag, INTERFACE_WIDTH *pIn, INTERFACE_WIDTH *tIn, float local_pIn[TILE_Z][TILE_Y][TILE_X], float local_tIn[TILE_Z][TILE_Y + 2][TILE_X], int tileOffset, TileStatus isBoundary) {
    //#pragma HLS INLINE OFF
    //#pragma HLS DATAFLOW
    if (flag) {
        load_power(pIn, local_pIn, tileOffset);
        load_in_with_halo(tIn, local_tIn, tileOffset, isBoundary);
    }
    return;
}

void compute(int flag, float local_pIn[TILE_Z][TILE_Y][TILE_X], 
             float local_tIn[TILE_Z][TILE_Y + 2][TILE_X], 
             float local_tOut[TILE_Z][TILE_Y][TILE_X], float stepDivCap,
             float ce, float cw, float cn, float cs, float ct, float cb, float cc) {
    //#pragma HLS inline off
    if (flag) {
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
    return;
}

// Store function
void store(int flag, INTERFACE_WIDTH* tOut, float local_tOut[TILE_Z][TILE_Y][TILE_X], int tileOffset) {
    //#pragma HLS INLINE OFF
    if (flag) {
        STORE_LOOP:
        for (int z = 0; z < TILE_Z; z++) {
            int localIdx = z * TILE_X * TILE_Y;
            int globalIdx = tileOffset + z * NX * NY;
            //local_pIn[localIdx] = pIn[globalIdx];   

            memcpy_wide_bus_write_float((class ap_uint<LARGE_BUS> *)(tOut + globalIdx/WIDTH_FACTOR), (float*)local_tOut[z], 0, sizeof(float) * TILE_X * TILE_Y);             
        }
    }
    return;
}

void hotspot(INTERFACE_WIDTH pIn[NX*NY*NZ/WIDTH_FACTOR], INTERFACE_WIDTH tIn[NX*NY*NZ/WIDTH_FACTOR], INTERFACE_WIDTH tOut[NX*NY*NZ/WIDTH_FACTOR], 
             float stepDivCap, float ce, float cw, float cn, float cs, float ct, float cb, float cc) {

    // float local_pIn[TILE_Z][TILE_Y][TILE_X];
    // #pragma hls array_partition variable=local_pIn cyclic factor=3 dim=1
    // #pragma hls array_partition variable=local_pIn complete  dim=2
    // #pragma hls array_partition variable=local_pIn cyclic factor=3 dim=3

    // float local_tIn[TILE_Z][TILE_Y + 2][TILE_X];
    // #pragma hls array_partition variable=local_tIn cyclic factor=3 dim=1
    // #pragma hls array_partition variable=local_tIn complete  dim=2
    // #pragma hls array_partition variable=local_tIn cyclic factor=3 dim=3

    // float local_tOut[TILE_Z][TILE_Y][TILE_X];
    // #pragma hls array_partition variable=local_tOut cyclic factor=3 dim=1
    // #pragma hls array_partition variable=local_tOut complete  dim=2
    // #pragma hls array_partition variable=local_tOut cyclic factor=3 dim=3

    float pIn_0[TILE_Z][TILE_Y][TILE_X];
    #pragma hls array_partition variable=pIn_0 cyclic factor=3 dim=1
    #pragma hls array_partition variable=pIn_0 complete  dim=2
    #pragma hls array_partition variable=pIn_0 cyclic factor=3 dim=3

    float pIn_1[TILE_Z][TILE_Y][TILE_X];
    #pragma hls array_partition variable=pIn_1 cyclic factor=3 dim=1
    #pragma hls array_partition variable=pIn_1 complete  dim=2
    #pragma hls array_partition variable=pIn_1 cyclic factor=3 dim=3

    float pIn_2[TILE_Z][TILE_Y][TILE_X];
    #pragma hls array_partition variable=pIn_1 cyclic factor=3 dim=1
    #pragma hls array_partition variable=pIn_1 complete  dim=2
    #pragma hls array_partition variable=pIn_1 cyclic factor=3 dim=3

    float tIn_0[TILE_Z][TILE_Y + 2][TILE_X];
    #pragma hls array_partition variable=tIn_0 cyclic factor=3 dim=1
    #pragma hls array_partition variable=tIn_0 complete  dim=2
    #pragma hls array_partition variable=tIn_0 cyclic factor=3 dim=3

    float tIn_1[TILE_Z][TILE_Y + 2][TILE_X];
    #pragma hls array_partition variable=tIn_1 cyclic factor=3 dim=1
    #pragma hls array_partition variable=tIn_1 complete  dim=2
    #pragma hls array_partition variable=tIn_1 cyclic factor=3 dim=3

    float tIn_2[TILE_Z][TILE_Y + 2][TILE_X];
    #pragma hls array_partition variable=tIn_1 cyclic factor=3 dim=1
    #pragma hls array_partition variable=tIn_1 complete  dim=2
    #pragma hls array_partition variable=tIn_1 cyclic factor=3 dim=3

    float tOut_0[TILE_Z][TILE_Y][TILE_X];
    #pragma hls array_partition variable=tOut_0 cyclic factor=3 dim=1
    #pragma hls array_partition variable=tOut_0 complete  dim=2
    #pragma hls array_partition variable=tOut_0 cyclic factor=3 dim=3

    float tOut_1[TILE_Z][TILE_Y][TILE_X];
    #pragma hls array_partition variable=tOut_1 cyclic factor=3 dim=1
    #pragma hls array_partition variable=tOut_1 complete  dim=2
    #pragma hls array_partition variable=tOut_1 cyclic factor=3 dim=3

    float tOut_2[TILE_Z][TILE_Y][TILE_X];
    #pragma hls array_partition variable=tOut_1 cyclic factor=3 dim=1
    #pragma hls array_partition variable=tOut_1 complete  dim=2
    #pragma hls array_partition variable=tOut_1 cyclic factor=3 dim=3


    ITERATION_LOOP:
    for (int iter = 0; iter < NUMITER/2; iter++) {
        TILE_LOOP_1:
        for (int k = 0; k < NY / TILE_Y + 2; k++) {
            int yTile = k * TILE_Y;

            // int load_flag = yTile >= 0 && yTile < NY; 
            // int compute_flag = yTile >= TILE_Y;
            // int store_flag = compute_flag;

            int load_flag = k >= 0 && k < NY / TILE_Y;
            int compute_flag = k >= 1 && k < NY / TILE_Y + 1;
            int store_flag = k >= 2 && k < NY / TILE_Y + 2;

            TileStatus boundaryFlag = NOT_BOUNDARY;
            if (yTile == 0 ) {
                boundaryFlag = FIRST_TILE;
            } else if (yTile == NY - TILE_Y) {
                boundaryFlag = LAST_TILE;
            }

            int tileOffset = yTile * NX;

            if (k % 3 == 0) {
                load(load_flag, pIn, tIn, pIn_0, tIn_0, tileOffset, boundaryFlag);
                compute(compute_flag, pIn_2, tIn_2, tOut_2, stepDivCap, ce, cw, cn, cs, ct, cb, cc);
                store(store_flag, tOut, tOut_1, tileOffset - 2*TILE_Y * NX);
            } else if (k % 3 == 1) {
                load(load_flag, pIn, tIn, pIn_1, tIn_1, tileOffset, boundaryFlag);
                compute(compute_flag, pIn_0, tIn_0, tOut_0, stepDivCap, ce, cw, cn, cs, ct, cb, cc);
                store(store_flag, tOut, tOut_2, tileOffset - 2*TILE_Y * NX);
            } else {
                load(load_flag, pIn, tIn, pIn_2, tIn_2, tileOffset, boundaryFlag);
                compute(compute_flag, pIn_1, tIn_1, tOut_1, stepDivCap, ce, cw, cn, cs, ct, cb, cc);
                store(store_flag, tOut, tOut_0, tileOffset - 2*TILE_Y * NX);
            }
        }

        TILE_LOOP_2:
        for (int k = 0; k < NY / TILE_Y + 2; k++) {
            int yTile = k * TILE_Y;

            // int load_flag = yTile >= 0 && yTile < NY; 
            // int compute_flag = yTile >= TILE_Y;
            // int store_flag = compute_flag;

            int load_flag = k >= 0 && k < NY / TILE_Y;
            int compute_flag = k >= 1 && k < NY / TILE_Y + 1;
            int store_flag = k >= 2 && k < NY / TILE_Y + 2;

            TileStatus boundaryFlag = NOT_BOUNDARY;
            if (yTile == 0 ) {
                boundaryFlag = FIRST_TILE;
            } else if (yTile == NY - TILE_Y) {
                boundaryFlag = LAST_TILE;
            }

            int tileOffset = yTile * NX;

            if (k % 3 == 0) {
                load(load_flag, pIn, tOut, pIn_0, tIn_0, tileOffset, boundaryFlag);
                compute(compute_flag, pIn_2, tIn_2, tOut_2, stepDivCap, ce, cw, cn, cs, ct, cb, cc);
                store(store_flag, tIn, tOut_1, tileOffset - 2*TILE_Y * NX);
            } else if (k % 3 == 1) {
                load(load_flag, pIn, tOut, pIn_1, tIn_1, tileOffset, boundaryFlag);
                compute(compute_flag, pIn_0, tIn_0, tOut_0, stepDivCap, ce, cw, cn, cs, ct, cb, cc);
                store(store_flag, tIn, tOut_2, tileOffset - 2*TILE_Y * NX);
            } else {
                load(load_flag, pIn, tOut, pIn_2, tIn_2, tileOffset, boundaryFlag);
                compute(compute_flag, pIn_1, tIn_1, tOut_1, stepDivCap, ce, cw, cn, cs, ct, cb, cc);
                store(store_flag, tIn, tOut_0, tileOffset - 2*TILE_Y * NX);
            }
        }
    }
    return;
}
