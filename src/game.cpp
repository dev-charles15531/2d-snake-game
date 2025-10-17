#include "../include/game.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <iostream>
#include <memory>

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

  shaderProgram = std::make_unique<Shader>("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");
  snake = std::make_unique<Snake>(*shaderProgram, GRID_WIDTH);
  food = std::make_unique<Food>(*shaderProgram, GRID_WIDTH);
  renderEngine = std::make_unique<RenderEngine>(window, *snake, *shaderProgram, *food, cellSize,
                                                screenSize);  // Attach event listener for controls

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

void Game::run()
{
  while (window.isOpen())
  {
    if (isPlaying) snake->move();

    // Check if the snake has eaten the food
    if (snake->isEating(food->getPosition()))
    {
      snake->grow();
      food->respawn();

      // spawn big food after eating every x food
      // TODO: never span big food in position of normal food
      if (food->getRespawnCounter() % 2 == 0 && food->getRespawnCounter())
      {
        std::cout << "Big Food Spawned!\n";
        bigFood = std::make_unique<BigFood>(*shaderProgram, food->getCellUnitSize());
        renderEngine->setBigFood(*bigFood);
        bigFood->isActive = true;
      }
    }

    // check if snake has eaten big food
    else if (bigFood && bigFood->isActive && snake->isEating(bigFood->getPosition()))
    {
      snake->grow();
      bigFood->isActive = false;
    }

    renderEngine->render();
  }
}
