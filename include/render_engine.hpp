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

  RenderEngine(sf::Window& window, Snake& snake, Shader& shaderProgram, Food& food, CellSize& cellSize,
               ScreenSize& screenSize);
  ~RenderEngine();
  void clearScreen();
  void render();
  void terminate();

  sf::Window& getWindow() const { return window; }
  void addEventListener(const EventCallback& event);

  void setBigFood(std::optional<std::reference_wrapper<BigFood>> bigFoodRef);

  GLuint getVAO() const { return VAO; }

 private:
  sf::Window& window;
  Snake& snake;
  Food& food;
  std::optional<std::reference_wrapper<BigFood>> bigFood;
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
