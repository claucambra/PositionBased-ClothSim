#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>


#include "clothsimulation.h"

constexpr int RESOLUTION = 256;

constexpr int window_width = 1024;
constexpr int window_height = 768;

// vbo
static GLuint ibo = 0;
static GLuint vbo = 0;
static GLuint cbo = 0;
static GLuint nbo = 0;

ClothSimulation simulation(RESOLUTION);

void init_buffers() {
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &cbo);
    glGenBuffers(1, &nbo);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * simulation.vertices.size(),
                 &simulation.vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {

}

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(window_width, window_width);
    glutCreateWindow("Position-Based Dynamics");
    glutDisplayFunc(display);

    glewInit();

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

    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)window_width / (GLfloat) window_height, 0.1, 10.0);
    glTranslatef(0.0, 0.0, -6.0);
    glRotated(300, 1, 0, 0);
    glRotated(270, 0, 0, 1);

    init_buffers();
    glutMainLoop();
}
