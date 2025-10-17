#pragma once

#include <SFML/Window/Window.hpp>

#include "big_food.hpp"
#include "food.hpp"
#include "render_engine.hpp"
#include "snake.hpp"

class Game
{
 public:
  Game();
  void run();

 private:
  sf::Window window;
  std::unique_ptr<Shader> shaderProgram;
  std::unique_ptr<Snake> snake;
  std::unique_ptr<Food> food;
  std::unique_ptr<BigFood> bigFood;
  std::unique_ptr<RenderEngine> renderEngine;
  CellSize cellSize;
  bool isPlaying{true};
};
