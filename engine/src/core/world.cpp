#include "engine/core/world.hpp"
#include "engine/components/components.hpp"
#include "engine/components/script.hpp"
#include "engine/ecs/system.hpp"
#include <vector>
#include <algorithm>

namespace engine {

Entity World::createEntity() {

  Entity id = _nextEntity++;
  entities.push_back(id);
  return id; 
}

void World::setCameraEntity(Entity c) { _cameraE = c; }

Entity World::getCamera() { return _cameraE; }

const std::vector<Entity> &World::getEntities() { return entities; }

void World::updateSystems(float dt) { systemManager.updateAll(*this, dt); }

void World::startSystems() { systemManager.startAll(*this); }

void World::addSystem(std::shared_ptr<System> system) {
  systemManager.addSystem(system);
}
void World::addScript(uint32_t entity, ScriptPtr script) {
  ScriptComponent sc;
  script->setContext(entity, this);
  sc.script = script;

  addComponent(entity, sc);
}

void World::setParent(Entity child, Entity parent) {
  if (!hasComponent<ChildrenComponent>(parent)) {
    addComponent(parent, ChildrenComponent{});
  }

  auto &children = getComponent<ChildrenComponent>(parent).children;
  if (std::find(children.begin(), children.end(), child) == children.end()) {
    children.push_back(child);
  }

  if (!hasComponent<ParentComponent>(child)) {
    addComponent(child, ParentComponent{parent});
  } else {
    getComponent<ParentComponent>(child).parent = parent;
  }
}
void World::removeParent(Entity child) {
  if (!hasComponent<ParentComponent>(child))
    return;

  Entity parent = getComponent<ParentComponent>(child).parent;

  if (hasComponent<ChildrenComponent>(parent)) {
    auto &children = getComponent<ChildrenComponent>(parent).children;
    children.erase(std::remove(children.begin(), children.end(), child),
                   children.end());
  }

  removeComponent<ParentComponent>(child);
}
void World::removeChild(Entity parent, Entity child) {

  if (!hasComponent<ChildrenComponent>(parent)) return;

    auto& children = getComponent<ChildrenComponent>(parent).children;
    children.erase(std::remove(children.begin(), children.end(), child), children.end());

    if (hasComponent<ParentComponent>(child) &&
        getComponent<ParentComponent>(child).parent == parent) {
        removeComponent<ParentComponent>(child);
    }

}

void World::removeAllChildren(Entity parent) {
    if (!hasComponent<ChildrenComponent>(parent)) return;

    auto& children = getComponent<ChildrenComponent>(parent).children;
    for (Entity child : children) {
        if (hasComponent<ParentComponent>(child)) {
            removeComponent<ParentComponent>(child);
        }
    }
    children.clear();
}


std::vector<Entity> World::getChildren(Entity parent) {
  if(hasComponent<ChildrenComponent>(parent)){
    return getComponent<ChildrenComponent>(parent).children;

  }
  return std::vector<Entity>();
  }

} // namespace engine
