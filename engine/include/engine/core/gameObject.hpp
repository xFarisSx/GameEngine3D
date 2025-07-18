
#pragma once

#include "engine/assets/material.hpp"
#include "engine/assets/mesh.hpp"
#include "engine/components/components.hpp"
#include "engine/core/world.hpp"

namespace engine {

class GameObject {
  World &world;
  Entity entity;

public:
  GameObject() = default;
  GameObject(World &w);
  GameObject(World &w, Entity existing);


  Entity getEntity() const;

  template <typename T> void addComponent(const T &component);
  template <typename T> void addComponent();

  template <typename T> T &getComponent();

  template <typename T> bool hasComponent();

  template <typename T> void removeComponent();

  void setMesh(const std::shared_ptr<Mesh> &mesh);
  void setMaterial(const MaterialComponent &material);
  void addScript(const std::shared_ptr<Script> &script);

  void setParent(const GameObject &parent);
  void removeParent();
  std::vector<GameObject> getChildren() const;
};

template <typename T> void GameObject::addComponent(const T &component) {
  world.addComponent<T>(entity, component);
}
template <typename T> void GameObject::addComponent() {
  world.addComponent<T>(entity);
}
template <typename T> T &GameObject::getComponent() {
  return world.getComponent<T>(entity);
}

template <typename T> bool GameObject::hasComponent() {
  return world.hasComponent<T>(entity);
}

template <typename T> void GameObject::removeComponent() {
  world.removeComponent<T>(entity);
}
} // namespace engine
