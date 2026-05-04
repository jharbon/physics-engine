cmake_minimum_required(VERSION 3.16)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(include)

add_executable(physics_engine
    src/main.cpp
    src/glad.c
    src/Vec2.cpp
    src/Particle.cpp
)

add_executable(test_vec2
    tests/test_vec2.cpp
    src/Vec2.cpp
)

enable_testing()

add_test(NAME Vec2Tests
    COMMAND test_vec2
)

target_link_libraries(physics_engine
    OpenGL::GL
    glfw
)