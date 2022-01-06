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
#include <array>

class Vertex {
public:
    int id;
    std::array<float, 3> position;
    std::array<float, 3> normal;
    std::array<float, 3> velocity;
    std::array<float, 4> color;
};

class ClothVertex : public Vertex {
public:
    int row;
    int column;
};

class Edge {
public:
    int triangles[2];
    int vertices[4]; // [p1, p2, p3, p4] from the PBD paper
};

class Triangle {
public:
    int vertices[3];
};

class Mesh {
public:
    Vertex *vertices;
    Triangle *triangles;
};

class ClothVertexData {
public:
    int vertexID; // don't know if this is needed
    float mass;
    float invmass;
};

class ClothEdge {
public:
    uint edgeID;
    float initialDihedralAngle;
    float initialLength;
};

class ClothTriangleData {
public:
    uint triangleID; // don't know if this is needed
    int neighbourIDs[3];
    float mass;
};
