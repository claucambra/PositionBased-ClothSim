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

#include <cmath>
#include "process_cloth.h"
#include "clothkernel.h"

int main() {

	const int resolution = 36;
	const int num_vertices = resolution * resolution;
	const float time_step = 0.025;
	const int iterations = 128;
	const float bias = 0.17;

	ClothVertex vertices_in[2048];
	ClothVertex vertices_out[2048];

	for (int row = 0; row < resolution; row++) {
	    for (int column = 0; column < resolution; column++) {
			ClothVertex vertex;

			vertex.position[0] = row * 4.0f / (resolution - 1) - 2;      // X
			vertex.position[1] = column * 4.0f / (resolution - 1 ) - 2;  // Y
			vertex.position[2] = 2;                                      // Z

			vertex.velocity[0] = 0;
			vertex.velocity[1] = 0;
			vertex.velocity[2] = 0;

			vertices_in[row * resolution + column] = vertex;
	    }
	}

	float constraint_two = vertices_in[resolution].position[0] - vertices_in[0].position[0];
	float constraint_dia = sqrt(2 * constraint_two * constraint_two);

	for (int i =0; i < 1000; i++) {
		process_cloth(vertices_in, vertices_out, resolution, time_step, iterations, bias, constraint_two, constraint_dia);

		memset(&vertices_in[0], 0, sizeof(vertices_in));
		memcpy(&vertices_in[0], &vertices_out[0], sizeof(ClothVertex) * resolution);
		memset(&vertices_out[0], 0, sizeof(vertices_out));

	}
}
