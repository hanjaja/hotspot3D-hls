#include <systemc>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <stdint.h>
#include "SysCFileHandler.h"
#include "ap_int.h"
#include "ap_fixed.h"
#include <complex>
#include <stdbool.h>
#include "autopilot_cbe.h"
#include "hls_stream.h"
#include "hls_half.h"
#include "hls_signal_handler.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

// wrapc file define:
#define AUTOTB_TVIN_pIn "../tv/cdatafile/c.computeTempFPGA.autotvin_pIn.dat"
#define AUTOTB_TVOUT_pIn "../tv/cdatafile/c.computeTempFPGA.autotvout_pIn.dat"
// wrapc file define:
#define AUTOTB_TVIN_tIn "../tv/cdatafile/c.computeTempFPGA.autotvin_tIn.dat"
#define AUTOTB_TVOUT_tIn "../tv/cdatafile/c.computeTempFPGA.autotvout_tIn.dat"
// wrapc file define:
#define AUTOTB_TVIN_tOut "../tv/cdatafile/c.computeTempFPGA.autotvin_tOut.dat"
#define AUTOTB_TVOUT_tOut "../tv/cdatafile/c.computeTempFPGA.autotvout_tOut.dat"
// wrapc file define:
#define AUTOTB_TVIN_nx "../tv/cdatafile/c.computeTempFPGA.autotvin_nx.dat"
#define AUTOTB_TVOUT_nx "../tv/cdatafile/c.computeTempFPGA.autotvout_nx.dat"
// wrapc file define:
#define AUTOTB_TVIN_ny "../tv/cdatafile/c.computeTempFPGA.autotvin_ny.dat"
#define AUTOTB_TVOUT_ny "../tv/cdatafile/c.computeTempFPGA.autotvout_ny.dat"
// wrapc file define:
#define AUTOTB_TVIN_nz "../tv/cdatafile/c.computeTempFPGA.autotvin_nz.dat"
#define AUTOTB_TVOUT_nz "../tv/cdatafile/c.computeTempFPGA.autotvout_nz.dat"
// wrapc file define:
#define AUTOTB_TVIN_Cap "../tv/cdatafile/c.computeTempFPGA.autotvin_Cap.dat"
#define AUTOTB_TVOUT_Cap "../tv/cdatafile/c.computeTempFPGA.autotvout_Cap.dat"
// wrapc file define:
#define AUTOTB_TVIN_Rx "../tv/cdatafile/c.computeTempFPGA.autotvin_Rx.dat"
#define AUTOTB_TVOUT_Rx "../tv/cdatafile/c.computeTempFPGA.autotvout_Rx.dat"
// wrapc file define:
#define AUTOTB_TVIN_Ry "../tv/cdatafile/c.computeTempFPGA.autotvin_Ry.dat"
#define AUTOTB_TVOUT_Ry "../tv/cdatafile/c.computeTempFPGA.autotvout_Ry.dat"
// wrapc file define:
#define AUTOTB_TVIN_Rz "../tv/cdatafile/c.computeTempFPGA.autotvin_Rz.dat"
#define AUTOTB_TVOUT_Rz "../tv/cdatafile/c.computeTempFPGA.autotvout_Rz.dat"
// wrapc file define:
#define AUTOTB_TVIN_dt "../tv/cdatafile/c.computeTempFPGA.autotvin_dt.dat"
#define AUTOTB_TVOUT_dt "../tv/cdatafile/c.computeTempFPGA.autotvout_dt.dat"
// wrapc file define:
#define AUTOTB_TVIN_numiter "../tv/cdatafile/c.computeTempFPGA.autotvin_numiter.dat"
#define AUTOTB_TVOUT_numiter "../tv/cdatafile/c.computeTempFPGA.autotvout_numiter.dat"

#define INTER_TCL "../tv/cdatafile/ref.tcl"

// tvout file define:
#define AUTOTB_TVOUT_PC_pIn "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_pIn.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_tIn "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_tIn.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_tOut "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_tOut.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_nx "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_nx.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_ny "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_ny.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_nz "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_nz.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_Cap "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_Cap.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_Rx "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_Rx.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_Ry "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_Ry.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_Rz "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_Rz.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_dt "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_dt.dat"
// tvout file define:
#define AUTOTB_TVOUT_PC_numiter "../tv/rtldatafile/rtl.computeTempFPGA.autotvout_numiter.dat"

class INTER_TCL_FILE {
  public:
INTER_TCL_FILE(const char* name) {
  mName = name; 
  pIn_depth = 0;
  tIn_depth = 0;
  tOut_depth = 0;
  nx_depth = 0;
  ny_depth = 0;
  nz_depth = 0;
  Cap_depth = 0;
  Rx_depth = 0;
  Ry_depth = 0;
  Rz_depth = 0;
  dt_depth = 0;
  numiter_depth = 0;
  trans_num =0;
}
~INTER_TCL_FILE() {
  mFile.open(mName);
  if (!mFile.good()) {
    cout << "Failed to open file ref.tcl" << endl;
    exit (1); 
  }
  string total_list = get_depth_list();
  mFile << "set depth_list {\n";
  mFile << total_list;
  mFile << "}\n";
  mFile << "set trans_num "<<trans_num<<endl;
  mFile.close();
}
string get_depth_list () {
  stringstream total_list;
  total_list << "{pIn " << pIn_depth << "}\n";
  total_list << "{tIn " << tIn_depth << "}\n";
  total_list << "{tOut " << tOut_depth << "}\n";
  total_list << "{nx " << nx_depth << "}\n";
  total_list << "{ny " << ny_depth << "}\n";
  total_list << "{nz " << nz_depth << "}\n";
  total_list << "{Cap " << Cap_depth << "}\n";
  total_list << "{Rx " << Rx_depth << "}\n";
  total_list << "{Ry " << Ry_depth << "}\n";
  total_list << "{Rz " << Rz_depth << "}\n";
  total_list << "{dt " << dt_depth << "}\n";
  total_list << "{numiter " << numiter_depth << "}\n";
  return total_list.str();
}
void set_num (int num , int* class_num) {
  (*class_num) = (*class_num) > num ? (*class_num) : num;
}
void set_string(std::string list, std::string* class_list) {
  (*class_list) = list;
}
  public:
    int pIn_depth;
    int tIn_depth;
    int tOut_depth;
    int nx_depth;
    int ny_depth;
    int nz_depth;
    int Cap_depth;
    int Rx_depth;
    int Ry_depth;
    int Rz_depth;
    int dt_depth;
    int numiter_depth;
    int trans_num;
  private:
    ofstream mFile;
    const char* mName;
};

static void RTLOutputCheckAndReplacement(std::string &AESL_token, std::string PortName) {
  bool no_x = false;
  bool err = false;

  no_x = false;
  // search and replace 'X' with '0' from the 3rd char of token
  while (!no_x) {
    size_t x_found = AESL_token.find('X', 0);
    if (x_found != string::npos) {
      if (!err) { 
        cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port" 
             << PortName << ", possible cause: There are uninitialized variables in the C design."
             << endl; 
        err = true;
      }
      AESL_token.replace(x_found, 1, "0");
    } else
      no_x = true;
  }
  no_x = false;
  // search and replace 'x' with '0' from the 3rd char of token
  while (!no_x) {
    size_t x_found = AESL_token.find('x', 2);
    if (x_found != string::npos) {
      if (!err) { 
        cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'x' on port" 
             << PortName << ", possible cause: There are uninitialized variables in the C design."
             << endl; 
        err = true;
      }
      AESL_token.replace(x_found, 1, "0");
    } else
      no_x = true;
  }
}
extern "C" void computeTempFPGA_hw_stub_wrapper(volatile void *, volatile void *, volatile void *, int, int, int, float, float, float, float, float, int);

extern "C" void apatb_computeTempFPGA_hw(volatile void * __xlx_apatb_param_pIn, volatile void * __xlx_apatb_param_tIn, volatile void * __xlx_apatb_param_tOut, int __xlx_apatb_param_nx, int __xlx_apatb_param_ny, int __xlx_apatb_param_nz, float __xlx_apatb_param_Cap, float __xlx_apatb_param_Rx, float __xlx_apatb_param_Ry, float __xlx_apatb_param_Rz, float __xlx_apatb_param_dt, int __xlx_apatb_param_numiter) {
  refine_signal_handler();
  fstream wrapc_switch_file_token;
  wrapc_switch_file_token.open(".hls_cosim_wrapc_switch.log");
  int AESL_i;
  if (wrapc_switch_file_token.good())
  {

    CodeState = ENTER_WRAPC_PC;
    static unsigned AESL_transaction_pc = 0;
    string AESL_token;
    string AESL_num;{
      static ifstream rtl_tv_out_file;
      if (!rtl_tv_out_file.is_open()) {
        rtl_tv_out_file.open(AUTOTB_TVOUT_PC_tIn);
        if (rtl_tv_out_file.good()) {
          rtl_tv_out_file >> AESL_token;
          if (AESL_token != "[[[runtime]]]")
            exit(1);
        }
      }
  
      if (rtl_tv_out_file.good()) {
        rtl_tv_out_file >> AESL_token; 
        rtl_tv_out_file >> AESL_num;  // transaction number
        if (AESL_token != "[[transaction]]") {
          cerr << "Unexpected token: " << AESL_token << endl;
          exit(1);
        }
        if (atoi(AESL_num.c_str()) == AESL_transaction_pc) {
          std::vector<sc_bv<32> > tIn_pc_buffer(1);
          int i = 0;

          rtl_tv_out_file >> AESL_token; //data
          while (AESL_token != "[[/transaction]]"){

            RTLOutputCheckAndReplacement(AESL_token, "tIn");
  
            // push token into output port buffer
            if (AESL_token != "") {
              tIn_pc_buffer[i] = AESL_token.c_str();;
              i++;
            }
  
            rtl_tv_out_file >> AESL_token; //data or [[/transaction]]
            if (AESL_token == "[[[/runtime]]]" || rtl_tv_out_file.eof())
              exit(1);
          }
          if (i > 0) {
            ((int*)__xlx_apatb_param_tIn)[0] = tIn_pc_buffer[0].to_int64();
          }
        } // end transaction
      } // end file is good
    } // end post check logic bolck
  {
      static ifstream rtl_tv_out_file;
      if (!rtl_tv_out_file.is_open()) {
        rtl_tv_out_file.open(AUTOTB_TVOUT_PC_tOut);
        if (rtl_tv_out_file.good()) {
          rtl_tv_out_file >> AESL_token;
          if (AESL_token != "[[[runtime]]]")
            exit(1);
        }
      }
  
      if (rtl_tv_out_file.good()) {
        rtl_tv_out_file >> AESL_token; 
        rtl_tv_out_file >> AESL_num;  // transaction number
        if (AESL_token != "[[transaction]]") {
          cerr << "Unexpected token: " << AESL_token << endl;
          exit(1);
        }
        if (atoi(AESL_num.c_str()) == AESL_transaction_pc) {
          std::vector<sc_bv<32> > tOut_pc_buffer(1);
          int i = 0;

          rtl_tv_out_file >> AESL_token; //data
          while (AESL_token != "[[/transaction]]"){

            RTLOutputCheckAndReplacement(AESL_token, "tOut");
  
            // push token into output port buffer
            if (AESL_token != "") {
              tOut_pc_buffer[i] = AESL_token.c_str();;
              i++;
            }
  
            rtl_tv_out_file >> AESL_token; //data or [[/transaction]]
            if (AESL_token == "[[[/runtime]]]" || rtl_tv_out_file.eof())
              exit(1);
          }
          if (i > 0) {
            ((int*)__xlx_apatb_param_tOut)[0] = tOut_pc_buffer[0].to_int64();
          }
        } // end transaction
      } // end file is good
    } // end post check logic bolck
  
    AESL_transaction_pc++;
    return ;
  }
static unsigned AESL_transaction;
static AESL_FILE_HANDLER aesl_fh;
static INTER_TCL_FILE tcl_file(INTER_TCL);
std::vector<char> __xlx_sprintf_buffer(1024);
CodeState = ENTER_WRAPC;
//pIn
aesl_fh.touch(AUTOTB_TVIN_pIn);
aesl_fh.touch(AUTOTB_TVOUT_pIn);
//tIn
aesl_fh.touch(AUTOTB_TVIN_tIn);
aesl_fh.touch(AUTOTB_TVOUT_tIn);
//tOut
aesl_fh.touch(AUTOTB_TVIN_tOut);
aesl_fh.touch(AUTOTB_TVOUT_tOut);
//nx
aesl_fh.touch(AUTOTB_TVIN_nx);
aesl_fh.touch(AUTOTB_TVOUT_nx);
//ny
aesl_fh.touch(AUTOTB_TVIN_ny);
aesl_fh.touch(AUTOTB_TVOUT_ny);
//nz
aesl_fh.touch(AUTOTB_TVIN_nz);
aesl_fh.touch(AUTOTB_TVOUT_nz);
//Cap
aesl_fh.touch(AUTOTB_TVIN_Cap);
aesl_fh.touch(AUTOTB_TVOUT_Cap);
//Rx
aesl_fh.touch(AUTOTB_TVIN_Rx);
aesl_fh.touch(AUTOTB_TVOUT_Rx);
//Ry
aesl_fh.touch(AUTOTB_TVIN_Ry);
aesl_fh.touch(AUTOTB_TVOUT_Ry);
//Rz
aesl_fh.touch(AUTOTB_TVIN_Rz);
aesl_fh.touch(AUTOTB_TVOUT_Rz);
//dt
aesl_fh.touch(AUTOTB_TVIN_dt);
aesl_fh.touch(AUTOTB_TVOUT_dt);
//numiter
aesl_fh.touch(AUTOTB_TVIN_numiter);
aesl_fh.touch(AUTOTB_TVOUT_numiter);
CodeState = DUMP_INPUTS;
// print pIn Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_pIn, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)__xlx_apatb_param_pIn);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_pIn, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.pIn_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_pIn, __xlx_sprintf_buffer.data());
}
// print tIn Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_tIn, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)__xlx_apatb_param_tIn);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_tIn, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.tIn_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_tIn, __xlx_sprintf_buffer.data());
}
// print tOut Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_tOut, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)__xlx_apatb_param_tOut);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_tOut, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.tOut_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_tOut, __xlx_sprintf_buffer.data());
}
// print nx Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_nx, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_nx);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_nx, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.nx_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_nx, __xlx_sprintf_buffer.data());
}
// print ny Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_ny, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_ny);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_ny, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.ny_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_ny, __xlx_sprintf_buffer.data());
}
// print nz Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_nz, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_nz);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_nz, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.nz_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_nz, __xlx_sprintf_buffer.data());
}
// print Cap Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_Cap, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_Cap);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_Cap, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.Cap_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_Cap, __xlx_sprintf_buffer.data());
}
// print Rx Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_Rx, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_Rx);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_Rx, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.Rx_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_Rx, __xlx_sprintf_buffer.data());
}
// print Ry Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_Ry, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_Ry);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_Ry, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.Ry_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_Ry, __xlx_sprintf_buffer.data());
}
// print Rz Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_Rz, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_Rz);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_Rz, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.Rz_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_Rz, __xlx_sprintf_buffer.data());
}
// print dt Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_dt, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_dt);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_dt, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.dt_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_dt, __xlx_sprintf_buffer.data());
}
// print numiter Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVIN_numiter, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)&__xlx_apatb_param_numiter);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVIN_numiter, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.numiter_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVIN_numiter, __xlx_sprintf_buffer.data());
}
CodeState = CALL_C_DUT;
computeTempFPGA_hw_stub_wrapper(__xlx_apatb_param_pIn, __xlx_apatb_param_tIn, __xlx_apatb_param_tOut, __xlx_apatb_param_nx, __xlx_apatb_param_ny, __xlx_apatb_param_nz, __xlx_apatb_param_Cap, __xlx_apatb_param_Rx, __xlx_apatb_param_Ry, __xlx_apatb_param_Rz, __xlx_apatb_param_dt, __xlx_apatb_param_numiter);
CodeState = DUMP_OUTPUTS;
// print tIn Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVOUT_tIn, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)__xlx_apatb_param_tIn);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVOUT_tIn, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.tIn_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVOUT_tIn, __xlx_sprintf_buffer.data());
}
// print tOut Transactions
{
  sprintf(__xlx_sprintf_buffer.data(), "[[transaction]] %d\n", AESL_transaction);
  aesl_fh.write(AUTOTB_TVOUT_tOut, __xlx_sprintf_buffer.data());
  {
    sc_bv<32> __xlx_tmp_lv = *((int*)__xlx_apatb_param_tOut);

    sprintf(__xlx_sprintf_buffer.data(), "%s\n", __xlx_tmp_lv.to_string(SC_HEX).c_str());
    aesl_fh.write(AUTOTB_TVOUT_tOut, __xlx_sprintf_buffer.data()); 
  }
  tcl_file.set_num(1, &tcl_file.tOut_depth);
  sprintf(__xlx_sprintf_buffer.data(), "[[/transaction]] \n");
  aesl_fh.write(AUTOTB_TVOUT_tOut, __xlx_sprintf_buffer.data());
}
CodeState = DELETE_CHAR_BUFFERS;
AESL_transaction++;
tcl_file.set_num(AESL_transaction , &tcl_file.trans_num);
}
