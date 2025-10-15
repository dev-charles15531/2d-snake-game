#include "../include/snake.hpp"

#include <SFML/Window/Event.hpp>

#include "../include/glad/glad.h"

Snake::Snake(GLuint direction, Shader& shaderProgram)
    : direction(direction), shaderProgram(shaderProgram), segments(generateSegments())
{
}

/**
 * Generates the initial segments of the snake.
 * The snake starts with 3 segments positioned horizontally.
 * @return A vector of Cell representing the initial segments of the snake.
 */
std::vector<Cell> Snake::generateSegments()
{
  std::vector<Cell> newSegments;
  for (int i{0}; i < 3; ++i)
  {
    newSegments.push_back({5 + i, 10});
  }

  return newSegments;
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
