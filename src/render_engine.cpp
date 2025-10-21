#include "../include/render_engine.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <utility>
#include <vector>

#include "../include/glad/glad.h"
#include "../include/glm/gtc/type_ptr.hpp"

RenderEngine::RenderEngine(sf::Window& window, Snake& snake, Shader& shaderProgram, Food& food, ScreenSize& screenSize,
                           GLuint gridSize)
    : window(window),
      snake(snake),
      shaderProgram(shaderProgram),
      food(food),
      screenSize(screenSize),
      gridInfo(gridSize, screenSize)
{
  setupQuad();
  setupCoordinates();
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
  // Define a simple 1x1 square with its bottom-left corner at the origin
  std::vector<GLfloat> vertices{
      0.5f,  0.5f,  0.0f,  // top right
      0.5f,  -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f   // top left
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
void RenderEngine::setupCoordinates() const
{
  GLfloat aspectRatio{static_cast<GLfloat>(screenSize.first) /
                      static_cast<GLfloat>(screenSize.second == 0 ? 1.0f : screenSize.second)};

  shaderProgram.use();

  glm::mat4 view{1.0f};
  auto [xMax, yMax] = gridInfo.getGridSizeF();
  glm::mat4 projection{glm::ortho(0.0f, xMax,  // left, right
                                  yMax, 0.0f,  // top, bottom (flip Y)
                                  -1.0f, 1.0f)};

  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

/**
 * Clears the screen with a specified color and clears the depth buffer.
 */
void RenderEngine::clearScreen() const
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
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

  // Draw the snake
  snake.draw(VAO);

  // Draw the food
  food.draw(VAO);

  // Draw big food if available
  if (bigFood && bigFood->isActive) bigFood->draw(VAO);

  window.display();
}

/**
 * Cleans up allocated OpenGL resources.
 */
void RenderEngine::terminate() const
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

/**
 * Poll events during rendering
 */
void RenderEngine::pollEvents() const
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
 * Add event listener to the render engine.
 * @param callback The event callback function
 */
void RenderEngine::addEventListener(const EventCallback& callback) { listeners.push_back(callback); }

/**
 * Dispatch event from array of listeners
 * @param event The event to dispatch
 */
void RenderEngine::dispatchEvent(const sf::Event& event) const
{
  for (auto& listener : listeners)
  {
    listener(event);
  }
}
