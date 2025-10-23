#include "../include/snake.hpp"

#include <SFML/Window/Event.hpp>
#include <iostream>
#include <random>

#include "../include/glad/glad.h"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/render_engine.hpp"

Snake::Snake(Shader& shaderProgram, const GridInfo& gridInfo)
    : shaderProgram(shaderProgram), segments(generateSegments()), direction(1), gridInfo(gridInfo)
{
}

/**
 * Generates initial segments for the snake.
 * The snake starts with 3 segments positioned horizontally.
 * @return A vector of Cell representing the initial segments of the snake.
 */
std::vector<Cell> Snake::generateSegments()
{
  auto [xMax, yMax]{gridInfo.getGridSizeI()};

  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> distX(0, xMax - 3);
  std::uniform_int_distribution<int> distY(0, yMax - 1);

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
 */
void Snake::draw(const GLuint& VAO) const
{
  shaderProgram.use();
  glBindVertexArray(VAO);

  GLint modelLoc{glGetUniformLocation(shaderProgram.ID, "model")};
  if (modelLoc == -1) std::cerr << "Warning: 'model' uniform not found in shader\n";

  for (const auto& segment : getSegments())
  {
    glm::mat4 model{1.0f};

    // Place each segment directly at its grid coordinate
    model = glm::translate(model, glm::vec3(segment.x - 0.5f, segment.y - 0.5f, 0.0f));

    // Scale each cell to 1.0 unit (each grid cell is 1 world unit)
    model = glm::scale(model, scaleFactor * glm::vec3(1.0f));

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
  float deltaTime{clock.getElapsedTime().asSeconds()};

  // Move the snake only if enough time has passed
  if (deltaTime >= moveDelay)
  {
    clock.restart();

    // move body
    for (size_t i{segments.size() - 1}; i > 0; --i) segments[i] = segments[i - 1];

    // move head    // Move the head
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
  }
}

/**
 * Moves the snake, and eat food it comes contact with.
 * @param renderEngine Reference to the current running render engine.
 * @param food Reference to food instance.
 * @param bigFood Reference to big food instance.
 * @return 1 if collision happened during the course of snake's movement,
 *         0 if movement and eating is happening without collision.
 */
int Snake::moveAndEat(RenderEngine& renderEngine, Food& food, std::unique_ptr<BigFood>& bigFood)
{
  // start moving the snake
  move();

  // Check for collision
  if (isCollided())
  {
    return 1;
  }

  // allow for snake movement to wrap around edges
  mirrorEdges();

  // timer for big food if available
  if (bigFood && bigFood->isActive) bigFood->startCounting();

  // Check if the snake has eaten the food
  if (isEating(food.getPosition()))
  {
    // grow snake and spawn new food
    grow();
    food.respawn();

    // spawn big food after eating every x food
    // TODO: never spawn big food in position of normal food
    if (food.getRespawnCounter() % 2 == 0 && food.getRespawnCounter())
    {
      std::cout << "Big Food Spawned!\n";
      bigFood = std::make_unique<BigFood>(shaderProgram, renderEngine.getGridInfo());
      bigFood->isActive = true;
      renderEngine.setBigFood(bigFood.get());
    }
  }

  // check if snake has eaten big food
  else if (bigFood && bigFood->isActive && isEating(bigFood->getPosition()))
  {
    grow();
    bigFood->isActive = false;
  }

  return 0;
}

/**
 * Sets the direction of the snake.
 * Prevents the snake from reversing direction directly.
 * @param dir The new direction (0: down, 1: right, 2: up, 3: left).
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
      setDirection(2);
      break;
    case sf::Keyboard::Scan::Down:
      setDirection(0);
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
bool Snake::isEating(const std::vector<Cell>& foodPosition) const
{
  const auto& head{getHead()};
  std::cout << "[isEating] Head: (" << head.x << ", " << head.y << ")\n";

  for (const auto& foodCell : foodPosition)
  {
    std::cout << "  Food: (" << foodCell.x << ", " << foodCell.y << ")\n";
    if (head.x == foodCell.x && head.y == foodCell.y)
    {
      std::cout << "  -> Match found!\n";
      return true;
    }
  }
  return false;
}

/**
 * Set the head position of the snake
 * @param cell The cell position of the new snake head
 */
void Snake::setHead(Cell cell)
{
  if (!segments.empty())
  {
    segments.front() = cell;
  }
}

/**
 * Wrap snake's movement aroun the edges such that;
 * if the snake leaves from the left edge of the screen,
 * it appears back from the right. same apply for all sides.
 */
void Snake::mirrorEdges()
{
  auto head{getHead()};
  auto [xMax, yMax]{gridInfo.getGridSizeI()};

  if (head.x < 0)
    head.x = xMax;
  else if (head.x >= xMax + 0.5)
    head.x = 0;

  if (head.y < 0)
    head.y = yMax;
  else if (head.y >= yMax + 0.5)
    head.y = 0;

  setHead(head);
}

/**
 * Checks if the snake collided with itself.
 * @return True if there is collision, and false otherwise.
 */
bool Snake::isCollided() const
{
  const Cell& snakeHead{getHead()};

  for (auto it{segments.begin() + 1}; it != segments.end(); ++it)
  {
    if (snakeHead.x == it->x && snakeHead.y == it->y)
    {
      std::cout << "Collision detected\n";
      return true;
    }
  }

  return false;
}
