#pragma once

#include <SFML/System/Clock.hpp>

#include "food.hpp"
#include "header.hpp"

class BigFood : public Food
{
 public:
  BigFood(Shader& shader, const GridInfo& gridInfo);
  bool isActive = false;
  void startCounting(GLfloat snakeMoveDelay);
  void drawUI() const;
  void reset() override;

  static GLfloat constexpr LifeTime{20.0f};

 private:
  sf::Clock clock;
  float timeToLive{LifeTime};
  bool isCounting = false;
  float lastTime = 0.0f;
};
