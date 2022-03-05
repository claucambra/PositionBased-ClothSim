# Position-Based Cloth Simulation (C++ and Vitis HLS)
A dual implementation of a position-based dynamics simulation involving cloth physics. Implementations in C++ (CPU-driven physics) and in Vitis HLS (synthesised FPGA-driven). The purpose of these was for a comparative study of the performance of a standard CPU-driven physics simulation (C++) to a hardware-accelerated implementation using an FPGA (Vitis HLS). The two implementations were also compared in terms of ease of simplicity from the perspective of a newcomer to the field of domain-specific software development.

These implementations were developed as part of the final project for the 2021-2022 Domain-Specific Computing postgraduate class at Peking University.

## Building

First, clone the repository. Then:

### C++ version (Linux)

```
cd CPP/
cmake
cd build/
make
```

The simulation an simply be run, with an OpenGL visualisation, by running `src/pb-clothsim`

### Vitis HLS 

```
cd Vitis_HLS/
vitis_hls -f pb-clothsim.tcl
```

### Acknowledgements
This work was inspired by [cudaCloths](https://github.com/gravitino/cudaCloths).
