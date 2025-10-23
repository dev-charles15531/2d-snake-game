#pragma once

#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>

#include "./header.hpp"
#include "./shader.hpp"

class Food
{
 public:
  Food(Shader &, const GridInfo &gridInfo, bool isBigFood = false);

  void draw(const GLuint &VAO) const;
  void respawn();

  const std::vector<Cell> &getPosition() const { return position; }
  const unsigned int &getRespawnCounter() const { return respawnCounter; }

 private:
  Shader &shaderProgram;
  const GridInfo &gridInfo;
  std::vector<Cell> position;  // position of the food, multiple set for big food
  std::vector<Cell> generatePosition() const;
  std::vector<Cell> generateBigFoodPosition() const;
  unsigned int respawnCounter{0};
};
