#pragma once
#include <cmath>
#include "engine/math/vec3.hpp"

namespace engine::math {
inline void updateCameraBasis(const Vec3 &rotation, Vec3 &forward, Vec3 &right,
                              Vec3 &up, const Vec3 &worldUp = Vec3(0, 1, 0)) {
  float yaw = rotation.y;
  float pitch = rotation.x;

  forward.x = std::cos(pitch) * std::sin(yaw);
  forward.y = std::sin(pitch);
  forward.z = std::cos(pitch) * std::cos(yaw);
  forward = forward.normalized();

  right = worldUp.cross(forward);
  right = right.normalized();

  up = forward.cross(right);
  up = up.normalized();
}
}
