cmake_minimum_required(VERSION 3.0)

project(positionbaseddynamics)

cmake_policy(SET CMP0072 NEW)

find_package(glfw3 3.3 REQUIRED)

add_subdirectory(src)
