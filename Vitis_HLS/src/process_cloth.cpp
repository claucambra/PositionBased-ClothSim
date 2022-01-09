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
	const int num_vertices = resolution * resolution;

	// Vertex buffers we are going to be working on
	ClothVertex in_buffer[2048];
	ClothVertex mid_buffer[2048]; // Transfer out from update_positions to validate_positions
	ClothVertex out_buffer[2048];

	memcpy(&in_buffer[0], &vertices_in[0], sizeof(ClothVertex) * resolution);

	// The Position-Based Dynamics algorithm is implemented here (M. Müller et al. / Position Based Dynamics)
	// Since we receive data from outside, we can skip the initialisation phase and go straight to (5)

	// (5) Manipulate velocities. Hook up external forces (e.g. gravity)
	// if these forces cannot be converted to positional constraints
	// (6) Dampen velocities (if necessary)
	// (7) Find the projected point for all vertices
	// (8) Generate the collision constraints for all vertices
	// (16) Update velocities
	ClothKernel::update_positions(in_buffer, mid_buffer, resolution, time_step);

	// (9-11) Iteratively manipulate position estimates so that they satisfy the constraints
	// (12-15) Move positions of vertices to optimised estimates and update velocities accordingly (iterative)
	ClothKernel::validate_positions(mid_buffer, out_buffer, constraint_dia, constraint_two, resolution, iterations, bias);

	memcpy(&vertices_out[0], &out_buffer[0], sizeof(ClothVertex) * resolution);
}