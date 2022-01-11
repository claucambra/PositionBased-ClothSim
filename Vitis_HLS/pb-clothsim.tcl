############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
############################################################
open_project PositionBased-ClothSim
set_top process_cloth
add_files src/process_cloth.h
add_files src/process_cloth.cpp
add_files src/clothkernel.h
add_files src/clothkernel.cpp
add_files -tb src/testbench.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1" -flow_target vivado
set_part {xcvu5p-flva2104-1-e}
create_clock -period 10 -name default
set_directive_top -name process_cloth "process_cloth"
csim_design -ldflags "-L /usr/lib64"
csynth_design
cosim_design -disable_dependency_check
export_design -format ip_catalog
