#pragma once
#include "engine/assets/mesh.hpp"
#include "engine/components/script.hpp"
#include "engine/math/vec3.hpp"

#include <memory>

namespace engine {

struct TransformComponent {
  Vec3 position = {0, 0, 0};
  Vec3 rotation = {0, 0, 0};
  Vec3 scale = {1.0f, 1.0f, 1.0f};
};

struct CameraComponent {

  float fov = M_PI / 2;
  float aspectRatio = 16.0f / 9.0f;
  float nearPlane = 1.0f;
  float farPlane = 100.0f;

  double speed = 2;
  double sensitivity = 0.01;
};

struct MeshComponent {

  std::shared_ptr<Mesh> mesh;
};

struct ScriptComponent {
  ScriptPtr script;
};

} // namespace engine
