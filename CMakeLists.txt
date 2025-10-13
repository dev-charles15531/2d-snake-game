cmake_minimum_required(VERSION 3.17)
project(snake-game-2d LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Tell CMake to use static SFML libraries
set(SFML_STATIC_LIBRARIES TRUE)

# Include directories (for glad and KHR)
include_directories(include)

# Add source files
set(SOURCES
    src/main.cpp
    src/glad.c
    src/shader.cpp
    src/snake.cpp
    src/render_engine.cpp
    src/game.cpp
)

# Add glad as a static library from source
add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)

# Find SFML 3 (case-sensitive components!)
find_package(SFML 3 REQUIRED COMPONENTS Window System)

add_executable(main ${SOURCES})

# Link SFML + OpenGL
target_link_libraries(main
    glad
    SFML::Window
    SFML::System
    GL  # For OpenGL on Linux
)

