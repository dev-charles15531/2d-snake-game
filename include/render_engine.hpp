#pragma once
#include <SFML/Window.hpp>
#include <SFML/Window/Window.hpp>
#include <functional>
#include <utility>
#include <vector>

#include "./glad/glad.h"
#include "header.hpp"
#include "shader.hpp"
#include "snake.hpp"

class RenderEngine
{
 public:
  using EventCallback = std::function<void(const sf::Event&)>;  // Event listener callback type

  RenderEngine(sf::Window& window, Snake& snake, Shader& shaderProgram, CellSize& cellSize, ScreenSize& screenSize);
  ~RenderEngine();
  void clearScreen();
  void render();
  void terminate();

  sf::Window& getWindow() const { return window; }
  void addEventListener(const EventCallback& event);

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

  // Event callbacks
  std::vector<EventCallback> listeners;
  void dispatchEvent(const sf::Event& event);
};
