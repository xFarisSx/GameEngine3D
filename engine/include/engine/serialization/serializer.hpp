#pragma once
#include "engine/thirdparty/nlohmann/json.hpp"
#include "engine/math/mat4.hpp"
#include "engine/math/vec3.hpp"
#include <string>

using json = nlohmann::json;

namespace engine {


inline void to_json(nlohmann::json& j, const Vec3& v) {
    j = nlohmann::json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}
inline void from_json(const nlohmann::json& j, Vec3& v) {
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
    j.at("z").get_to(v.z);
}
inline void to_json(nlohmann::json& j, const Mat4& m) {
    j = nlohmann::json::array();
    for (int row = 0; row < 4; ++row) {
        nlohmann::json rowArr = nlohmann::json::array();
        for (int col = 0; col < 4; ++col) {
            rowArr.push_back(m[row][col]);
        }
        j.push_back(rowArr);
    }
}
inline void from_json(const nlohmann::json& j, Mat4& m) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            m[row][col] = j.at(row).at(col).get<float>();
        }
    }
}

// Mat4 serialization
class World;

class Serializer {
public:
  static void saveScene(World &world, const std::string &filepath);
  static void loadScene(World &world, const std::string &filepath);
};

} // namespace engine
