#include <systemc>
#include <vector>
#include <iostream>
#include "hls_stream.h"
#include "ap_int.h"
#include "ap_fixed.h"
using namespace std;
using namespace sc_dt;
class AESL_RUNTIME_BC {
  public:
    AESL_RUNTIME_BC(const char* name) {
      file_token.open( name);
      if (!file_token.good()) {
        cout << "Failed to open tv file " << name << endl;
        exit (1);
      }
      file_token >> mName;//[[[runtime]]]
    }
    ~AESL_RUNTIME_BC() {
      file_token.close();
    }
    int read_size () {
      int size = 0;
      file_token >> mName;//[[transaction]]
      file_token >> mName;//transaction number
      file_token >> mName;//pop_size
      size = atoi(mName.c_str());
      file_token >> mName;//[[/transaction]]
      return size;
    }
  public:
    fstream file_token;
    string mName;
};
extern "C" void computeTempFPGA(volatile void *, volatile void *, volatile void *, int, int, int, float, float, float, float, float, int);
extern "C" void apatb_computeTempFPGA_hw(volatile void * __xlx_apatb_param_pIn, volatile void * __xlx_apatb_param_tIn, volatile void * __xlx_apatb_param_tOut, int __xlx_apatb_param_nx, int __xlx_apatb_param_ny, int __xlx_apatb_param_nz, float __xlx_apatb_param_Cap, float __xlx_apatb_param_Rx, float __xlx_apatb_param_Ry, float __xlx_apatb_param_Rz, float __xlx_apatb_param_dt, int __xlx_apatb_param_numiter) {
  // DUT call
  computeTempFPGA(__xlx_apatb_param_pIn, __xlx_apatb_param_tIn, __xlx_apatb_param_tOut, __xlx_apatb_param_nx, __xlx_apatb_param_ny, __xlx_apatb_param_nz, __xlx_apatb_param_Cap, __xlx_apatb_param_Rx, __xlx_apatb_param_Ry, __xlx_apatb_param_Rz, __xlx_apatb_param_dt, __xlx_apatb_param_numiter);
}
