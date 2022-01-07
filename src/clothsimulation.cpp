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

#include <iostream>
#include <cstring>
#include <cmath>
#include <array>
#include "clothsimulation.h"

ClothSimulation::ClothSimulation()
{
    reset();
}

void ClothSimulation::reset()
{
    vertices.resize(0);
    indices.resize(0);

    // Create a 256*256 flat cloth mesh
    # pragma omp parallel for
    for (int row = 0; row < resolution; row++) {

        for (int column = 0; column < resolution; column++) {
            ClothVertex vertex;

            vertex.id = row + column;
            vertex.row = row;
            vertex.column = column;

            vertex.position[0] = row * 4.0f / (resolution - 1) - 2.25;      // X
            vertex.position[1] = column * 4.0f / (resolution - 1 ) - 2.25;  // Y
            vertex.position[2] = 2;                                         // Z

            vertex.color[0] = 1;                            // R
            vertex.color[1] = 0;                            // G
            vertex.color[2] = 0;                            // B
            vertex.color[3] = 1;                            // A

            vertex.velocity[0] = 0;
            vertex.velocity[1] = 0;
            vertex.velocity[2] = 0;

            vertex.normal[0] = 0;
            vertex.normal[1] = 0;
            vertex.normal[2] = 0;

            vertices.push_back(vertex);
        }
    }

    m_constraint_two = vertices[resolution].position[0]-vertices[0].position[0];
    m_constraint_diagonal = sqrt( 2 * m_constraint_two * m_constraint_two);

    update_normals();

    # pragma omp parallel for
    for (int row = 0; row < resolution - 1; row++) {
        int base = row * resolution;
        indices.push_back(base);
        for (int column = 0; column < resolution; column++) {
            indices.push_back(base + column);
            indices.push_back(base + column + resolution);
        }
        indices.push_back(base + 2 * resolution - 1);
    }

    std::cout << "VERTICES: " << vertices.size() << "\tINDICES: " << indices.size() << std::endl;
}

float ClothSimulation::inv_sqrt(float number)
{
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

    return y;
}

void ClothSimulation::update_normal(ClothVertex &vertex, int row_difference, int column_difference) const {
    // These are three points spanning a triangle
    std::array<float, 3> center = vertex.position;
    std::array<float, 3> span_vertical = vertices[(vertex.row + row_difference) * resolution + vertex.column].position;
    std::array<float, 3> span_horizontal = vertices[vertex.row * resolution + (vertex.column + column_difference)].position;

    // XYZ of both spanning vectors
    # pragma omp parallel for
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
}

void ClothSimulation::update_normals()
{
    #pragma omp parallel for
    for(auto &vertex : vertices) {
        std::memset(&vertex.normal, 0, sizeof(float) * vertex.normal.size());

        if (vertex.row > 0 && vertex.column > 0)
            update_normal(vertex, -1, -1);

        if (vertex.row > 0 && vertex.column + 1 < resolution)
            update_normal(vertex, -1, +1);

        if (vertex.row + 1 < resolution && vertex.column > 0)
            update_normal(vertex, +1, -1);

        if (vertex.row + 1 < resolution && vertex.column + 1 < resolution)
            update_normal(vertex, +1, +1);

        normalise_normal(vertex);
    }
}

void ClothSimulation::normalise_normal(ClothVertex &vertex) const
{
    const float number = (vertex.normal[0] * vertex.normal[0] +
                          vertex.normal[1] * vertex.normal[1] +
                          vertex.normal[2] * vertex.normal[2]);

    const float y = inv_sqrt(number);

    vertex.normal[0] *= y;
    vertex.normal[1] *= y;
    vertex.normal[2] *= y;
}

void ClothSimulation::update_positions()
{
    #pragma omp parallel for
    for(auto &vertex : vertices) {
        vertex.velocity[2] -= (vertex.position[0] * vertex.position[0] +
                               vertex.position[1] * vertex.position[1] +
                               vertex.position[2] * vertex.position[2] > 2) ? time_step : 0;

        vertex.position[0] += time_step * vertex.velocity[0];
        vertex.position[1] += time_step * vertex.velocity[1];
        vertex.position[2] += time_step * vertex.velocity[2];
    }
}

void ClothSimulation::relax_constraint(std::vector<ClothVertex> &buffer, int row, int column, float constraint) const
{
    // displacement vector
    std::array<float, 3> delta = {vertices[row].position[0] - vertices[column].position[0],
                                  vertices[row].position[1] - vertices[column].position[1],
                                  vertices[row].position[2] - vertices[column].position[2]};

    const float invlen = inv_sqrt(delta[0] * delta[0] +
                                  delta[1] * delta[1] +
                                  delta[2] * delta[2]);

    const float factor = (1.0f - constraint * invlen) * bias;

    buffer[row].position[0] -= delta[0] * factor;
    buffer[row].position[1] -= delta[1] * factor;
    buffer[row].position[2] -= delta[2] * factor;

    buffer[column].position[0] += delta[0] * factor;
    buffer[column].position[1] += delta[1] * factor;
    buffer[column].position[2] += delta[2] * factor;
}


void ClothSimulation::adjust_positions(ClothVertex &vertex) const
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

void ClothSimulation::validate_positions()
{
    std::vector<ClothVertex> buffer(vertices.size());

    #pragma omp parallel for
    for(int i = 0; i < iterations; i++) {
        std::memcpy(buffer.data(), vertices.data(), sizeof(ClothVertex) * vertices.size());

        #pragma omp single
        for (int row = 0; row < resolution - 1; row++) {
            for (int column = 0; column < resolution; column++) {
                relax_constraint(buffer,
                                 row * resolution + column,
                                 (row + 1) * resolution + column,
                                 m_constraint_two);
            }
        }

        # pragma omp for nowait
        for (int row = 0; row < resolution; row++) {
            for (int column = 0; column < resolution - 1; column++) {
                relax_constraint(buffer,
                                 row * resolution + column,
                                 row * resolution + (column + 1),
                                 m_constraint_two);
            }
        }

        # pragma omp for nowait
        for (int row = 0; row < resolution - 2; row++) {
            for (int column = 0; column < resolution; column++) {
                relax_constraint(buffer,
                                 row * resolution + column,
                                 (row + 2) * resolution + column,
                                 2 * m_constraint_two);
            }
        }

        # pragma omp for nowait
        for (int row = 0; row < resolution; row++) {
            for (int column = 0; column < resolution - 2; column++) {
                relax_constraint(buffer,
                                 row * resolution + column,
                                 row * resolution + (column + 2),
                                 2 * m_constraint_two);
            }
        }

        # pragma omp for nowait
        for (int row = 0; row < resolution - 1; row++) {
            for (int column = 0; column < resolution - 1; column++) {
                relax_constraint(buffer,
                                 row * resolution + column,
                                 (row + 1) * resolution + (column + 1),
                                 m_constraint_diagonal);
            }
        }

        # pragma omp for nowait
        for (int row = 1; row < resolution; row++) {
            for (int column = 0; column < resolution - 1; column++) {
                relax_constraint(buffer,
                                 row * resolution + column,
                                 (row - 1) * resolution + (column + 1),
                                 m_constraint_diagonal);
            }
        }

        # pragma omp single
        for (auto &vertex : buffer) {
            adjust_positions(vertex);
        }

        std::memcpy(vertices.data(), buffer.data(), sizeof(ClothVertex) * buffer.size());
    }
}

