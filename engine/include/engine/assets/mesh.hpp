#pragma once
#include "engine/math/vec3.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

namespace engine {

struct Triangle {
  int i0, i1, i2;
  int uv0, uv1, uv2 = 0;
};

struct Mesh {
  std::vector<Vec3> vertices;
  std::vector<Triangle> triangles;
  std::vector<Vec3> textureMap;

  static std::shared_ptr<Mesh> createCube();
  static std::shared_ptr<Mesh> loadFromObj(const std::string &filename);
};
} // namespace engine
