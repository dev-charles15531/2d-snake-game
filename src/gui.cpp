#include "../include/gui.hpp"

#include <SFML/System/Time.hpp>

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#include "../include/imgui/imgui_impl_sfml.h"

GUI::GUI() {}
GUI::~GUI() {}

void GUI::init(sf::Window& window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup style
  ImGui::StyleColorsDark();

  // Initialize backends
  ImGui::SFML::Init(window);
  ImGui_ImplOpenGL3_Init("#version 440");
}

void GUI::shutdown()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui::SFML::Shutdown();
  ImGui::DestroyContext();
}

void GUI::beginFrame(sf::Window& window, sf::Time deltaTime)
{
  ImGui::SFML::Update(window, deltaTime);
  ImGui_ImplOpenGL3_NewFrame();
}
void GUI::endFrame()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
