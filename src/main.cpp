#include <SFML/Window.hpp>

#include "../include/game.hpp"
#include "../include/glad/glad.h"

int main()
{
  Game game;

  // run the game loop
  game.run();

  return 0;
}
