#include "../include/render_engine.hpp"

#include <SFML/Window/Window.hpp>
#include <iostream>
#include <vector>

#include "../include/glad/glad.h"
#include "../include/glm/gtc/type_ptr.hpp"

RenderEngine::RenderEngine(Snake& snake, Shader& shaderProgram, int screenWidth, int screenHeight)
    : snake(snake), shaderProgram(shaderProgram), screenWidth(screenWidth), screenHeight(screenHeight)
{
  setupQuad();
  setupCoordinates();
  glEnable(GL_DEPTH_TEST);
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

  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 projection =
      glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight), 0.1f, 100.0f);

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
 * Renders the snake on the screen.
 * Each segment of the snake is drawn as a scaled and translated quad.
 * @param window The SFML window where rendering occurs.
 * @param cellWidth Width of each cell in the grid.
 * @param cellHeight Height of each cell in the grid.
 */
void RenderEngine::render(sf::Window& window, float cellWidth, float cellHeight)
{
  // Handle window events
  pollEvents(window);

  // Clear the screen
  clearScreen();

  shaderProgram.use();
  glBindVertexArray(VAO);

  GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
  if (modelLoc == -1) std::cerr << "Warning: 'model' uniform not found in shader\n";

  for (const auto& segment : snake.getSegments())
  {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(segment.x * cellWidth, segment.y * cellHeight, 0.0f));
    model = glm::scale(model, glm::vec3(cellWidth / 2.0f, cellHeight / 2.0f, 1.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  glBindVertexArray(0);
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
 * Polls and processes window events.
 * @param window The SFML window to poll events from.
 */
void RenderEngine::pollEvents(sf::Window& window)
{
  while (const std::optional<sf::Event> event{window.pollEvent()})
  {
    if (event->is<sf::Event::Closed>()) window.close();

    if (event->is<sf::Event::Resized>())
    {
      glViewport(0, 0, window.getSize().x, window.getSize().y);
    }
  }
}
