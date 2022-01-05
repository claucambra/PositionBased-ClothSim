add_executable(positionbaseddynamics main.cpp glad.c datatypes.h)

target_include_directories(positionbaseddynamics PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(positionbaseddynamics glfw)

install(TARGETS positionbaseddynamics RUNTIME DESTINATION bin)
