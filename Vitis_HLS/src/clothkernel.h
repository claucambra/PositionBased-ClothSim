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

#ifndef SRC_CLOTH_KERNEL_H_
#define SRC_CLOTH_KERNEL_H_

struct ClothVertex {
    float position[3];
    float velocity[3];
};

class ClothKernel {
public:
	static float inv_sqrt(float number);

	static void adjust_position(ClothVertex &vertex);

	static void relax_constraint(ClothVertex &vertex_a,
 	 	 	   	   	   	   	     ClothVertex &vertex_b,
								 float constraint,
								 float bias);

	static void update_positions(ClothVertex vertices_in[],
								 ClothVertex vertices_out[],
								 int resolution,
								 float time_step);

	static void validate_positions(ClothVertex vertices_in[],
								   ClothVertex vertices_out[],
								   float constraint_dia,
								   float constraint_two,
								   int resolution,
								   int iterations,
								   float bias);
};

#endif
