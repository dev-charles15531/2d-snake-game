#pragma once

#include <utility>

#include "glad/glad.h"

// Define a simple structure to represent a cell in the grid
struct Cell
{
  int x;
  int y;
};

// Define a structure to represent the size of each cell
struct CellSize
{
  float width;
  float height;
};

// Define a type alias for screen size
using ScreenSize = std::pair<GLuint, GLuint>;
