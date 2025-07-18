#include "engine/core/gameObject.hpp"
#include "engine/components/components.hpp"

namespace engine { 

GameObject::GameObject(World& w)
  : world(w), entity(w.createEntity()) {
  
  addComponent<TransformComponent>();

}

GameObject::GameObject(World& w, Entity existing)
  : world(w), entity(existing) {}

Entity GameObject::getEntity() const {
  return entity;
}

void GameObject::setMesh(const std::shared_ptr<Mesh>& mesh) {
  MeshComponent m;
  m.mesh = mesh;
  if (world.hasComponent<MeshComponent>(entity)) {
    world.getComponent<MeshComponent>(entity) = m;
  } else {
    world.addComponent<MeshComponent>(entity, m);
  }
}

void GameObject::setMaterial(const MaterialComponent& material) {
  if (world.hasComponent<MaterialComponent>(entity)) {
    world.getComponent<MaterialComponent>(entity) = material;
  } else {
    world.addComponent<MaterialComponent>(entity, material);
  }
}

void GameObject::addScript(const std::shared_ptr<Script>& script) {
  world.addScript(entity, script);
}

void GameObject::setParent(const GameObject& parent) {
  world.setParent(entity, parent.getEntity());
}

void GameObject::removeParent() {
  world.removeParent(entity);
}

std::vector<GameObject> GameObject::getChildren() const {
  std::vector<Entity> children = world.getChildren(entity);
  std::vector<GameObject> result;
  result.reserve(children.size());
  for (Entity e : children) {
    result.emplace_back(world, e);
  }
  return result;
}

} // namespace engine

