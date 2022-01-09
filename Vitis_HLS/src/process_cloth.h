#ifndef SRC_PBD_H_
#define SRC_PBD_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clothkernel.h"

void process_cloth(ClothVertex vertices_in[],
				   ClothVertex vertices_out[],
				   int resolution,
				   float time_step,
				   int iterations,
				   float bias,
				   float constraint_two,
				   float constraint_diagonal);

#endif
