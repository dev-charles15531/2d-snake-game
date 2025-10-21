#include "../include/game.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <iostream>
#include <memory>
#include <utility>

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
  screenSize = {screenWidth, screenHeight};

  desktopMode.size.x = screenWidth;
  desktopMode.size.y = screenHeight;

  GRID_SIZE = 80;  // A square matrix for grid

  window.create(desktopMode, "SNAKE GAME", sf::Style::Default, sf::State::Windowed, settings);
  window.setFramerateLimit(60);

  if (!gladLoadGL())
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    exit(1);
  }

  shaderProgram = std::make_unique<Shader>("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");
  snake = std::make_unique<Snake>(*shaderProgram, GridInfo(GRID_SIZE, screenSize));
  food = std::make_unique<Food>(*shaderProgram, GridInfo(GRID_SIZE, screenSize), screenSize);
  renderEngine = std::make_unique<RenderEngine>(window, *snake, *shaderProgram, *food, screenSize, GRID_SIZE);

  // Attach event listener for controls
  renderEngine->addEventListener(
      [this](const sf::Event& event)
      {
        if (event.is<sf::Event::KeyPressed>())
        {
          const auto* keyPressed{event.getIf<sf::Event::KeyPressed>()};
          if (isPlaying)
          {
            // attach control to snake
            snake->attachControl(*keyPressed);
          }

          // toggle play/pause on space key
          if (keyPressed->scancode == sf::Keyboard::Scan::Space)
          {
            isPlaying = !isPlaying;
            std::cout << (isPlaying ? "▶️  Playing\n" : "⏸️  Paused\n");
          }
        }
      });
}

///// RUN THE GAME /////
void Game::run()
{
  while (window.isOpen())
  {
    // start moving the snake
    if (isPlaying)
    {
      if (snake->moveAndEat(*renderEngine, *food, bigFood) == 1)
      {
        std::cout << "Game Over!\n";
        // Handle game over logic:
        // stop game loop, reset, etc.
        // return;
        isPlaying = false;
      }
    }

    renderEngine->render();
  }
}
