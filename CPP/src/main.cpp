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
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "clothsimulation.h"

constexpr int WINDOW_WIDTH = 1440;
constexpr int WINDOW_HEIGHT = 900;

static GLuint VAO = 0;
static GLuint VBO = 0;
static GLuint IBO = 0;

static size_t frames = 0;

ClothSimulation simulation;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotated((frames++)*0.2, 0, 0, 1);

    if (frames % 500 == 0) {
        std::cout << frames*1000.0/glutGet(GLUT_ELAPSED_TIME) << std::endl;
        std::cout << glGetError() << std::endl;
    }

    // Set up the scene with background and the sphere*/
    glColor3d(0, 0, 1);
    glutSolidSphere(0.97, 100, 100);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    simulation.update_positions();
    simulation.validate_positions();
    simulation.update_normals();

    glBufferData(GL_ARRAY_BUFFER, sizeof(ClothVertex) * simulation.vertices.size(),
                 &simulation.vertices.front(), GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * simulation.indices.size(),
                  &simulation.indices.front(), GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    // NOT GL_TRIANGLES, otherwise you'll just get a grid of triangles
    glDrawElements(GL_TRIANGLE_STRIP, simulation.indices.size(), GL_UNSIGNED_INT, 0);

    // Index 0 is for positions, index 2 is for normals, and index 3 is for color. Don't change the first args!
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), (void*)offsetof(ClothVertex, position));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), (void*)offsetof(ClothVertex, normal));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), (void*)offsetof(ClothVertex, color));
    glEnableVertexAttribArray(2); // Vertex normals
    glEnableVertexAttribArray(3); // Vertex colors*/

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);

    if (frames % 750 == 0)
        simulation.reset();

    glutSwapBuffers();
    glutPostRedisplay();
}

void init_buffers() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ClothVertex) * simulation.vertices.size(),
                 &simulation.vertices.front(), GL_DYNAMIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * simulation.indices.size(),
                  &simulation.indices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), (void*)offsetof(ClothVertex, position));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), (void*)offsetof(ClothVertex, normal));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), (void*)offsetof(ClothVertex, color));
    glEnableVertexAttribArray(0); // Vertex positions
    glEnableVertexAttribArray(2); // Vertex normals
    glEnableVertexAttribArray(3); // Vertex colors*/

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Position-Based Dynamics");
    // Calls display()
    glutDisplayFunc(display);

    glewInit();

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat mat_specular[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)WINDOW_WIDTH / (GLfloat) WINDOW_HEIGHT, 0.1, 10.0);
    glTranslatef(0.0, 0.0, -6.0);
    glRotated(300, 1, 0, 0);
    glRotated(270, 0, 0, 1);

    init_buffers();
    glutMainLoop();
}
