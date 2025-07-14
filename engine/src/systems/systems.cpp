#include "engine/systems/systems.hpp"

#include "engine/components/components.hpp"
#include "engine/components/script.hpp"
#include "engine/core/world.hpp"
#include "engine/ecs/system.hpp"
#include "engine/math/general.hpp"
#include "engine/math/mat4.hpp"
#include "engine/renderer/renderer.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
namespace engine {
using Entity = uint32_t;

void RenderSystem::update(World &world, float dt) {
  const auto &entities = world.getEntities();

  Entity cameraEntity = world.getCamera();
  if (!cameraEntity)
    return;

  auto &cameraGM =
      world.getComponent<GlobalTransform>(cameraEntity).worldMatrix;
  auto &camera = world.getComponent<CameraComponent>(cameraEntity);

  TransformComponent cameraGlobalT = math::transformFromMatrix(cameraGM);
  for (Entity entity : entities) {
    if (world.hasComponent<GlobalTransform>(entity) &&
        world.hasComponent<MeshComponent>(entity) &&
        world.hasComponent<MaterialComponent>(entity)) {

      Mat4 &globalMat = world.getComponent<GlobalTransform>(entity).worldMatrix;
      auto &meshC = world.getComponent<MeshComponent>(entity);
      if (!meshC.mesh)
        continue;
      auto &material = world.getComponent<MaterialComponent>(entity);

      renderer->renderMesh(meshC.mesh.get(), globalMat, cameraGlobalT, camera,
                           material);
    }
  }
}

void ScriptSystem::update(World &world, float dt) {
  const auto &entities = world.getEntities();
  for (Entity entity : entities) {
    if (world.hasComponent<ScriptComponent>(entity)) {
      auto &sc = world.getComponent<ScriptComponent>(entity);
      sc.script->update(dt);
    }
  }
}

void ScriptSystem::start(World &world) {

  auto &entities = world.getEntities();
  for (Entity entity : entities) {
    if (world.hasComponent<ScriptComponent>(entity)) {
      auto &sc = world.getComponent<ScriptComponent>(entity);
      sc.script->start();
    }
  }
}

void processEntity(World &world, Entity e, const Mat4 &parentMatrix) {
  auto &local = world.getComponent<TransformComponent>(e);
  Mat4 localMat = Mat4::modelMatrix(local);

  Mat4 globalMat = parentMatrix * localMat;
  world.getComponent<GlobalTransform>(e).worldMatrix = globalMat;

  for (Entity child : world.getChildren(e)) {
    processEntity(world, child, globalMat);
  }
}

void HierarchySystem::update(World &world, float dt) {
  for (Entity e : world.getEntities()) {
    if (!world.hasComponent<ParentComponent>(e)) {
      // This is a root node
      processEntity(world, e, Mat4::identity());
    }
  }
}

void CameraControllerSystem::update(World &world, float dt) {
  if (!controller)
    return;
  Vec3 moveDir{};
  if (controller->isKeyPressed(Key::W))
    moveDir.z += 1;

  if (controller->isKeyPressed(Key::S))
    moveDir.z -= 1;
  if (controller->isKeyPressed(Key::A))
    moveDir.x -= 1;

  if (controller->isKeyPressed(Key::D))
    moveDir.x += 1;

  if (controller->isKeyPressed(Key::E))
    moveDir.y += 1;

  if (controller->isKeyPressed(Key::Q))
    moveDir.y -= 1;

  moveDir = moveDir.normalized();

  for (Entity e : world.view<CameraControllerComponent, TransformComponent,
                             CameraComponent>()) {

    auto &transform = world.getComponent<TransformComponent>(e);
    auto &cameraC = world.getComponent<CameraControllerComponent>(e);

    Vec3 forward, right, up;
    math::updateCameraBasis(transform.rotation, forward, right, up);

    transform.position =
        transform.position +
        (forward * moveDir.z + right * moveDir.x + up * moveDir.y) *
            cameraC.speed * dt;

    if (controller->rightClick) {

      transform.rotation.y += controller->dx * cameraC.sens * dt;

      transform.rotation.x -= controller->dy * cameraC.sens * dt;
      transform.rotation.x = std::clamp(transform.rotation.x, -1.5f, 1.5f);
    }
  }
  controller->dx = 0;
  controller->dy = 0;
}

} // namespace engine
