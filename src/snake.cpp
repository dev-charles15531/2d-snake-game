#include "../include/snake.hpp"

#include <SFML/Window/Event.hpp>
#include <iostream>
#include <random>

#include "../include/glad/glad.h"
#include "../include/glm/gtc/type_ptr.hpp"

Snake::Snake(Shader& shaderProgram, int cellUnitSize)
    : shaderProgram(shaderProgram), segments(generateSegments(cellUnitSize)), direction(1)
{
}

/**
 * Generates initial segments for the snake.
 * The snake starts with 3 segments positioned horizontally.
 * @param cUnitSize The size of the grid in which the snake is placed.
 * @return A vector of Cell representing the initial segments of the snake.
 */
std::vector<Cell> Snake::generateSegments(const int& cUnitSize)
{
  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> distX(0, cUnitSize - 3);
  std::uniform_int_distribution<int> distY(0, cUnitSize - 1);

  int rN = distX(gen);
  int rN1 = distY(gen);

  std::vector<Cell> newSegments;

  for (int i{0}; i < 3; ++i) newSegments.push_back({rN - i, rN1});

  return newSegments;
}

/**
 * Draws the snake on the screen using OpenGL.
 * Each segment of the snake is drawn as a scaled and translated quad.
 * @param VAO The Vertex Array Object for the quad.
 * @param cellSize The size of each cell in the grid.
 */
void Snake::draw(const GLuint& VAO, const CellSize& cellSize)
{
  shaderProgram.use();
  glBindVertexArray(VAO);

  GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
  if (modelLoc == -1) std::cerr << "Warning: 'model' uniform not found in shader\n";

  for (const auto& segment : getSegments())
  {
    glm::mat4 model{glm::mat4(1.0f)};
    model = glm::translate(model, glm::vec3(segment.x * cellSize.width, segment.y * cellSize.height, 0.0f));
    model = glm::scale(model, glm::vec3(cellSize.width, cellSize.height, 1.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  glBindVertexArray(0);
}

/**
 * Moves the snake in the current direction.
 * The snake moves one cell at a time based on the moveDelay.
 * The head of the snake moves in the specified direction, and each segment
 * follows the segment in front of it.
 */
void Snake::move()
{
  float deltaTime{clock.restart().asSeconds()};
  timeSinceLastMove += deltaTime;

  // Move the snake only if enough time has passed
  if (timeSinceLastMove >= moveDelay)
  {
    for (size_t i{segments.size() - 1}; i > 0; --i)
    {
      segments[i] = segments[i - 1];
    }

    // Move the head
    switch (direction)
    {
      case 0:
        segments[0].y += 1;
        break;  // Up
      case 1:
        segments[0].x += 1;
        break;  // Right
      case 2:
        segments[0].y -= 1;
        break;  // Down
      case 3:
        segments[0].x -= 1;
        break;  // Left
      default:
        break;
    }

    timeSinceLastMove = 0.0f;
  }
}

/**
 * Sets the direction of the snake.
 * Prevents the snake from reversing direction directly.
 * @param dir The new direction (0: up, 1: right, 2: down, 3: left).
 */
void Snake::setDirection(int dir)
{
  // Prevent the snake from reversing
  if ((direction == 0 && dir != 2) || (direction == 1 && dir != 3) || (direction == 2 && dir != 0) ||
      (direction == 3 && dir != 1))
  {
    direction = dir;
  }
}

/**
 * Attaches keyboard controls to change the snake's direction.
 * Maps arrow keys to corresponding directions.
 * @param keyPressed The key press event to handle.
 */
void Snake::attachControl(const sf::Event::KeyPressed& keyPressed)
{
  switch (keyPressed.scancode)
  {
    case sf::Keyboard::Scan::Right:
      setDirection(1);
      break;
    case sf::Keyboard::Scan::Left:
      setDirection(3);
      break;
    case sf::Keyboard::Scan::Up:
      setDirection(0);
      break;
    case sf::Keyboard::Scan::Down:
      setDirection(2);
      break;
    default:
      break;
  }
}

/**
 * Checks if the snake's head is at the same position as the food.
 * @param foodPosition The position of the food.
 * @return True if the snake is eating the food, false otherwise.
 */
bool Snake::isEating(const std::vector<Cell>& foodPosition)
{
  for (const auto& foodCell : foodPosition)
  {
    if (getHead().x == foodCell.x && getHead().y == foodCell.y) return true;
  }
  return false;
}
