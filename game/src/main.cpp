#include <engine/engine.hpp>


int main(){
    engine::Engine engine;


    engine.init(1600, 900, "My Game");

    auto& world = engine.world();

    engine::Entity cat = world.createEntity();
    engine::TransformComponent tc{engine::Vec3(), engine::Vec3(), engine::Vec3()};
    world.addComponent<engine::TransformComponent>(cat, tc);
  
 
    engine.run();
    engine.shutdown();
 
 
    return 0;
}     
    
