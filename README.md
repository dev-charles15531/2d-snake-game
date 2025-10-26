# 2D Snake Game (OpenGL + SFML)

**Compact, modern C++ implementation of the classic Snake game, rendered with OpenGL and driven with SFML for windowing/events.**

This repository is a focused learning project built after completing the *Getting Started* chapter on LearnOpenGL. It demonstrates a working 2D game engine pipeline, clean separation between rendering and game logic, and practical use of modern C++ idioms (smart pointers, RAII, clear ownership).

---

## Demo / Elevator pitch (for recruiters)

I built a small but complete real-time 2D Snake game using OpenGL for rendering and SFML for windowing/input. The project shows hands-on experience with the OpenGL rendering pipeline (VAO/VBO/EBO, shaders, orthographic projection), GUI integration (ImGui + SFML), and robust C++ architecture (unique_ptr, clear separation of concerns, minimal global state). It’s intentionally compact so the codebase is easy to read and extend.

**Skills demonstrated:** Modern C++ (smart pointers, RAII), realtime graphics (OpenGL 4.x), shader authoring, game loop design, GUI integration (ImGui), event-driven design, and pragmatic engineering (clear file layout and modular components).

## 🎮 Gameplay Preview
https://github.com/user-attachments/assets/924754b5-3ddc-45f1-96a6-6fcdcea18d01

---

## Features

* Clean separation between rendering (`RenderEngine`) and game logic (`Game`, `Snake`, `Food`, `BigFood`).
* OpenGL rendering with VAO/VBO/EBO and simple vertex/fragment shaders.
* ImGui-based HUD / pause menu / game-over UI integrated via `ImGui::SFML` + `ImGui_ImplOpenGL3`.
* Scalable logical grid that adapts to window aspect ratio (`GridInfo`).
* Normal food (1 cell) and big food (2x2) with a timed UI progress bar.
* Wrap-around movement (edges mirror the snake to the opposite side).
* Difficulty presets + custom speed slider.
* Minimal external dependencies (SFML, GLAD, GLM, ImGui, OpenGL).

---

## Project layout (important files)

```
/include
  ├─ big_food.hpp         # BigFood class (timed 2x2 food)
  ├─ food.hpp             # Food class (position generation + draw)
  ├─ game.hpp             # Game orchestration, menus, HUD
  ├─ gui.hpp              # ImGui wrapper
  ├─ header.hpp           # Common types: Cell, GridInfo, scaleFactor
  ├─ render_engine.hpp    # OpenGL setup, VAO/VBO/EBO, event dispatch
  ├─ shader.hpp           # Simple shader loader / uniform helpers
  └─ snake.hpp            # Snake logic + controls

/src
  ├─ big_food.cpp
  ├─ food.cpp
  ├─ game.cpp
  ├─ gui.cpp
  ├─ render_engine.cpp
  ├─ shader.cpp
  └─ snake.cpp

/CMakeLists.txt
/README.md
```

---

## Build & Run

### Dependencies

* C++17 (recommended: GCC 9+, Clang 10+, MSVC 2019+)
* CMake (3.10+)
* SFML (3)
* OpenGL (core profile 4.4 or compatible)
* GLAD (included)
* GLM (included)
* ImGui (included minimal SFML + OpenGL backend)

> The project includes bundled copies of GLAD, GLM, and ImGui headers. You still need SFML and a working OpenGL context on your platform.

### Quick build (Linux/macOS)

```bash
git clone <your-repo>
cd snake-game-2d
mkdir build && cd build
cmake ..
make -j$(nproc)
./bin/main    # run location determines how shader loads
```

On Windows use your preferred CMake generator (Visual Studio / Ninja) and ensure SFML dev libraries are available.

---

## Controls

* Arrow keys — Move snake
* Space or Esc — Pause / Resume
* R — Reset game (when paused)
* Use Pause menu to change difficulty or set a custom snake speed

---

## Design notes & code highlights

### Grid and coordinate system

* `GridInfo` calculates logical grid size based on a `baseSize` and the current screen aspect ratio. This lets rendering and game logic operate in grid coordinates (1 world unit = 1 cell), while shaders use an orthographic projection.

### Rendering

* `RenderEngine` sets up a reusable quad (VAO/VBO/EBO) and draws everything by applying model transforms per object (snake segments, food cells).
* Shaders are loaded via a small `Shader` helper class that compiles & links GLSL files and exposes uniform setters.

### Game logic

* `Snake` encapsulates movement, collision detection, growth, and input handling.
* `Food` and `BigFood` handle spawn logic; normal food spawns as a single Cell, `BigFood` spawns as a 2x2 cluster and has a lifetime with a visible ImGui progress bar.
* `Game` coordinates the loop, menus, score/highscore, and interactions between components.

### Event dispatch pattern

* `RenderEngine` exposes an `addEventListener` method and stores callbacks. `Game` attaches a listener to receive SFML events. This keeps render + event plumbing separate from game logic.

### Memory & ownership

* `unique_ptr` is used consistently for heap-managed singletons (snake, food, GUI, render engine, shader). `BigFood` is spawned on-demand via `std::make_unique` and handed to `RenderEngine` as a raw pointer for drawing, which keeps ownership in the `Game` side.

---

## Known issues & honest caveats (read this before you tinker)

I don’t sugarcoat, here’s what’s rough and what you should expect:

* **Edge cases for big food spawn**: `TODO` comment in code notes big food might spawn on top of normal food. That needs a collision check when spawning.
* **High score persistence**: high score is only in-memory, there’s a TODO to persist it to disk.
* **Magic numbers and hard-coded shader paths**: shader paths are relative (`../src/shaders/...`) which may break if you run the binary from another working directory. Make them configurable or package shaders alongside the binary.
* **No automated tests**: this is a small demo app; adding unit tests for grid calculations / spawn logic would make the repo more production-ready.
* **Resource cleanup depends on window context**: `RenderEngine::terminate()` checks `window.isOpen()` before releasing GL resources; double-check destruction order on application shutdown.

These are intentional trade-offs; it’s a learning project, not a shipped AAA game. But they’re quick fixes if you want to polish for contribution sake or anything.

---

## License

```
MIT License
(c) Mys3
```

---

Built after reading the [*Getting Started*](https://learnopengl.com/Getting-started) chapter on [LearnOpenGL.com](https://learnopengl.com).
