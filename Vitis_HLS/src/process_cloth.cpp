/*
 * Copyright (C) 2022 Claudio Cambra
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "process_cloth.h"

// Top function
void process_cloth(ClothVertex vertices_in[],
		   ClothVertex vertices_out[],
		   int resolution,
		   float time_step,
		   int iterations,
		   float bias,
		   float constraint_two,
		   float constraint_dia)
{
	// Vertex buffers we are going to be working on
	ClothVertex mid_buffer[1024]; // Transfer out from update_positions to validate_positions
	ClothVertex out_buffer[1024]; // These sizes are meant for a 32*32 mesh, increase for larger meshes

	// The Position-Based Dynamics algorithm is implemented here (M. Müller et al. / Position Based Dynamics)
	// Since we receive data from outside, we can skip the initialisation phase and go straight to (5)

	// (16) Update velocities
	// (5) Manipulate velocities. Hook up external forces (e.g. gravity)
	// if these forces cannot be converted to positional constraints
	// (6) Dampen velocities (if necessary)
	// (7) Find the projected point for all vertices
	// (8) Generate the collision constraints for all vertices
	ClothKernel::update_positions(vertices_in, mid_buffer, resolution, time_step);

	// (9-11) Iteratively manipulate position estimates so that they satisfy the constraints
	// (12-15) Move positions of vertices to optimised estimates and update velocities accordingly (iterative)
	ClothKernel::validate_positions(mid_buffer, out_buffer, constraint_dia, constraint_two, resolution, iterations, bias);

	memcpy(&vertices_out[0], &out_buffer[0], sizeof(ClothVertex) * (resolution * resolution));
}
