#pragma once
#include "engine/components/components.hpp"
#include "engine/math/mat4.hpp"
#include "engine/math/vec3.hpp"
#include <cmath>

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

inline Vec3 extractPosition(const Mat4 &m) {
  return Vec3(m[3][0], m[3][1], m[3][2]);
}

inline Vec3 extractScale(const Mat4 &m) {

  Vec3 right(m[0][0], m[1][0], m[2][0]);
  Vec3 up(m[0][1], m[1][1], m[2][1]);
  Vec3 forward(m[0][2], m[1][2], m[2][2]);
  return Vec3(right.length(), up.length(), forward.length());
}

inline Vec3 extractRotationEuler(const Mat4 &m) {
  Vec3 scale = extractScale(m);
  Mat4 norm = m;

  // Normalize rows to remove scale
  for (int col = 0; col < 4; ++col) {
    norm[col][0] /= scale.x; // row 0 (right)
    norm[col][1] /= scale.y; // row 1 (up)
    norm[col][2] /= scale.z; // row 2 (forward)
  }

  float pitch = std::asin(-norm[2][1]);            // -m[col=2][row=1]
  float yaw = std::atan2(norm[2][0], norm[2][2]);  // m[2][0], m[2][2]
  float roll = std::atan2(norm[0][1], norm[1][1]); // m[0][1], m[1][1]

  return Vec3(pitch, yaw, roll);
}

inline TransformComponent transformFromMatrix(const Mat4 &m) {
  TransformComponent t;
  t.position = extractPosition(m);
  t.scale = extractScale(m);
  t.rotation = extractRotationEuler(m);
  return t;
}

} // namespace engine::math
