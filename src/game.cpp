#include "../include/game.hpp"

#include <SFML/Window/Event.hpp>
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
  unsigned int cellUnit{std::min(screenWidth / GRID_WIDTH, screenHeight / GRID_HEIGHT)};
  cellSize = {static_cast<float>(cellUnit), static_cast<float>(cellUnit)};

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

  // Attach event listener for controls
  renderEngine->addEventListener(
      [this](const sf::Event& event)
      {
        if (event.is<sf::Event::KeyPressed>())
        {
          if (isPlaying)
          {
            // attach control to snake
            const auto* keyPressed{event.getIf<sf::Event::KeyPressed>()};
            snake->attachControl(*keyPressed);
          }

          // toggle play/pause on space key
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
          {
            isPlaying = !isPlaying;
            std::cout << (isPlaying ? "▶️  Playing\n" : "⏸️  Paused\n");
          }
        }
      });
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
    if (isPlaying) snake->move();

    renderEngine->render();
  }
}
