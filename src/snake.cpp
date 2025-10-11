#include "../include/snake.hpp"

#include "../include/glad/glad.h"

Snake::Snake(GLuint direction, Shader& shaderProgram)
    : direction(direction), shaderProgram(shaderProgram), segments(generateSegments())
{
}

/**
 * Generates the initial segments of the snake.
 * The snake starts with 3 segments positioned horizontally.
 * @return A vector of Cell representing the initial segments of the snake.
 */
std::vector<Cell> Snake::generateSegments()
{
  std::vector<Cell> newSegments;
  for (int i{0}; i < 3; ++i)
  {
    newSegments.push_back({5 + i, 10});
  }

  return newSegments;
}

void Snake::move()
{
  for (size_t i{segments.size() - 1}; i > 0; --i)
  {
    segments[i] = segments[i - 1];
  }

  // Move the head
  switch (direction)
  {
    case 0:
      segments[0].y += 1;
      break;  // Up
    case 1:
      segments[0].x += 1;
      break;  // Right
    case 2:
      segments[0].y -= 1;
      break;  // Down
    case 3:
      segments[0].x -= 1;
      break;  // Left
    default:
      break;
  }
}

void Snake::setDirection(int dir)
{
  // Prevent the snake from reversing
  if ((direction == 0 && dir != 2) || (direction == 1 && dir != 3) || (direction == 2 && dir != 0) ||
      (direction == 3 && dir != 1))
  {
    direction = dir;
  }
}
