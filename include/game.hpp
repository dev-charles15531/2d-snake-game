#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Window.hpp>

#include "big_food.hpp"
#include "food.hpp"
#include "header.hpp"
#include "render_engine.hpp"
#include "snake.hpp"

class Game
{
 public:
  Game();
  void run();
  void showPauseMenu();
  void showGameOverMenu();
  void resetGame();
  void showHUD();
  const static constexpr GLfloat GameSpeed{0.2f};

 private:
  sf::Window window;
  CellSize cellSize;
  ScreenSize screenSize;

  bool showPauseMenuWindow{false};
  bool showGameOverWindow{false};
  GLfloat snakeSpeed{GameSpeed};  // controls moveDelay
  int difficulty{1};              // 0=Easy, 1=Medium, 2=Hard
  GLuint highScore{0};
  GLuint score{0};
  bool isPlaying{true};
  GLuint gridSize;
  sf::Clock clock;

  GridInfo gridInfo;

  std::unique_ptr<RenderEngine> renderEngine;
  std::unique_ptr<Shader> shaderProgram;
  std::unique_ptr<Snake> snake;
  std::unique_ptr<Food> food;
  std::unique_ptr<BigFood> bigFood;
  std::unique_ptr<GUI> gui;
};
