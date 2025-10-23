#include "../include/big_food.hpp"

#include "../include/imgui/imgui.h"

BigFood::BigFood(Shader& shader, const GridInfo& gridInfo) : Food(shader, gridInfo, true) {}

/**
 * Starts or updates the countdown timer for big food expiration.
 * The countdown scales dynamically with the snake's move delay (speed).
 *
 * @param snakeMoveDelay The delay between snake moves (smaller = faster).
 */
void BigFood::startCounting(GLfloat snakeMoveDelay)
{
  if (!isCounting)
  {
    // Start countdown on first call
    clock.restart();
    isCounting = true;
    lastTime = 0.0f;
    return;
  }

  // Time delta
  float currentTime = clock.getElapsedTime().asSeconds();
  float deltaTime = currentTime - lastTime;
  lastTime = currentTime;

  // Adjust expiration rate based on snake speed
  const float baselineDelay = 0.3f;  // typical snake speed baseline
  float speedFactor = baselineDelay / snakeMoveDelay;

  // Apply scaled time decay
  timeToLive -= deltaTime * speedFactor;

  if (timeToLive <= 0.0f)
  {
    isActive = false;
    isCounting = false;
    timeToLive = LifeTime;  // reset for next spawn
  }
}

void BigFood::drawUI() const
{
  // Debug window
  ImVec2 windowPos{ImVec2((ImGui::GetIO().DisplaySize.x / 2) - 1.0f, ImGui::GetIO().DisplaySize.y - 40.0f)};
  ImVec2 pivot{ImVec2(0.5f, 0.5f)};  // center

  ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, pivot);
  ImGui::Begin("big food time board", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);

  GLfloat progress{(timeToLive - 0.0f) / (LifeTime - 0.0f)};

  ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f), "");
  ImGui::End();
}

void BigFood::reset()
{
  setPosition(Food::generateBigFoodPosition());
  setRespawnCounter(0);

  timeToLive = LifeTime;
  isCounting = false;
  lastTime = 0.0f;
  isActive = false;
}
