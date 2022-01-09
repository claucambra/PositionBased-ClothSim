add_executable(positionbaseddynamics main.cpp datatypes.h clothsimulation.h clothsimulation.cpp)

add_dependencies(positionbaseddynamics OpenGL::OpenGL)
target_link_libraries(positionbaseddynamics ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${OPENMP_LIBRARIES} )

install(TARGETS positionbaseddynamics RUNTIME DESTINATION bin)
