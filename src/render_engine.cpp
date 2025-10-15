#include "../include/render_engine.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <iostream>
#include <vector>

#include "../include/glad/glad.h"
#include "../include/glm/gtc/type_ptr.hpp"

RenderEngine::RenderEngine(sf::Window& window, Snake& snake, Shader& shaderProgram, CellSize& cellSize,
                           ScreenSize& screenSize)
    : window(window), snake(snake), shaderProgram(shaderProgram), cellSize(cellSize), screenSize(screenSize)
{
  setupQuad();
  setupCoordinates();
  glEnable(GL_DEPTH_TEST);
}

RenderEngine::~RenderEngine()
{
  terminate();
  window.close();
}

/**
 * Sets up a simple quad (square) for rendering.
 * The quad is defined with 4 vertices and 2 triangles.
 */
void RenderEngine::setupQuad()
{
  // Define a simple square centered at origin
  std::vector<GLfloat> vertices{
      // positions
      1.0f,  1.0f,  0.0f,  // top right
      1.0f,  -1.0f, 0.0f,  // bottom right
      -1.0f, -1.0f, 0.0f,  // bottom left
      -1.0f, 1.0f,  0.0f   // top left
  };

  std::vector<GLuint> indices{
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  // Vertex attribute (position only)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  // Safety: unbind
  glBindVertexArray(0);
}

/**
 * Sets up the view and projection matrices for 2D rendering.
 * The view matrix translates the scene back to view it properly.
 * The projection matrix is set to orthographic projection based on screen dimensions.
 */
void RenderEngine::setupCoordinates()
{
  shaderProgram.use();

  glm::mat4 view{glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))};
  glm::mat4 projection{glm::ortho(0.0f, static_cast<float>(screenSize.first), 0.0f,
                                  static_cast<float>(screenSize.second), 0.1f, 100.0f)};
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

/**
 * Clears the screen with a specified color and clears the depth buffer.
 */
void RenderEngine::clearScreen()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * Rendering engine
 */
void RenderEngine::render()
{
  // Handle window events
  pollEvents();

  // Clear the screen
  clearScreen();

  shaderProgram.use();
  glBindVertexArray(VAO);

  GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
  if (modelLoc == -1) std::cerr << "Warning: 'model' uniform not found in shader\n";

  for (const auto& segment : snake.getSegments())
  {
    glm::mat4 model{glm::mat4(1.0f)};
    model = glm::translate(model, glm::vec3(segment.x * cellSize.width, segment.y * cellSize.height, 0.0f));
    model = glm::scale(model, glm::vec3(cellSize.width, cellSize.height, 1.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  glBindVertexArray(0);

  window.display();
}

/**
 * Cleans up allocated OpenGL resources.
 */
void RenderEngine::terminate()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

/**
 * Poll events during rendering
 */
void RenderEngine::pollEvents()
{
  while (const std::optional<sf::Event> event{window.pollEvent()})
  {
    if (event->is<sf::Event::Closed>()) window.close();

    if (event->is<sf::Event::Resized>())
    {
      glViewport(0, 0, window.getSize().x, window.getSize().y);
    }

    // dispatch other events to listeners
    dispatchEvent(event.value());
  }
}

/**
 *
 */
void RenderEngine::addEventListener(const EventCallback& callback) { listeners.push_back(callback); }

void RenderEngine::dispatchEvent(const sf::Event& event)
{
  for (auto& listener : listeners)
  {
    listener(event);
  }
}
