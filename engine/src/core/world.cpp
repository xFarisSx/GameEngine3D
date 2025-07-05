#include "engine/core/world.hpp"
#include <vector>
#include "engine/ecs/system.hpp"
#include "engine/components/script.hpp"
#include "engine/components/components.hpp"

namespace engine {
 
  
  Entity World::createEntity(){

    Entity id = _nextEntity++;
    entities.push_back(id);
    return id;
  }

  void World::setCameraEntity(Entity c){

    _cameraE = c;

  }

  Entity World::getCamera(){
  return _cameraE;}
 
  const std::vector<Entity>& World::getEntities(){
    
    return entities;
    
  }

  void World::updateSystems() {
        systemManager.updateAll(*this);
    }

    void World::addSystem(std::shared_ptr<System> system) {
        systemManager.addSystem(system);
    }
void World::addScript(uint32_t entity, ScriptPtr script) {
  ScriptComponent sc;
  script->setContext(entity, this);
  sc.script = script;

  addComponent(entity, sc);
    
}

}    
 
