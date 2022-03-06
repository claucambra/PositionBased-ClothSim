# Position-Based Cloth Simulation (C++ and Vitis HLS)
A dual implementation of a position-based dynamics simulation involving cloth physics. Implementations in C++ (CPU-driven physics) and in Vitis HLS (synthesised FPGA-driven). 

The purpose of these was for a comparative study exploring the performance of a standard CPU-driven physics simulation (C++) versus a hardware-accelerated implementation using an FPGA (Vitis HLS). The two implementations were also compared in terms of ease of simplicity from the perspective of a newcomer to the field of domain-specific software development.

These implementations were developed as part of the final project for the 2021-2022 Domain-Specific Computing postgraduate class at Peking University.

https://user-images.githubusercontent.com/70155116/156903873-8a4391b3-d218-4eda-81b1-58cb2a6a452e.mp4

(Example above mesh 32\*32 mesh) 

## How it works

The project paper goes into detail about how both implementations work in section 4. [Read the project paper here.](https://github.com/claucambra/PositionBased-ClothSim/blob/77c91a634f43020f5227d3ceed1436dcbaefd7c7/project_paper.pdf)

## Building

First, clone the repository. Then:

### C++ version (Linux)

```
cd PositionBased-ClothSim/CPP/
mkdir build
cd build/

cmake ..
make
```

The simulation an simply be run, with an OpenGL visualisation, by running `src/pb-clothsim`.

### Vitis HLS 

```
cd PositionBased-ClothSim/Vitis_HLS/
vitis_hls -f pb-clothsim.tcl
```

### Acknowledgements
This work was inspired by [cudaCloths](https://github.com/gravitino/cudaCloths).
