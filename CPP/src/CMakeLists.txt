add_executable(pb-clothsim main.cpp datatypes.h clothsimulation.h clothsimulation.cpp)

add_dependencies(pb-clothsim OpenGL::OpenGL)
target_link_libraries(pb-clothsim ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${OPENMP_LIBRARIES} )

add_executable(latencytest latencytest.cpp datatypes.h clothsimulation.h clothsimulation.cpp)

install(TARGETS pb-clothsim RUNTIME DESTINATION bin)
