cmake_minimum_required(VERSION 3.0)

project(positionbaseddynamics)

cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
include_directories( ${FREEGLUT_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} )

add_subdirectory(src)
