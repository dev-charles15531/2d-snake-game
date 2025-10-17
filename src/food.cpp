#include "../include/food.hpp"

#include <iostream>
#include <random>

#include "../include/glad/glad.h"
#include "../include/glm/gtc/type_ptr.hpp"

Food::Food(Shader& shaderProgram, int cellUnitSize, bool isBigFood)
    : shaderProgram(shaderProgram),
      position(isBigFood ? generateBigFoodPosition(cellUnitSize) : generatePosition(cellUnitSize)),
      cellUnitSize(cellUnitSize)
{
}

/**
 * Generates a random position for the food within the grid.
 * @param cUnitSize The size of the grid in which the food is placed.
 * @return A vector of Cells representing the position of the food.
 */
std::vector<Cell> Food::generatePosition(const int& cUnitSize)
{
  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> distX(0, cUnitSize - 1);
  std::uniform_int_distribution<int> distY(0, cUnitSize - 1);

  return {{distX(gen), distY(gen)}};
}

/**
 * Generates random positions for big food within the grid.
 * Big food occupies a 2x2 area, so the positions are generated accordingly.
 * @param cUnitSize The size of the grid in which the big food is placed.
 * @return A vector of Cells representing the positions of the big food.
 */
std::vector<Cell> Food::generateBigFoodPosition(const int& cUnitSize)
{
  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> distX(0, cUnitSize - 2);
  std::uniform_int_distribution<int> distY(0, cUnitSize - 2);
  int rN = distX(gen);
  int rN1 = distY(gen);

  return {{rN, rN1}, {rN + 1, rN1}, {rN, rN1 + 1}, {rN + 1, rN1 + 1}};
}

/**
 * Returns the current position of the food.
 * @return A Cell vector representing the position of the food.
 */
std::vector<Cell> Food::getPosition() const { return position; }

/**
 * Respawns the food at a new random position within the grid.
 */
void Food::respawn()
{
  respawnCounter++;
  position = generatePosition(cellUnitSize);
}

/**
 * Draws the food on the screen using OpenGL.
 * The food is drawn as a scaled and translated quad.
 * @param VAO The Vertex Array Object for the quad.
 * @param cellSize The size of each cell in the grid.
 */
void Food::draw(const GLuint& VAO, const CellSize& cellSize)
{
  shaderProgram.use();
  glBindVertexArray(VAO);
  GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
  if (modelLoc == -1) std::cerr << "Warning: 'model' uniform not found in shader\n";

  for (const auto& pos : position)
  {
    glm::mat4 model{glm::mat4(1.0f)};
    model = glm::translate(model, glm::vec3(pos.x * cellSize.width, pos.y * cellSize.height, 0.0f));
    model = glm::scale(model, glm::vec3(cellSize.width, cellSize.height, 1.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }
  glBindVertexArray(0);
}
