#pragma once

#include <SDL2/SDL.h>

#include <cstdint>

#include <vector>


#include "engine/ecs/component.hpp"

namespace engine {

  struct System;

  class World{

    public:
      using Entity = uint32_t;
      

      World();

            
    Entity createEntity();

    template<typename T>
    void registerComponent();

    template<typename T>
    void addComponent(Entity entity, const T& component); 

    template<typename T>
    bool hasComponent(Entity entity); 

    template<typename T>
    T& getComponent(Entity entity);

    const std::vector<Entity>& getEntities(); 

    
    private:
      std::vector<Entity> entities;
      Entity _nextEntity = 0;
      ComponentManager componentManager; 

  };

}
