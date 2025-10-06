#include "../include/glad/glad.h"
#include <SFML/Window.hpp>
#include <iostream>

int main() {
  // Request a core OpenGL context
  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.attributeFlags = sf::ContextSettings::Core;

  // Get the desktop resolution
  sf::VideoMode desktopMode{sf::VideoMode::getDesktopMode()};
  unsigned int screenWidth{desktopMode.size.x / 2};
  unsigned int screenHeight{desktopMode.size.y / 2};

  desktopMode.size.x = screenWidth;
  desktopMode.size.y = screenHeight;

  sf::Window window(desktopMode, "SFML + GLAD", sf::Style::Default,
                    sf::State::Windowed, settings);
  window.setFramerateLimit(60);

  // Initialize GLAD
  if (!gladLoadGL()) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  while (window.isOpen()) {
    while (const std::optional<sf::Event> event{window.pollEvent()}) {
      if (event->is<sf::Event::Closed>())
        window.close();

      if (event->is<sf::Event::Resized>()) {
        glViewport(0, 0, window.getSize().x, window.getSize().y);
      }
    }

    // OpenGL rendering here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    window.display();
  }

  return 0;
}
