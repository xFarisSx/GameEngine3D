
#include "engine/math/vec3.hpp"
#include <cmath>

namespace engine {

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

Vec3 Vec3::operator+(const Vec3 &v) const {
  return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3 &v) const {
  return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator*(float s) const { return Vec3(x * s, y * s, z * s); }

float Vec3::dot(const Vec3 &v) const { return x * v.x + y * v.y + z * v.z; }

Vec3 Vec3::cross(const Vec3 &v) const {
  return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

float Vec3::length() const { return std::sqrt(x * x + y * y + z * z); }

Vec3 Vec3::normalized() const {
  float len = length();
  if (len == 0)
    return Vec3(0, 0, 0);
  return Vec3(x / len, y / len, z / len);
}
} // namespace engine
