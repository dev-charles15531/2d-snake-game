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

  virtual void reset();

  const std::vector<Cell> &getPosition() const { return position; }
  void setPosition(std::vector<Cell> newPos) { position = newPos; }
  const GridInfo &getGridInfo() const { return gridInfo; }
  const unsigned int &getRespawnCounter() const { return respawnCounter; }
  void setRespawnCounter(GLuint counter) { respawnCounter = counter; }

 private:
  Shader &shaderProgram;
  const GridInfo &gridInfo;
  std::vector<Cell> position;  // position of the food, multiple set for big food
  unsigned int respawnCounter{0};

 protected:
  std::vector<Cell> generatePosition() const;
  std::vector<Cell> generateBigFoodPosition() const;
};
