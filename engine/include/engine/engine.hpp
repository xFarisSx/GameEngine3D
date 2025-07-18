#pragma once
#include "engine/assets/mesh.hpp"
#include "engine/components/components.hpp"
#include "engine/core/world.hpp"
#include "engine/ecs/component.hpp"
#include "engine/ecs/system.hpp"
#include "engine/engineContext.hpp"
#include "engine/input/controller.hpp"
#include "engine/input/inputManager.hpp"
#include "engine/math/vec3.hpp"
#include "engine/renderer/renderer.hpp"
#include "engine/script/script.hpp"
#include "engine/systems/systems.hpp"
#include "engine/core/gameObject.hpp"
#include <SDL2/SDL.h>
#include <chrono>

namespace engine {
using Entity = uint32_t;
class Engine {
public:
  Engine();
  ~Engine();

  void init(int width, int height, const char *title);
  void run();
  void shutdown();

  World &world() { return _world; }
  const World &world() const { return _world; }

private:
  World _world;
  Renderer *renderer;
  Controller *controller = nullptr;
  InputManager inputManager;
  EngineContext *context;
  bool _running = true;
};
} // namespace engine
