#pragma once

#include <SFML/Window/Window.hpp>

#include "render_engine.hpp"
#include "snake.hpp"

class Game
{
 public:
  Game();
  ~Game();
  void run();

 private:
  sf::Window window;
  Shader* shaderProgram{nullptr};
  RenderEngine* renderEngine{nullptr};
  Snake* snake{nullptr};
  CellSize cellSize;
  bool isPlaying{true};
};
