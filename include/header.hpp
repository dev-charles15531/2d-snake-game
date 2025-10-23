#pragma once

#include <cmath>
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

const float scaleFactor{1.2f};  // adjust to taste

struct GridInfo
{
  GLuint baseSize;         // logical grid size
  ScreenSize& screenSize;  // window dimensions (width, height)

  GridInfo(GLuint gridSize, ScreenSize& screen) : baseSize(gridSize), screenSize(screen) {}

  // Computes actual X and Y grid limits depending on aspect ratio
  std::pair<GLuint, GLuint> getGridSizeI() const
  {
    int xMax, yMax;
    GLfloat aspectRatio{getAspectRatio()};

    if (aspectRatio >= 1.0f)
    {
      xMax = static_cast<int>(std::round(baseSize * aspectRatio));
      yMax = static_cast<int>(baseSize);
    }
    else
    {
      xMax = static_cast<int>(baseSize);
      yMax = static_cast<int>(std::round(baseSize / aspectRatio));
    }

    return {xMax, yMax};
  }

  std::pair<GLfloat, GLfloat> getGridSizeF() const
  {
    GLfloat xMax, yMax;
    GLfloat aspectRatio{getAspectRatio()};

    if (aspectRatio >= 1.0f)
    {
      xMax = static_cast<int>(std::round(baseSize * aspectRatio));
      yMax = static_cast<int>(baseSize);
    }
    else
    {
      xMax = static_cast<int>(baseSize);
      yMax = static_cast<int>(std::round(baseSize / aspectRatio));
    }

    return {xMax, yMax};
  }

  float getAspectRatio() const
  {
    return static_cast<float>(screenSize.first) / static_cast<float>(screenSize.second == 0 ? 1.0f : screenSize.second);
  }

  std::pair<GLuint, GLuint> getPixelPosition(GLuint gx, GLuint gy) const
  {
    auto [xMax, yMax] = getGridSizeI();

    float cellWidth = static_cast<float>(screenSize.first) / xMax;
    float cellHeight = static_cast<float>(screenSize.second) / yMax;

    float px = gx * cellWidth;
    float py = gy * cellHeight;

    return std::make_pair(px, py);
  }

  // Helper for window resizing
  void updateScreenSize(const std::pair<int, int>& newSize) { screenSize = newSize; }
  void updategridSize(GLuint newSize) { baseSize = newSize; }
};
