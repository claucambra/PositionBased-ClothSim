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

#pragma once

#include <iostream>
#include <vector>
#include "datatypes.h"

class ClothSimulation
{
public:
    // Resolution affects the number of vertices in the cloth mesh
    ClothSimulation();

    void reset();

    static float inv_sqrt(float number);
    void update_normal(ClothVertex &vertex, int row_difference, int column_difference) const;
    void normalise_normal(ClothVertex &vertex) const;
    void adjust_positions(ClothVertex &vertex) const;
    void relax_constraint(std::vector<ClothVertex> &buffer, int row, int column, float constraint) const;

    void update_normals();
    void update_positions();
    void validate_positions();

    std::vector<ClothVertex> vertices;
    std::vector<int> indices;

    static constexpr int resolution = 32;
    static constexpr float time_step = 0.025;
    static constexpr int iterations = 128;
    static constexpr float bias = 0.17;

private:
    float m_constraint_two;
    float m_constraint_diagonal;
};
