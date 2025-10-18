#pragma once

#include <SFML/System/Clock.hpp>

#include "food.hpp"

class BigFood : public Food
{
 public:
  BigFood(Shader& shader, int cellUnitSize);
  bool isActive{false};
  void startCounting();

 private:
  float timeToLive{10.0f};
  sf::Clock clock;
};
