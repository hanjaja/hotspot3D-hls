set moduleName computeTempFPGA
set isTopModule 1
set isCombinational 0
set isDatapathOnly 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set C_modelName {computeTempFPGA}
set C_modelType { void 0 }
set C_modelArgList {
	{ pIn int 32 regular {pointer 0}  }
	{ tIn int 32 regular {pointer 2}  }
	{ tOut int 32 regular {pointer 2}  }
	{ nx int 32 regular  }
	{ ny int 32 regular  }
	{ nz int 32 regular  }
	{ Cap float 32 regular  }
	{ Rx float 32 regular  }
	{ Ry float 32 regular  }
	{ Rz float 32 regular  }
	{ dt float 32 regular  }
	{ numiter int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "pIn", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "pIn","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} , 
 	{ "Name" : "tIn", "interface" : "wire", "bitwidth" : 32, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "tIn","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} , 
 	{ "Name" : "tOut", "interface" : "wire", "bitwidth" : 32, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "tOut","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} , 
 	{ "Name" : "nx", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "nx","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} , 
 	{ "Name" : "ny", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "ny","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} , 
 	{ "Name" : "nz", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "nz","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} , 
 	{ "Name" : "Cap", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "Cap","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} , 
 	{ "Name" : "Rx", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "Rx","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} , 
 	{ "Name" : "Ry", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "Ry","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} , 
 	{ "Name" : "Rz", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "Rz","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} , 
 	{ "Name" : "dt", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "dt","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} , 
 	{ "Name" : "numiter", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":0,"cElement": [{"cName": "numiter","cData": "int","bit_use": { "low": 0,"up": 0},"cArray": [{"low" : 0,"up" : 0,"step" : 0}]}]}]} ]}
# RTL Port declarations: 
set portNum 22
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ pIn sc_in sc_lv 32 signal 0 } 
	{ tIn_i sc_in sc_lv 32 signal 1 } 
	{ tIn_o sc_out sc_lv 32 signal 1 } 
	{ tIn_o_ap_vld sc_out sc_logic 1 outvld 1 } 
	{ tOut_i sc_in sc_lv 32 signal 2 } 
	{ tOut_o sc_out sc_lv 32 signal 2 } 
	{ tOut_o_ap_vld sc_out sc_logic 1 outvld 2 } 
	{ nx sc_in sc_lv 32 signal 3 } 
	{ ny sc_in sc_lv 32 signal 4 } 
	{ nz sc_in sc_lv 32 signal 5 } 
	{ Cap sc_in sc_lv 32 signal 6 } 
	{ Rx sc_in sc_lv 32 signal 7 } 
	{ Ry sc_in sc_lv 32 signal 8 } 
	{ Rz sc_in sc_lv 32 signal 9 } 
	{ dt sc_in sc_lv 32 signal 10 } 
	{ numiter sc_in sc_lv 32 signal 11 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "pIn", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "pIn", "role": "default" }} , 
 	{ "name": "tIn_i", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "tIn", "role": "i" }} , 
 	{ "name": "tIn_o", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "tIn", "role": "o" }} , 
 	{ "name": "tIn_o_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "tIn", "role": "o_ap_vld" }} , 
 	{ "name": "tOut_i", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "tOut", "role": "i" }} , 
 	{ "name": "tOut_o", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "tOut", "role": "o" }} , 
 	{ "name": "tOut_o_ap_vld", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "tOut", "role": "o_ap_vld" }} , 
 	{ "name": "nx", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "nx", "role": "default" }} , 
 	{ "name": "ny", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "ny", "role": "default" }} , 
 	{ "name": "nz", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "nz", "role": "default" }} , 
 	{ "name": "Cap", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "Cap", "role": "default" }} , 
 	{ "name": "Rx", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "Rx", "role": "default" }} , 
 	{ "name": "Ry", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "Ry", "role": "default" }} , 
 	{ "name": "Rz", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "Rz", "role": "default" }} , 
 	{ "name": "dt", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "dt", "role": "default" }} , 
 	{ "name": "numiter", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "numiter", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"],
		"CDFG" : "computeTempFPGA",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "90", "EstimateLatencyMax" : "9227469143",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "pIn", "Type" : "None", "Direction" : "I"},
			{"Name" : "tIn", "Type" : "OVld", "Direction" : "IO"},
			{"Name" : "tOut", "Type" : "OVld", "Direction" : "IO"},
			{"Name" : "nx", "Type" : "None", "Direction" : "I"},
			{"Name" : "ny", "Type" : "None", "Direction" : "I"},
			{"Name" : "nz", "Type" : "None", "Direction" : "I"},
			{"Name" : "Cap", "Type" : "None", "Direction" : "I"},
			{"Name" : "Rx", "Type" : "None", "Direction" : "I"},
			{"Name" : "Ry", "Type" : "None", "Direction" : "I"},
			{"Name" : "Rz", "Type" : "None", "Direction" : "I"},
			{"Name" : "dt", "Type" : "None", "Direction" : "I"},
			{"Name" : "numiter", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.fadd_32ns_32ns_32_5_full_dsp_1_U1", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.fmul_32ns_32ns_32_4_max_dsp_1_U2", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.fmul_32ns_32ns_32_4_max_dsp_1_U3", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.fdiv_32ns_32ns_32_10_no_dsp_1_U4", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.fdiv_32ns_32ns_32_10_no_dsp_1_U5", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.fptrunc_64ns_32_2_no_dsp_1_U6", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.fpext_32ns_64_2_no_dsp_1_U7", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.fpext_32ns_64_2_no_dsp_1_U8", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.dadddsub_64ns_64ns_64_5_full_dsp_1_U9", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.dmul_64ns_64ns_64_5_max_dsp_1_U10", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.dmul_64ns_64ns_64_5_max_dsp_1_U11", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_32ns_32ns_64_1_1_U12", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_32ns_64ns_96_2_1_U13", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	computeTempFPGA {
		pIn {Type I LastRead 40 FirstWrite -1}
		tIn {Type IO LastRead 45 FirstWrite 88}
		tOut {Type IO LastRead 45 FirstWrite 88}
		nx {Type I LastRead 41 FirstWrite -1}
		ny {Type I LastRead 41 FirstWrite -1}
		nz {Type I LastRead 42 FirstWrite -1}
		Cap {Type I LastRead 0 FirstWrite -1}
		Rx {Type I LastRead 10 FirstWrite -1}
		Ry {Type I LastRead 10 FirstWrite -1}
		Rz {Type I LastRead 15 FirstWrite -1}
		dt {Type I LastRead 0 FirstWrite -1}
		numiter {Type I LastRead 43 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "90", "Max" : "9227469143"}
	, {"Name" : "Interval", "Min" : "91", "Max" : "637534552"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	pIn { ap_none {  { pIn in_data 0 32 } } }
	tIn { ap_ovld {  { tIn_i in_data 0 32 }  { tIn_o out_data 1 32 }  { tIn_o_ap_vld out_vld 1 1 } } }
	tOut { ap_ovld {  { tOut_i in_data 0 32 }  { tOut_o out_data 1 32 }  { tOut_o_ap_vld out_vld 1 1 } } }
	nx { ap_none {  { nx in_data 0 32 } } }
	ny { ap_none {  { ny in_data 0 32 } } }
	nz { ap_none {  { nz in_data 0 32 } } }
	Cap { ap_none {  { Cap in_data 0 32 } } }
	Rx { ap_none {  { Rx in_data 0 32 } } }
	Ry { ap_none {  { Ry in_data 0 32 } } }
	Rz { ap_none {  { Rz in_data 0 32 } } }
	dt { ap_none {  { dt in_data 0 32 } } }
	numiter { ap_none {  { numiter in_data 0 32 } } }
}

set busDeadlockParameterList { 
}

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
