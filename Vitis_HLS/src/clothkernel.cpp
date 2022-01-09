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

#include <array>
#include <cstring>
#include <cmath>
#include "clothkernel.h"

float ClothKernel::inv_sqrt(float number)
{
    return 1/sqrt(number);
}

void ClothKernel::update_positions(ClothVertex vertices_in[], ClothVertex vertices_out[], int resolution, float time_step)
{
    for(int i = 0; i < resolution * resolution; i++) {
#pragma HLS UNROLL
    	ClothVertex vertex = vertices_in[i];
        vertex.velocity[2] -= (vertex.position[0] * vertex.position[0] +
                               vertex.position[1] * vertex.position[1] +
                               vertex.position[2] * vertex.position[2] > 2) ? time_step : 0;

        vertex.position[0] += time_step * vertex.velocity[0];
        vertex.position[1] += time_step * vertex.velocity[1];
        vertex.position[2] += time_step * vertex.velocity[2];
        vertices_out[i] = vertex;
    }
}

void ClothKernel::relax_constraint(ClothVertex &vertex_a,
		 	 	 	 	 	 	   ClothVertex &vertex_b,
								   float constraint,
								   float bias)
{
    // displacement vector
    std::array<float, 3> delta = {vertex_a.position[0] - vertex_b.position[0],
    							  vertex_a.position[1] - vertex_b.position[1],
								  vertex_a.position[2] - vertex_b.position[2]};

    const float invlen = inv_sqrt(delta[0] * delta[0] +
                                  delta[1] * delta[1] +
                                  delta[2] * delta[2]);

    const float factor = (1.0f - constraint * invlen) * bias;

    vertex_a.position[0] -= delta[0] * factor;
    vertex_a.position[1] -= delta[1] * factor;
    vertex_a.position[2] -= delta[2] * factor;

    vertex_b.position[0] += delta[0] * factor;
    vertex_b.position[1] += delta[1] * factor;
    vertex_b.position[2] += delta[2] * factor;
}


void ClothKernel::adjust_position(ClothVertex &vertex)
{
    const float number = (vertex.position[0] * vertex.position[0] +
                          vertex.position[1] * vertex.position[1] +
                          vertex.position[2] * vertex.position[2]);

    const float invrho = inv_sqrt(number);

    // move vertex to surface of sphere if inside
    vertex.position[0] *= invrho < 1 ? 1 : invrho;
    vertex.position[1] *= invrho < 1 ? 1 : invrho;
    vertex.position[2] *= invrho < 1 ? 1 : invrho;
}

void ClothKernel::validate_positions(ClothVertex vertices_in[],
									 ClothVertex vertices_out[],
									 float constraint_dia,
									 float constraint_two,
									 int resolution,
									 int iterations,
									 float bias)
{
	ClothVertex buffer[2048];

    for(int i = 0; i < iterations; i++) {
        std::memcpy(&buffer[0], &vertices_in[0], sizeof(ClothVertex) * (resolution * resolution));

#pragma HLS PIPELINE
        for (int row = 0; row < resolution - 1; row++) {
            for (int column = 0; column < resolution; column++) {
                relax_constraint(buffer[row * resolution + column],
                                 buffer[(row + 1) * resolution + column],
                                 constraint_two,
								 bias);
            }
        }

#pragma HLS PIPELINE
        for (int row = 0; row < resolution; row++) {
            for (int column = 0; column < resolution - 1; column++) {
                relax_constraint(buffer[row * resolution + column],
							     buffer[row * resolution + (column + 1)],
                                 constraint_two,
								 bias);
            }
        }

#pragma HLS PIPELINE
        for (int row = 0; row < resolution - 2; row++) {
            for (int column = 0; column < resolution; column++) {
                relax_constraint(buffer[row * resolution + column],
							     buffer[(row + 2) * resolution + column],
                                 2 * constraint_two,
								 bias);
            }
        }

#pragma HLS PIPELINE
        for (int row = 0; row < resolution; row++) {
            for (int column = 0; column < resolution - 2; column++) {
                relax_constraint(buffer[row * resolution + column],
							     buffer[row * resolution + (column + 2)],
                                 2 * constraint_two,
								 bias);
            }
        }

#pragma HLS PIPELINE
        for (int row = 0; row < resolution - 1; row++) {
            for (int column = 0; column < resolution - 1; column++) {
                relax_constraint(buffer[row * resolution + column],
							     buffer[(row + 1) * resolution + (column + 1)],
                                 constraint_dia,
								 bias);
            }
        }

#pragma HLS PIPELINE
        for (int row = 1; row < resolution; row++) {
            for (int column = 0; column < resolution - 1; column++) {
                relax_constraint(buffer[row * resolution + column],
							     buffer[(row - 1) * resolution + (column + 1)],
                                 constraint_dia,
								 bias);
            }
        }

        for (int j = 0; j < resolution * resolution; j++) {
#pragma HLS UNROLL
            adjust_position(buffer[i]);
        }
    }

    std::memcpy(&vertices_out[0], &buffer[0], sizeof(ClothVertex) * (resolution * resolution));
}
