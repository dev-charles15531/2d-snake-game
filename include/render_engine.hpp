#pragma once

#include <SFML/Window.hpp>
#include <SFML/Window/Window.hpp>

#include "./glad/glad.h"
#include "shader.hpp"
#include "snake.hpp"

class RenderEngine
{
 public:
  RenderEngine(Snake& snake, Shader& shaderProgram, int screenWidth, int screenHeight);
  void clearScreen();

  void terminate();
  void render(sf::Window& window, float cellWidth, float cellHeight);

 private:
  float screenWidth;
  float screenHeight;
  Snake& snake;
  Shader& shaderProgram;
  GLuint VBO, VAO, EBO;
  void setupQuad();
  void pollEvents(sf::Window& window);
  void setupCoordinates();
};
