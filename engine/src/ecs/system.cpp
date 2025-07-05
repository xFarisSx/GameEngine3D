#include "engine/ecs/system.hpp"
#include "engine/core/world.hpp"
namespace engine {

void SystemManager::addSystem(std::shared_ptr<System> system) {
    systems.push_back(system);
}

void SystemManager::updateAll(World& world) {
    for (auto& system : systems) {
        system->update(world);
    }
}

}  
