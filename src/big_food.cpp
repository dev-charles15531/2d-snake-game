#include "../include/big_food.hpp"

#include "../include/shader.hpp"

BigFood::BigFood(Shader &shader, int cellUnitSize) : Food(shader, cellUnitSize, true) {}

void BigFood::startCounting()
{
  float deltaTime{clock.restart().asSeconds()};
  timeToLive -= deltaTime;

  if (timeToLive <= 0)
  {
    isActive = false;

    timeToLive = 10.0f;
  }
}
