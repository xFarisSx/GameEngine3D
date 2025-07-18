#pragma once
#include "engine/assets/mesh.hpp"
#include "engine/assets/texture.hpp"

#include "engine/math/mat4.hpp"
#include "engine/math/vec3.hpp"
#include <memory>
   
namespace engine {

class Script;
using ScriptPtr = std::shared_ptr<Script>;
using Entity = uint32_t;
struct TransformComponent {
  Vec3 position = {0, 0, 0};
  Vec3 rotation = {0, 0, 0};
  Vec3 scale = {1.0f, 1.0f, 1.0f};
};

struct GlobalTransform {
  Mat4 worldMatrix{};
};

struct CameraComponent {

  float fov = M_PI / 2;
  float aspectRatio = 16.0f / 9.0f;
  float nearPlane = 1.0f;
  float farPlane = 100.0f;
};

struct CameraControllerComponent {
  float sens = 0.5f;
  float speed = 5.f;
  bool active = true;
};

struct MeshComponent {

  std::shared_ptr<Mesh> mesh;
};

struct MaterialComponent {
  Vec3 baseColor = Vec3(1.0f, 1.0f, 1.0f); // Base color (used if no texture)
  float ambient = 0.1f;
  float specular = 0.5f;
  float shininess = 32.0f;
  std::shared_ptr<Texture> texture;

  bool useTexture = false;
};

struct ScriptComponent {
  ScriptPtr script;
};

struct ParentComponent {
  Entity parent;
};

struct ChildrenComponent {
  std::vector<Entity> children;
};

} // namespace engine
