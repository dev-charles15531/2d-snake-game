#include <SFML/Window.hpp>
#include <iostream>

#include "../include/glad/glad.h"
#include "../include/header.hpp"
#include "../include/render_engine.hpp"
#include "../include/snake.hpp"

int main()
{
  // Request a core OpenGL context
  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.majorVersion = 4;
  settings.minorVersion = 4;
  settings.attributeFlags = sf::ContextSettings::Core;

  // Get the desktop resolution
  sf::VideoMode desktopMode{sf::VideoMode::getDesktopMode()};
  unsigned int screenWidth{desktopMode.size.x / 2};
  unsigned int screenHeight{desktopMode.size.y / 2};

  // set new size
  desktopMode.size.x = screenWidth;
  desktopMode.size.y = screenHeight;

  const int GRID_WIDTH{80};
  const int GRID_HEIGHT{80};
  float cellUnit = std::min(screenWidth / GRID_WIDTH, screenHeight / GRID_HEIGHT);
  CellSize cellSize{cellUnit, cellUnit};

  sf::Window window(desktopMode, "SNAKE GAME", sf::Style::Default, sf::State::Windowed, settings);
  window.setFramerateLimit(60);

  // Initialize GLAD
  if (!gladLoadGL())
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  // load and compile shaders here
  Shader shaderProgram("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");

  Snake snake(0, shaderProgram);

  RenderEngine renderEngine(snake, shaderProgram, screenWidth, screenHeight);

  sf::Clock clock;
  float moveDelay{0.2f};  // seconds between each snake move
  float timeSinceLastMove{0.0f};

  while (window.isOpen())
  {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
    {
      // direction = 1;  // right
      snake.setDirection(1);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left))
    {
      // direction = 3;  // left
      snake.setDirection(3);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up))
    {
      // direction = 0;  // up
      snake.setDirection(0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down))
    {
      // direction = 2;  // down
      snake.setDirection(2);
    }

    float deltaTime{clock.restart().asSeconds()};
    timeSinceLastMove += deltaTime;

    // Move the snake only if enough time has passed
    if (timeSinceLastMove >= moveDelay)
    {
      snake.move();
      timeSinceLastMove = 0.0f;
    }

    renderEngine.render(window, cellSize.width, cellSize.height);

    // glBindVertexArray(VAO);
    glBindVertexArray(0);

    window.display();
  }

  renderEngine.terminate();
  window.close();

  return 0;
}
