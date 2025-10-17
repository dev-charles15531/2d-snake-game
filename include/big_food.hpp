#pragma once

#include "food.hpp"

class BigFood : public Food
{
 public:
  BigFood(Shader& shader, int cellUnitSize);
  bool isActive{false};
};
