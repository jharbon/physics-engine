cmake_minimum_required(VERSION 3.16)
project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(include)

add_executable(physics_engine
    src/main.cpp
    src/glad.c
)

target_link_libraries(physics_engine
    OpenGL::GL
    glfw
)