#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

struct ImGuiContext;

namespace ImGui
{
namespace SFML
{
bool Init(const sf::Window& window);
void ProcessEvent(const sf::Event& event);
void Update(const sf::Window& window, sf::Time dt);
void Shutdown();
}  // namespace SFML
}  // namespace ImGui
