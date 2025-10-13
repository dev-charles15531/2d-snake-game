#include "../include/game.hpp"

#include <SFML/Window/WindowEnums.hpp>
#include <iostream>

#include "../include/glad/glad.h"

Game::Game()
{
  // Setup window and OpenGL context
  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.majorVersion = 4;
  settings.minorVersion = 4;
  settings.attributeFlags = sf::ContextSettings::Core;

  sf::VideoMode desktopMode{sf::VideoMode::getDesktopMode()};
  unsigned int screenWidth{desktopMode.size.x / 2};
  unsigned int screenHeight{desktopMode.size.y / 2};
  std::pair<GLuint, GLuint> screenSize{screenWidth, screenHeight};

  desktopMode.size.x = screenWidth;
  desktopMode.size.y = screenHeight;

  const int GRID_WIDTH{80};
  const int GRID_HEIGHT{80};
  float cellUnit = std::min(screenWidth / GRID_WIDTH, screenHeight / GRID_HEIGHT);
  cellSize = {cellUnit, cellUnit};

  window.create(desktopMode, "SNAKE GAME", sf::Style::Default, sf::State::Windowed, settings);
  window.setFramerateLimit(60);

  if (!gladLoadGL())
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    exit(1);
  }

  shaderProgram = new Shader("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");
  snake = new Snake(0, *shaderProgram);
  renderEngine = new RenderEngine(window, *snake, *shaderProgram, cellSize, screenSize);
}

Game::~Game()
{
  delete shaderProgram;
  delete snake;
  delete renderEngine;
}

void Game::run()
{
  while (window.isOpen())
  {
    attachControls();

    if (isPlaying) snake->move();

    renderEngine->render();
  }
}

void Game::attachControls()
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
  {
    // direction = 1;  // right
    snake->setDirection(1);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left))
  {
    // direction = 3;  // left
    snake->setDirection(3);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up))
  {
    // direction = 0;  // up
    snake->setDirection(0);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down))
  {
    // direction = 2;  // down
    snake->setDirection(2);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
  {
    isPlaying = !isPlaying;
  }
}
