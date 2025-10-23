#include "../include/render_engine.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <vector>

#include "../include/game.hpp"
#include "../include/glad/glad.h"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/imgui/imgui_impl_sfml.h"

RenderEngine::RenderEngine(sf::Window& window, Snake& snake, Shader& shaderProgram, Food& food, ScreenSize& screenSize,
                           GridInfo& gridInfo, GUI& gui, Game* game)
    : window(window),
      shaderProgram(shaderProgram),
      snake(snake),
      food(food),
      game(game),
      screenSize(screenSize),
      gridInfo(gridInfo),
      gui(gui)
{
  setupQuad();
  setupCoordinates();

  // initialize ImGUI
  gui.init(window);
  imguiInitialized = true;
}

RenderEngine::~RenderEngine() { terminate(); }

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
  shaderProgram.use();

  glm::mat4 view{1.0f};
  auto [xMax, yMax]{gridInfo.getGridSizeF()};
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
  // Poll SFML events once per frame
  pollEvents();

  // === ImGui Frame Start ===
  sf::Time dt = clock.restart();
  gui.beginFrame(window, dt);

  // === Draw ImGui windows from Game ===
  if (game)
  {
    game->showHUD();        // Always show HUD
    game->showPauseMenu();  // Shows only when paused
  }

  // Big food timer
  if (bigFood && bigFood->isActive)
  {
    bigFood->drawUI();
  }

  // === Clear and draw game ===
  clearScreen();

  // Draw the snake and food using OpenGL
  snake.draw(VAO);
  food.draw(VAO);
  if (bigFood && bigFood->isActive)
  {
    bigFood->draw(VAO);
  }

  // Draw ImGui on top of everything
  gui.endFrame();

  // Swap buffers / display frame
  window.display();
}

/**
 * Cleans up allocated OpenGL resources.
 */
void RenderEngine::terminate()
{
  if (imguiInitialized)
  {
    // Ensure context is valid before ImGui shutdown
    if (window.isOpen())
    {
      gui.shutdown();
    }
    imguiInitialized = false;
  }

  // Delete OpenGL resources safely only if context is active
  if (window.isOpen())
  {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }
}

/**
 * Poll events during rendering
 */
void RenderEngine::pollEvents()
{
  while (const std::optional<sf::Event> event{window.pollEvent()})
  {
    // ImGUI events
    ImGui::SFML::ProcessEvent(event.value());

    if (event->is<sf::Event::Closed>())
    {
      window.close();
    }

    if (event->is<sf::Event::Resized>())
    {
      // gridInfo.updateScreenSize()
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
