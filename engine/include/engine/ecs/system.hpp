#pragma once


#include <memory>


#include <vector>



namespace engine {

class World;
class System {
  public:
      virtual void start(World& world) = 0;
      virtual void update(World& world) = 0; 
      virtual ~System() = default;
};

class SystemManager {
  public:
      void addSystem(std::shared_ptr<System> system);
      void updateAll(World& world);
      void startAll(World& world);

  private:
      std::vector<std::shared_ptr<System>> systems;
  };

}
    
