#include "engine/serialization/serializer.hpp"
#include "engine/core/world.hpp"
#include "engine/thirdparty/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

namespace engine {

void Serializer::saveScene(World &world, const std::string &filepath) {
  json scene;
  scene["activeCamera"] = world.getCamera();
  scene["entities"] = json::array();

  for (Entity e : world.getEntities()) {
    json entityJson;
    entityJson["id"] = static_cast<uint32_t>(e);
    json componentsJson;

    for (const auto &[componentName, serializer] : world.getSerializers()) {
      if (serializer.has(world, e)) {
        componentsJson[componentName] = serializer.to_json(world, e);
      }
    }

    entityJson["components"] = componentsJson;

    scene["entities"].push_back(entityJson);
  }

  std::ofstream out(filepath);
  out << scene.dump(2);
}

void Serializer::loadScene(World &world, const std::string &filepath) {
  std::ifstream in(filepath);
  json scene;
  in >> scene;
  auto &serializers = world.getSerializers();

  world.clearStorages();

  for (const auto &entityJson : scene["entities"]) {
    Entity e = world.createEntity();

    const auto &componentsJson = entityJson["components"];
    for (const auto &[componentName, componentData] : componentsJson.items()) {
      auto it = serializers.find(componentName);
      if (it != serializers.end()) {

        it->second.from_json(world, e, componentData);
      }
    }
  }
  world.setCameraEntity(scene["activeCamera"]);
}

} // namespace engine
