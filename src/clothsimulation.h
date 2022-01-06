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
    ClothSimulation(int resolution);

    void update_normals();

    std::vector<ClothVertex> vertices;

private:
    int m_resolution;
    float m_constraint_two;
    float m_constraint_diagonal;
};
