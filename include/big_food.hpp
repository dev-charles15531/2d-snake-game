#pragma once

#include <SFML/System/Clock.hpp>
// #include <utility>

#include "food.hpp"
#include "header.hpp"

class BigFood : public Food
{
 public:
  BigFood(Shader& shader, const GridInfo& gridInfo);
  bool isActive = false;
  void startCounting();

 private:
  sf::Clock clock;
  float timeToLive = 15.0f;
  bool isCounting = false;
  float lastTime = 0.0f;
};
