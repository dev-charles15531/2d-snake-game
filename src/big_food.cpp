#include "../include/big_food.hpp"

#include "../include/shader.hpp"

BigFood::BigFood(Shader& shader, const GridInfo& gridInfo) : Food(shader, gridInfo, true) {}

/**
 * Start counting when the currently active big food should disappear
 */
void BigFood::startCounting()
{
  if (!isCounting)
  {
    // Start the countdown
    clock.restart();
    isCounting = true;
    lastTime = 0.0f;
    return;
  }

  // Get total time since start
  float currentTime{clock.getElapsedTime().asSeconds()};
  float deltaTime{currentTime - lastTime};
  lastTime = currentTime;

  timeToLive -= deltaTime;

  if (timeToLive <= 0.0f)
  {
    isActive = false;
    isCounting = false;
    timeToLive = 15.0f;  // reset for next spawn
  }
}
