#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Window/Window.hpp>

class GUI
{
 public:
  GUI();
  ~GUI();

  void init(sf::Window& window);
  void shutdown();

  void beginFrame(sf::Window& window, sf::Time deltaTime);
  void endFrame();
};
