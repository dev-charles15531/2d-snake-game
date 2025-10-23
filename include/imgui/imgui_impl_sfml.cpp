#include "../../include/imgui/imgui_impl_sfml.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>

#include "../../include/imgui/imgui.h"

using namespace ImGui;

static sf::Vector2i g_MousePos;
static bool g_MouseJustPressed[5] = {false, false, false, false, false};

bool ImGui::SFML::Init(const sf::Window& window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.BackendPlatformName = "imgui_impl_sfml";

  // Set initial display size
  sf::Vector2u size = window.getSize();
  io.DisplaySize = ImVec2((float)size.x, (float)size.y);

  return true;
}

// Helper function to convert SFML Scancode to ImGuiKey
static ImGuiKey SFMLScancodeToImGuiKey(sf::Keyboard::Scancode scancode)
{
  switch (scancode)
  {
    case sf::Keyboard::Scan::Tab:
      return ImGuiKey_Tab;
    case sf::Keyboard::Scan::Left:
      return ImGuiKey_LeftArrow;
    case sf::Keyboard::Scan::Right:
      return ImGuiKey_RightArrow;
    case sf::Keyboard::Scan::Up:
      return ImGuiKey_UpArrow;
    case sf::Keyboard::Scan::Down:
      return ImGuiKey_DownArrow;
    case sf::Keyboard::Scan::PageUp:
      return ImGuiKey_PageUp;
    case sf::Keyboard::Scan::PageDown:
      return ImGuiKey_PageDown;
    case sf::Keyboard::Scan::Home:
      return ImGuiKey_Home;
    case sf::Keyboard::Scan::End:
      return ImGuiKey_End;
    case sf::Keyboard::Scan::Insert:
      return ImGuiKey_Insert;
    case sf::Keyboard::Scan::Delete:
      return ImGuiKey_Delete;
    case sf::Keyboard::Scan::Backspace:
      return ImGuiKey_Backspace;
    case sf::Keyboard::Scan::Space:
      return ImGuiKey_Space;
    case sf::Keyboard::Scan::Enter:
      return ImGuiKey_Enter;
    case sf::Keyboard::Scan::Escape:
      return ImGuiKey_Escape;
    case sf::Keyboard::Scan::Apostrophe:
      return ImGuiKey_Apostrophe;
    case sf::Keyboard::Scan::Comma:
      return ImGuiKey_Comma;
    case sf::Keyboard::Scan::Hyphen:
      return ImGuiKey_Minus;
    case sf::Keyboard::Scan::Period:
      return ImGuiKey_Period;
    case sf::Keyboard::Scan::Slash:
      return ImGuiKey_Slash;
    case sf::Keyboard::Scan::Semicolon:
      return ImGuiKey_Semicolon;
    case sf::Keyboard::Scan::Equal:
      return ImGuiKey_Equal;
    case sf::Keyboard::Scan::LBracket:
      return ImGuiKey_LeftBracket;
    case sf::Keyboard::Scan::Backslash:
      return ImGuiKey_Backslash;
    case sf::Keyboard::Scan::RBracket:
      return ImGuiKey_RightBracket;
    case sf::Keyboard::Scan::Grave:
      return ImGuiKey_GraveAccent;
    case sf::Keyboard::Scan::CapsLock:
      return ImGuiKey_CapsLock;
    case sf::Keyboard::Scan::ScrollLock:
      return ImGuiKey_ScrollLock;
    case sf::Keyboard::Scan::NumLock:
      return ImGuiKey_NumLock;
    case sf::Keyboard::Scan::PrintScreen:
      return ImGuiKey_PrintScreen;
    case sf::Keyboard::Scan::Pause:
      return ImGuiKey_Pause;
    case sf::Keyboard::Scan::Numpad0:
      return ImGuiKey_Keypad0;
    case sf::Keyboard::Scan::Numpad1:
      return ImGuiKey_Keypad1;
    case sf::Keyboard::Scan::Numpad2:
      return ImGuiKey_Keypad2;
    case sf::Keyboard::Scan::Numpad3:
      return ImGuiKey_Keypad3;
    case sf::Keyboard::Scan::Numpad4:
      return ImGuiKey_Keypad4;
    case sf::Keyboard::Scan::Numpad5:
      return ImGuiKey_Keypad5;
    case sf::Keyboard::Scan::Numpad6:
      return ImGuiKey_Keypad6;
    case sf::Keyboard::Scan::Numpad7:
      return ImGuiKey_Keypad7;
    case sf::Keyboard::Scan::Numpad8:
      return ImGuiKey_Keypad8;
    case sf::Keyboard::Scan::Numpad9:
      return ImGuiKey_Keypad9;
    case sf::Keyboard::Scan::NumpadDecimal:
      return ImGuiKey_KeypadDecimal;
    case sf::Keyboard::Scan::NumpadDivide:
      return ImGuiKey_KeypadDivide;
    case sf::Keyboard::Scan::NumpadMultiply:
      return ImGuiKey_KeypadMultiply;
    case sf::Keyboard::Scan::NumpadMinus:
      return ImGuiKey_KeypadSubtract;
    case sf::Keyboard::Scan::NumpadPlus:
      return ImGuiKey_KeypadAdd;
    case sf::Keyboard::Scan::NumpadEnter:
      return ImGuiKey_KeypadEnter;
    case sf::Keyboard::Scan::NumpadEqual:
      return ImGuiKey_KeypadEqual;
    case sf::Keyboard::Scan::LShift:
      return ImGuiKey_LeftShift;
    case sf::Keyboard::Scan::LControl:
      return ImGuiKey_LeftCtrl;
    case sf::Keyboard::Scan::LAlt:
      return ImGuiKey_LeftAlt;
    case sf::Keyboard::Scan::LSystem:
      return ImGuiKey_LeftSuper;
    case sf::Keyboard::Scan::RShift:
      return ImGuiKey_RightShift;
    case sf::Keyboard::Scan::RControl:
      return ImGuiKey_RightCtrl;
    case sf::Keyboard::Scan::RAlt:
      return ImGuiKey_RightAlt;
    case sf::Keyboard::Scan::RSystem:
      return ImGuiKey_RightSuper;
    case sf::Keyboard::Scan::Menu:
      return ImGuiKey_Menu;
    case sf::Keyboard::Scan::Num0:
      return ImGuiKey_0;
    case sf::Keyboard::Scan::Num1:
      return ImGuiKey_1;
    case sf::Keyboard::Scan::Num2:
      return ImGuiKey_2;
    case sf::Keyboard::Scan::Num3:
      return ImGuiKey_3;
    case sf::Keyboard::Scan::Num4:
      return ImGuiKey_4;
    case sf::Keyboard::Scan::Num5:
      return ImGuiKey_5;
    case sf::Keyboard::Scan::Num6:
      return ImGuiKey_6;
    case sf::Keyboard::Scan::Num7:
      return ImGuiKey_7;
    case sf::Keyboard::Scan::Num8:
      return ImGuiKey_8;
    case sf::Keyboard::Scan::Num9:
      return ImGuiKey_9;
    case sf::Keyboard::Scan::A:
      return ImGuiKey_A;
    case sf::Keyboard::Scan::B:
      return ImGuiKey_B;
    case sf::Keyboard::Scan::C:
      return ImGuiKey_C;
    case sf::Keyboard::Scan::D:
      return ImGuiKey_D;
    case sf::Keyboard::Scan::E:
      return ImGuiKey_E;
    case sf::Keyboard::Scan::F:
      return ImGuiKey_F;
    case sf::Keyboard::Scan::G:
      return ImGuiKey_G;
    case sf::Keyboard::Scan::H:
      return ImGuiKey_H;
    case sf::Keyboard::Scan::I:
      return ImGuiKey_I;
    case sf::Keyboard::Scan::J:
      return ImGuiKey_J;
    case sf::Keyboard::Scan::K:
      return ImGuiKey_K;
    case sf::Keyboard::Scan::L:
      return ImGuiKey_L;
    case sf::Keyboard::Scan::M:
      return ImGuiKey_M;
    case sf::Keyboard::Scan::N:
      return ImGuiKey_N;
    case sf::Keyboard::Scan::O:
      return ImGuiKey_O;
    case sf::Keyboard::Scan::P:
      return ImGuiKey_P;
    case sf::Keyboard::Scan::Q:
      return ImGuiKey_Q;
    case sf::Keyboard::Scan::R:
      return ImGuiKey_R;
    case sf::Keyboard::Scan::S:
      return ImGuiKey_S;
    case sf::Keyboard::Scan::T:
      return ImGuiKey_T;
    case sf::Keyboard::Scan::U:
      return ImGuiKey_U;
    case sf::Keyboard::Scan::V:
      return ImGuiKey_V;
    case sf::Keyboard::Scan::W:
      return ImGuiKey_W;
    case sf::Keyboard::Scan::X:
      return ImGuiKey_X;
    case sf::Keyboard::Scan::Y:
      return ImGuiKey_Y;
    case sf::Keyboard::Scan::Z:
      return ImGuiKey_Z;
    case sf::Keyboard::Scan::F1:
      return ImGuiKey_F1;
    case sf::Keyboard::Scan::F2:
      return ImGuiKey_F2;
    case sf::Keyboard::Scan::F3:
      return ImGuiKey_F3;
    case sf::Keyboard::Scan::F4:
      return ImGuiKey_F4;
    case sf::Keyboard::Scan::F5:
      return ImGuiKey_F5;
    case sf::Keyboard::Scan::F6:
      return ImGuiKey_F6;
    case sf::Keyboard::Scan::F7:
      return ImGuiKey_F7;
    case sf::Keyboard::Scan::F8:
      return ImGuiKey_F8;
    case sf::Keyboard::Scan::F9:
      return ImGuiKey_F9;
    case sf::Keyboard::Scan::F10:
      return ImGuiKey_F10;
    case sf::Keyboard::Scan::F11:
      return ImGuiKey_F11;
    case sf::Keyboard::Scan::F12:
      return ImGuiKey_F12;
    default:
      return ImGuiKey_None;
  }
}

void ImGui::SFML::ProcessEvent(const sf::Event& event)
{
  ImGuiIO& io = ImGui::GetIO();

  // KeyPressed event
  if (auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
  {
    // Convert SFML scancode to ImGuiKey
    ImGuiKey key = SFMLScancodeToImGuiKey(keyPressed->scancode);
    io.AddKeyEvent(key, true);

    // Modifiers
    io.AddKeyEvent(ImGuiMod_Ctrl, keyPressed->control);
    io.AddKeyEvent(ImGuiMod_Shift, keyPressed->shift);
    io.AddKeyEvent(ImGuiMod_Alt, keyPressed->alt);
    io.AddKeyEvent(ImGuiMod_Super, keyPressed->system);
  }

  // KeyReleased event
  if (auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
  {
    ImGuiKey key = SFMLScancodeToImGuiKey(keyReleased->scancode);
    io.AddKeyEvent(key, false);

    // Modifiers
    io.AddKeyEvent(ImGuiMod_Ctrl, keyReleased->control);
    io.AddKeyEvent(ImGuiMod_Shift, keyReleased->shift);
    io.AddKeyEvent(ImGuiMod_Alt, keyReleased->alt);
    io.AddKeyEvent(ImGuiMod_Super, keyReleased->system);
  }

  // MouseButtonPressed event
  if (auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
  {
    int button = static_cast<int>(mousePressed->button);
    if (button >= 0 && button < 5)
    {
      io.AddMouseButtonEvent(button, true);
    }
  }

  // MouseButtonReleased event
  if (auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
  {
    int button = static_cast<int>(mouseReleased->button);
    if (button >= 0 && button < 5)
    {
      io.AddMouseButtonEvent(button, false);
    }
  }

  // MouseWheelScrolled event
  if (auto* wheelScroll = event.getIf<sf::Event::MouseWheelScrolled>())
  {
    if (wheelScroll->wheel == sf::Mouse::Wheel::Vertical)
      io.AddMouseWheelEvent(0.0f, wheelScroll->delta);
    else if (wheelScroll->wheel == sf::Mouse::Wheel::Horizontal)
      io.AddMouseWheelEvent(wheelScroll->delta, 0.0f);
  }

  // TextEntered event
  if (auto* textEntered = event.getIf<sf::Event::TextEntered>())
  {
    if (textEntered->unicode > 0 && textEntered->unicode < 0x10000)
    {
      io.AddInputCharacter(static_cast<ImWchar>(textEntered->unicode));
    }
  }
}

void ImGui::SFML::Update(const sf::Window& window, sf::Time dt)
{
  ImGuiIO& io = ImGui::GetIO();

  io.DeltaTime = dt.asSeconds();

  // Update display size in case window resized
  sf::Vector2u size = window.getSize();
  io.DisplaySize = ImVec2((float)size.x, (float)size.y);

  // Mouse position
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
  io.AddMousePosEvent((float)mousePos.x, (float)mousePos.y);

  // Mouse buttons
  for (int i = 0; i < 5; ++i)
  {
    bool down = g_MouseJustPressed[i] || sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
    io.AddMouseButtonEvent(i, down);
    g_MouseJustPressed[i] = false;
  }

  ImGui::NewFrame();
}

void ImGui::SFML::Shutdown() { ImGui::DestroyContext(); }
