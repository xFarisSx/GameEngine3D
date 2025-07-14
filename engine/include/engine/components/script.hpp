#pragma once
#include <SDL2/SDL.h>
#include <memory>

#include "engine/core/world.hpp"

namespace engine {

class Script {
protected:
  uint32_t entityId = 0;
  World *world = nullptr;

public:
  virtual ~Script() = default;

  virtual void start() {}

  virtual void update(float dt) {}

  void setContext(uint32_t id, World *w);

  template <typename T> T &getComponent();
};

template <typename T> T &Script::getComponent() {
  return world->template getComponent<T>(entityId);
}

using ScriptPtr = std::shared_ptr<Script>;

} // namespace engine
