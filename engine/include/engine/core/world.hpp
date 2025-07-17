#pragma once

#include <SDL2/SDL.h>

#include <cstdint>

#include <vector>

#include "engine/components/components.hpp"
#include "engine/ecs/component.hpp"
#include "engine/ecs/system.hpp"
#include "engine/script/scriptRegistry.hpp"
 
namespace engine {

class Script;
using ScriptPtr = std::shared_ptr<Script>;

class World {

public:
  using Entity = uint32_t;

  Entity createEntity();
  void setCameraEntity(Entity c);
  Entity getCamera();
  void updateSystems(float dt);
  void addSystem(std::shared_ptr<System> system);
  void startSystems();

  void setParent(Entity child, Entity parent);
  void removeParent(Entity child);
  void removeChild(Entity parent, Entity child);
  void removeAllChildren(Entity parent);
  std::vector<Entity> getChildren(Entity parent);

  void registerDefaults();
  void clearStorages();
 
  template <typename T>
  void registerComponent(
      const std::string &name, std::function<json(World &, Entity)> to_json,
      std::function<void(World &, Entity, const json &)> from_json);

  const std::unordered_map<std::string, ComponentSerializer> &getSerializers();
  void loadScene(const std::string &filepath);
  void saveScene(const std::string &filepath);

  template <typename T> void addComponent(Entity entity, const T &component);
  template <typename T> void addComponent(Entity entity);

  template <typename T> void removeComponent(Entity entity);

  template <typename T> bool hasComponent(Entity entity);

  template <typename T> T &getComponent(Entity entity);

  template <typename... Components> std::vector<Entity> view();

  void addScript(uint32_t entity, ScriptPtr script);

  template <typename T>
  void registerScript(std::string name );

  const std::vector<Entity> &getEntities();

private:
  std::vector<Entity> entities;
  Entity _nextEntity = 1;
  Entity _cameraE = 0;
  ComponentManager componentManager;
  SystemManager systemManager;
  ScriptRegistry scriptRegistry;
};

template <typename T>
void World::registerScript(std::string name){
  scriptRegistry.registerScript<T>(name);
}


template <typename T>
void World::registerComponent(
    const std::string &name, std::function<json(World &, Entity)> to_json,
    std::function<void(World &, Entity, const json &)> from_json) {
  componentManager.registerComponent<T>(name, to_json, from_json);
}

template <typename T>
void World::addComponent(Entity entity, const T &component) {
  componentManager.getStorage<T>().add(entity, component);
}

template <>
inline void
World::addComponent<TransformComponent>(Entity e,
                                        const TransformComponent &transform) {
  componentManager.getStorage<TransformComponent>().add(e, transform);

  if (!hasComponent<GlobalTransform>(e)) {
    componentManager.getStorage<GlobalTransform>().add(
        e, GlobalTransform{Mat4::identity()});
  }
}

template <typename T> void World::removeComponent(Entity entity) {
  componentManager.getStorage<T>().remove(entity);
}

template <typename T> void World::addComponent(Entity entity) {
  componentManager.getStorage<T>().add(entity, T{});
}

template <typename T> bool World::hasComponent(Entity entity) {
  return componentManager.getStorage<T>().has(entity);
}

template <typename T> T &World::getComponent(Entity entity) {
  return componentManager.getStorage<T>().get(entity);
}
template <typename... Components> std::vector<Entity> World::view() {
  std::vector<Entity> result;
  for (Entity e : entities) {
    if ((hasComponent<Components>(e) && ...)) {
      result.push_back(e);
    }
  }
  return result;
}

} // namespace engine
