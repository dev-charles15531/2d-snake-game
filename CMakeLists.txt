cmake_minimum_required(VERSION 3.17)
project(snake-game-2d LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories for glad and ImGui
include_directories(include)

# ──────────────────────────────────────────────
# ImGui sources
# ──────────────────────────────────────────────
set(IMGUI_SOURCES
    include/imgui/imgui.cpp
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imgui_impl_opengl3.cpp
    include/imgui/imgui_impl_sfml.cpp
)

# ──────────────────────────────────────────────
# Game sources
# ──────────────────────────────────────────────
set(SOURCES
    src/main.cpp
    src/glad.c
    src/shader.cpp
    src/snake.cpp
    src/render_engine.cpp
    src/game.cpp
    src/food.cpp
    src/big_food.cpp
    src/gui.cpp
    ${IMGUI_SOURCES}
)

# ──────────────────────────────────────────────
# GLAD static library
# ──────────────────────────────────────────────
add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)

# ──────────────────────────────────────────────
# Find SFML 3
# ──────────────────────────────────────────────
find_package(SFML 3 REQUIRED COMPONENTS System Window)

# ──────────────────────────────────────────────
# Executable
# ──────────────────────────────────────────────
add_executable(my_game ${SOURCES})

# Link against SFML 3 imported targets
target_link_libraries(my_game
    PRIVATE
        glad
        SFML::System
        SFML::Window
        GL  # native OpenGL
)

