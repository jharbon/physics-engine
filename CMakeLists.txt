cmake_minimum_required(VERSION 3.16)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(fmt REQUIRED)

include_directories(
    include
    tests
)

add_executable(physics_engine
    src/main.cpp
    src/glad.c
    src/Shader.cpp
    src/Renderer.cpp
    src/SimulationPhysics.cpp
    src/SimulationControl.cpp
    src/Vec2.cpp
    src/Particle.cpp
    src/collision.cpp
)

add_executable(test_vec2
    tests/test_vec2.cpp
    src/Vec2.cpp
)

add_executable(test_collision
    tests/test_collision.cpp
    src/collision.cpp
    src/Vec2.cpp
    src/Particle.cpp
)

enable_testing()

add_test(NAME Vec2Tests
    COMMAND test_vec2
)

add_test(NAME CollisionTests
    COMMAND test_collision
)

target_link_libraries(physics_engine
    OpenGL::GL
    glfw
    fmt::fmt
)