cmake_minimum_required(VERSION 3.0)

project(positionbaseddynamics)

add_executable(positionbaseddynamics main.cpp)

install(TARGETS positionbaseddynamics RUNTIME DESTINATION bin)
