#pragma once
#include <SFML/Window.hpp>
#include <SFML/Window/Window.hpp>
#include <utility>

#include "./glad/glad.h"
#include "shader.hpp"
#include "snake.hpp"

using ScreenSize = std::pair<GLuint, GLuint>;

class RenderEngine
{
 public:
  RenderEngine(sf::Window& window, Snake& snake, Shader& shaderProgram, CellSize& cellSize, ScreenSize& screenSize);
  ~RenderEngine();
  void clearScreen();

  void terminate();
  void render();

  sf::Window& getWindow() const { return window; }

 private:
  sf::Window& window;
  Snake& snake;
  Shader& shaderProgram;
  CellSize& cellSize;
  std::pair<GLuint, GLuint>& screenSize;
  // OpenGL objects
  GLuint VBO, VAO, EBO;
  std::pair<GLuint, GLuint> init();
  void setupQuad();
  void pollEvents();
  void setupCoordinates();
};
