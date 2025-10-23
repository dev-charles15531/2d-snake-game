#include "../include/game.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "../include/glad/glad.h"
#include "../include/imgui/imgui.h"

Game::Game()
    : gridSize(80),  // Square matrix
      screenSize{sf::VideoMode::getDesktopMode().size.x / 2, sf::VideoMode::getDesktopMode().size.y / 2},
      gridInfo(gridSize, screenSize)
{
  // Setup window and OpenGL context
  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.majorVersion = 4;
  settings.minorVersion = 4;
  settings.attributeFlags = sf::ContextSettings::Core;

  sf::VideoMode desktopMode{sf::VideoMode::getDesktopMode()};
  desktopMode.size.x = screenSize.first;
  desktopMode.size.y = screenSize.second;

  window.create(desktopMode, "SNAKE GAME", sf::Style::Default, sf::State::Windowed, settings);
  window.setFramerateLimit(60);

  if (!gladLoadGL())
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    exit(1);
  }

  shaderProgram = std::make_unique<Shader>("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");
  snake = std::make_unique<Snake>(*shaderProgram, gridInfo);
  food = std::make_unique<Food>(*shaderProgram, gridInfo);
  gui = std::make_unique<GUI>();
  renderEngine =
      std::make_unique<RenderEngine>(window, *snake, *shaderProgram, *food, screenSize, gridInfo, *gui, this);

  // Attach event listener for controls
  renderEngine->addEventListener(
      [this](const sf::Event& event)
      {
        if (event.is<sf::Event::KeyPressed>())
        {
          const auto* keyPressed{event.getIf<sf::Event::KeyPressed>()};
          if (isPlaying)
          {
            // attach control to snake
            snake->attachControl(*keyPressed);
          }

          // toggle play/pause on space key
          if (keyPressed->scancode == sf::Keyboard::Scan::Space || keyPressed->scancode == sf::Keyboard::Scan ::Escape)
          {
            isPlaying = !isPlaying;
            showPauseMenuWindow = !isPlaying;
          }

          // Reset on R key when paused
          if (!isPlaying && keyPressed->scancode == sf::Keyboard::Scan::R)
          {
            resetGame();
            isPlaying = !isPlaying;
            showPauseMenuWindow = !isPlaying;
          }
        }
      });
}

///// RUN THE GAME /////
void Game::run()
{
  while (window.isOpen())
  {
    // Game logic
    if (isPlaying)
    {
      GLuint snakeAction = snake->moveAndEat(*renderEngine, *food, bigFood);

      if (snakeAction == 1)
      {
        std::cout << "💀 Game Over!\n";
        isPlaying = false;
        showPauseMenuWindow = true;  // Show pause menu on game over
      }
      else if (snakeAction == 2)
      {
        score++;
      }
      else if (snakeAction == 3)
      {
        score += 2;
      }
    }

    // Set transparency for HUD (not pause menu)
    if (!showPauseMenuWindow)
    {
      ImGui::GetStyle().Alpha = 0.3f;
    }
    else
    {
      // Reset alpha for pause menu
      ImGui::GetStyle().Alpha = 1.0f;
    }

    // Render game
    renderEngine->render();
  }
}

void Game::resetGame()
{
  // Update high score
  if (score > highScore)
  {
    highScore = score;
    score = 0;
  }

  // Reset game objects
  snake->reset();
  food->reset();
  if (bigFood->isActive) bigFood->reset();
}

void Game::showHUD()  // TODO: Treat these widgets as obstacles
{
  // === HUD: Game Stats (top-left) ===
  ImVec2 windowPos = ImVec2(10.0f, 5.0f);
  ImVec2 pivot = ImVec2(0.0f, 0.0f);
  ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, pivot);
  ImGui::Begin("Game Stats", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("%s", isPlaying ? "# Playing" : "# Paused");
  ImGui::Separator();
  ImGui::Text("Length: %zu", snake->getSegments().size());
  ImGui::End();

  // === HUD: Score (top-right) ===
  windowPos = ImVec2(ImGui::GetIO().DisplaySize.x - 10.0f, 5.0f);
  pivot = ImVec2(1.0f, 0.0f);
  ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, pivot);
  ImGui::Begin("Score", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Score: %d", score);
  ImGui::Separator();
  ImGui::Text("High score: %d", highScore);  // TODO: Implement highscore persistent storing and retrieval feature
  ImGui::End();
}

void Game::showPauseMenu()
{
  if (!showPauseMenuWindow) return;

  // Center the window
  ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);

  ImGui::Begin("- GAME PAUSED", &showPauseMenuWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // === GAME STATS ===
  ImGui::Text("- Snake Length: %zu", snake->getSegments().size());
  ImGui::Text("- Score: %d", score);
  ImGui::Text("- High Score: %d", highScore);

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // === DIFFICULTY SETTINGS ===
  ImGui::Text("-  Game Settings");
  ImGui::Spacing();

  // Difficulty preset
  const char* difficulties[] = {"Easy", "Medium", "Hard", "Insane", "Custom"};
  if (ImGui::Combo("Difficulty", &difficulty, difficulties, IM_ARRAYSIZE(difficulties)))
  {
    switch (difficulty)
    {
      case 0:
        snakeSpeed = 0.3f;
        break;  // Easy
      case 1:
        snakeSpeed = 0.2f;
        break;  // Medium
      case 2:
        snakeSpeed = 0.1f;
        break;  // Hard
      case 3:
        snakeSpeed = 0.05f;
        break;  // Insane
      case 4:
        break;  // Custom
    }
    snake->setMoveDelay(snakeSpeed);
  }

  // Custom speed slider
  ImGui::Text("Snake Speed");
  if (ImGui::SliderFloat("##speed", &snakeSpeed, 0.05f, 0.3f, "%.2fs"))
  {
    snake->setMoveDelay(snakeSpeed);
    difficulty = 4;  // Custom speed
  }
  ImGui::SameLine();
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered())
  {
    ImGui::SetTooltip("Lower = Faster snake");
  }

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // === CONTROL GUIDE ===
  ImGui::Text("- Controls");
  ImGui::BulletText("Arrow Keys: Move snake");
  ImGui::BulletText("Space/Esc: Pause/Resume");
  ImGui::BulletText("R: Reset game (when paused)");

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // === ACTION BUTTONS ===
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));

  if (ImGui::Button("Resume Game", ImVec2(-1, 40)))
  {
    isPlaying = true;
    showPauseMenuWindow = false;
  }
  ImGui::PopStyleColor(3);

  ImGui::Spacing();

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.4f, 0.0f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.5f, 0.1f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.3f, 0.0f, 1.0f));

  if (ImGui::Button("Reset Game", ImVec2(-1, 40)))
  {
    resetGame();
    isPlaying = true;
    showPauseMenuWindow = false;
  }
  ImGui::PopStyleColor(3);

  ImGui::Spacing();

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.0f, 0.0f, 1.0f));

  if (ImGui::Button("x Quit Game", ImVec2(-1, 40)))
  {
    window.close();
  }

  ImGui::PopStyleColor(3);

  ImGui::End();
}
