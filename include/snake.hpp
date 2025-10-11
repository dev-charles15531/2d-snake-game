#pragma once

#include <vector>

#include "./header.hpp"
#include "./shader.hpp"

class Snake
{
 public:
  Snake(GLuint direction, Shader &);
  void move();
  void setDirection(int dir);
  std::vector<Cell> getBody() const;
  Cell getHead() const;
  int getDirection() const;

  std::vector<Cell> getSegments() const { return segments; }

 private:
  std::vector<Cell> segments;  // stores the segments of the snake
  int direction;               // 0: up, 1: right, 2: down, 3: left
  Shader &shaderProgram;
  std::vector<Cell> generateSegments();
};
