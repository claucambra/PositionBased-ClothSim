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
#include "clothsimulation.h"

ClothSimulation::ClothSimulation(int resolution)
{
    m_resolution = resolution;

    // Create a 256*256 flat cloth mesh
    for (int row = 0; row < m_resolution; row++) {

        for (int column = 0; column < m_resolution; column++) {
            ClothVertex vertex;

            vertex.id = row + column;
            vertex.row = row;
            vertex.column = column;

            vertex.position[0] = row * 4.0f / (m_resolution - 1) - 2;       // X
            vertex.position[1] = column * 4.0f / (m_resolution - 1 )- 2,    // Y
            vertex.position[2] = 2.0f;                                      // Z

            vertex.color[0] = 1;                            // R
            vertex.color[1] = 0;                            // G
            vertex.color[2] = 0;                            // B
            vertex.color[3] = 1;                            // A

            // Init XYZ of velocity and normal to 0
            for(int i = 0; i < 3; i++) {
                vertex.velocity[i] = 0;
                vertex.normal[i] = 0;
            }

            vertices.push_back(vertex);
        }
    }

    update_normals();
}

void ClothSimulation::update_normals()
{
    const auto update_normal = [&](ClothVertex &vertex, int row_difference, int column_difference) {
        // These are three points spanning a triangle
        std::array<float, 3> center = vertex.position;
        std::array<float, 3> span_vertical = vertices[(vertex.row + row_difference) * m_resolution + vertex.column].position;
        std::array<float, 3> span_horizontal = vertices[vertex.row * m_resolution + (vertex.column + column_difference)].position;

        // XYZ of both spanning vectors
        for(int i = 0; i < 3; i++) {
            span_vertical[i] -= center[i];
            span_horizontal[i] -= center[i];
        }

        std::array<float, 3> cross_product;
        cross_product[0] = (span_vertical[1] * span_horizontal[2]) - (span_horizontal[1] * span_vertical[2]);
        cross_product[1] = (span_vertical[2] * span_horizontal[0]) - (span_horizontal[2] * span_vertical[0]);
        cross_product[2] = (span_vertical[0] * span_horizontal[1]) - (span_horizontal[0] * span_vertical[1]);

        vertex.normal[0] += cross_product[0] * row_difference * column_difference;
        vertex.normal[1] += cross_product[1] * row_difference * column_difference;
        vertex.normal[2] += cross_product[2] * row_difference * column_difference;
    };

    auto normalise_normal = [](ClothVertex &vertex) {
        const int number = (vertex.normal[0] * vertex.normal[0]) + (vertex.normal[1] * vertex.normal[1]) + (vertex.normal[2] * vertex.normal[2]);

        // Thank John Carmack for this one
        // QUAKE 3 INVERSE SQRT ALGO
        long i;
        float x2, y;
        const float threehalfs = 1.5F;

        x2 = number * 0.5F;
        y  = number;
        i  = * ( long * ) &y;
        i  = 0x5f3759df - ( i >> 1 );
        y  = * ( float * ) &i;
        y  = y * ( threehalfs - ( x2 * y * y ) );
        // John Carmack genius is now over

        vertex.normal[0] *= y;
        vertex.normal[1] *= y;
        vertex.normal[2] *= y;
    };

    for(auto &vertex : vertices) {
        if (vertex.row > 0 && vertex.column > 0)
            update_normal(vertex, -1, -1);

        if (vertex.row > 0 && vertex.column + 1 < m_resolution)
            update_normal(vertex, -1, +1);

        if (vertex.row + 1 < m_resolution && vertex.column > 0)
            update_normal(vertex, +1, -1);

        if (vertex.row + 1 < m_resolution && vertex.column + 1 < m_resolution)
            update_normal(vertex, +1, +1);

        normalise_normal(vertex);
    }
}

