#pragma once

#include <SFML/Window.hpp>
#include <SFML/Window/Window.hpp>
#include <functional>
#include <utility>
#include <vector>

#include "./glad/glad.h"
#include "big_food.hpp"
#include "food.hpp"
#include "header.hpp"
#include "shader.hpp"
#include "snake.hpp"

class RenderEngine
{
 public:
  using EventCallback = std::function<void(const sf::Event&)>;  // Event listener callback type

  RenderEngine(sf::Window& window, Snake& snake, Shader& shaderProgram, Food& food, ScreenSize& screenSize,
               GLuint gridSize);
  ~RenderEngine();
  void clearScreen() const;
  void terminate() const;
  void render();
  void addEventListener(const EventCallback& event);

  const GLuint& getVAO() const { return VAO; }
  sf::Window& getWindow() const { return window; }
  const std::pair<GLuint, GLuint>& getScreenSize() const { return screenSize; }
  const GridInfo& getGridInfo() const { return gridInfo; }

  void setBigFood(BigFood* ptr) { bigFood = ptr; }

 private:
  sf::Window& window;
  Snake& snake;
  Food& food;
  BigFood* bigFood = nullptr;
  Shader& shaderProgram;
  std::pair<GLuint, GLuint>& screenSize;
  const GridInfo gridInfo;

  // OpenGL stuffs
  GLuint VBO, VAO, EBO;
  void setupQuad();
  void pollEvents() const;
  void setupCoordinates() const;

  // Event callbacks
  std::vector<EventCallback> listeners;
  void dispatchEvent(const sf::Event& event) const;
};
