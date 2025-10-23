#include "../include/food.hpp"

#include <iostream>
#include <random>

#include "../include/glad/glad.h"
#include "../include/glm/gtc/type_ptr.hpp"

Food::Food(Shader& shaderProgram, const GridInfo& gridInfo, bool isBigFood)
    : shaderProgram(shaderProgram),
      gridInfo(gridInfo),
      position(isBigFood ? generateBigFoodPosition() : generatePosition())
{
}

void Food::reset()
{
  setPosition(generatePosition());
  setRespawnCounter(0);
}

/**
 * Generates a random position for the food within the grid.
 * @return A vector of Cells representing the position of the food.
 */
std::vector<Cell> Food::generatePosition() const
{
  auto [xMax, yMax]{gridInfo.getGridSizeI()};

  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> distX(2, xMax - 2);
  std::uniform_int_distribution<int> distY(2, yMax - 2);

  Cell cell{distX(gen), distY(gen)};
  std::cout << "Food spawned at: (" << cell.x << ", " << cell.y << ")\n";
  std::cout << "[DEBUG] gridWidth=" << xMax << " gridHeight=" << yMax << "\n";

  return {cell};
}

/**
 * Generates random positions for big food within the grid.
 * Big food occupies a 2x2 area, so the positions are generated accordingly.
 * @return A vector of Cells representing the positions of the big food.
 */
std::vector<Cell> Food::generateBigFoodPosition() const
{
  auto [xMax, yMax]{gridInfo.getGridSizeI()};

  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> distX(4, xMax - 4);
  std::uniform_int_distribution<int> distY(4, yMax - 4);
  int rN = distX(gen);
  int rN1 = distY(gen);

  return {{rN, rN1}, {rN + 1, rN1}, {rN, rN1 + 1}, {rN + 1, rN1 + 1}};
}

/**
 * Respawns the food at a new random position within the grid.
 */
void Food::respawn()
{
  respawnCounter++;
  position = generatePosition();
}

/**
 * Draws the food on the screen using OpenGL.
 * The food is drawn as a scaled and translated quad.
 * @param VAO The Vertex Array Object for the quad.
 */
void Food::draw(const GLuint& VAO) const
{
  shaderProgram.use();
  glBindVertexArray(VAO);

  GLint modelLoc{glGetUniformLocation(shaderProgram.ID, "model")};
  if (modelLoc == -1) std::cerr << "Warning: 'model' uniform not found in shader\n";

  for (const auto& pos : position)
  {
    glm::mat4 model{1.0f};

    // Place food directly at its grid coordinate
    model = glm::translate(model, glm::vec3(pos.x - 0.5f, pos.y - 0.5f, 0.0f));

    // Scale each cell to a scale factor
    model = glm::scale(model, scaleFactor * glm::vec3(1.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }
  glBindVertexArray(0);
}
