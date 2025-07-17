#include "engine/ecs/component.hpp"

namespace engine {

void ComponentSerializerRegistry::registerSerializer(
    const std::string &name, const ComponentSerializer &serializer) {
  serializers[name] = serializer;
}

bool ComponentSerializerRegistry::hasSerializer(const std::string &name) const {
  return serializers.find(name) != serializers.end();
}

const ComponentSerializer &
ComponentSerializerRegistry::getSerializer(const std::string &name) const {
  return serializers.at(name);
}

const std::unordered_map<std::string, ComponentSerializer> &
ComponentSerializerRegistry::getAll() const {
  return serializers;
}

ComponentSerializerRegistry &ComponentManager::getSerializerRegistry() {
  return serializerRegistry;
}

void ComponentManager::clearStorages() {
  for (auto &[type, storagePtr] : storages) {
    storagePtr->clear();
  }
}
} // namespace engine
