#pragma once

#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <vector>

#include "./header.hpp"
#include "./shader.hpp"
#include "big_food.hpp"

class RenderEngine;

class Snake
{
 public:
  Snake(Shader &shader, const GridInfo &gridInfo);

  // components
  const Cell &getHead() const { return segments.front(); }
  void setHead(Cell cell);
  const std::vector<Cell> getBody() const;
  const std::vector<Cell> &getSegments() const { return segments; }
  void setSegments(std::vector<Cell> segs) { segments = segs; }

  //
  void move();
  void setDirection(int dir);
  int getDirection() const;
  void setMoveDelay(GLfloat delay) { moveDelay = delay; }
  void grow() { segments.push_back(segments.back()); }
  GLuint moveAndEat(RenderEngine &renderEngine, Food &food, std::unique_ptr<BigFood> &bigFood);

  //
  void attachControl(const sf::Event::KeyPressed &keyPressed);
  void mirrorEdges();
  void draw(const GLuint &VAO) const;

  //
  bool isEating(const std::vector<Cell> &foodPosition) const;
  bool isCollided() const;

  //
  void reset();

 private:
  Shader &shaderProgram;
  const GridInfo &gridInfo;
  std::vector<Cell> segments;  // stores the segments of the snake
  int direction = 1;           // 0: down, 1: right, 2: up, 3: left
  sf::Clock clock;
  GLfloat moveDelay{0.2f};
  GLfloat timeSinceLastMove{0.0f};

  //
  std::vector<Cell> generateSegments();
};
