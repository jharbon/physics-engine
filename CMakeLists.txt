cmake_minimum_required(VERSION 3.16)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

include_directories(
    include
    tests
)

add_executable(physics_engine
    src/core/main.cpp
    src/glad/glad.c
    src/rendering/Shader.cpp
    src/rendering/Renderer.cpp
    src/math/utils.cpp
    src/particles/utils.cpp
    src/physics/SimulationPhysics.cpp
    src/input/SimulationControl.cpp
    src/math/Vec2.cpp
    src/particles/Particle.cpp
    src/collision/collision.cpp
)

add_executable(test_vec2
    tests/test_vec2.cpp
    src/math/Vec2.cpp
)

add_executable(test_collision
    tests/test_collision.cpp
    src/collision/collision.cpp
    src/math/Vec2.cpp
    src/particles/Particle.cpp
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
    spdlog::spdlog
    fmt::fmt
)