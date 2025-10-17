#pragma once

#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>

#include "./header.hpp"
#include "./shader.hpp"

class Food
{
 public:
  Food(Shader &, int cellUnitSize, bool isBigFood = false);
  std::vector<Cell> getPosition() const;
  void respawn();
  void draw(const GLuint &VAO, const CellSize &cellSize);
  unsigned int &getRespawnCounter() { return respawnCounter; }
  int getCellUnitSize() const { return cellUnitSize; }

 private:
  Shader &shaderProgram;
  std::vector<Cell> position;  // position of the food, multiple set for big food
  static std::vector<Cell> generatePosition(const int &cUnitSize);
  static std::vector<Cell> generateBigFoodPosition(const int &cUnitSize);
  int cellUnitSize;
  unsigned int respawnCounter{0};
};
