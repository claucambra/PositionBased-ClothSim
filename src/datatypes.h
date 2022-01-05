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

class Vertex {
    float position[3];
    float normal[3];
    float texCoord[2];
    float color[4];
};

class Edge {
    int triangles[2];
    int vertices[4]; // [p1, p2, p3, p4] from the PBD paper
};

class Triangle {
    int vertices[3];
};

class Mesh {
    Vertex *vertices;
    Triangle *triangles;
};

class ClothVertexData {
    int vertexID; // don't know if this is needed
    float mass;
    float invmass;
};

class ClothEdge {
    uint edgeID;
    float initialDihedralAngle;
    float initialLength;
};

class ClothTriangleData {
    uint triangleID; // don't know if this is needed
    int neighbourIDs[3];
    float mass;
};
