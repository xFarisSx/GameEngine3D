#include "engine/engine.hpp"
#include "engine/renderer/renderer.hpp"
#include "engine/input/controller.hpp"
#include <iostream>
#include "engine/systems/systems.hpp"
#include "engine/components/components.hpp"
#include "engine/components/script.hpp"

namespace engine {

  Engine::Engine(){}

Engine::~Engine() {
    shutdown(); 
} 
 
void Engine::init(int width, int height, const char* title) {
    _world = World();
    renderer = new Renderer(width, height, title);
    controller = Controller();

    _world.registerComponent<TransformComponent>();
    _world.registerComponent<CameraComponent>();
    _world.registerComponent<MeshComponent>();
    _world.registerComponent<ScriptComponent>();

    RenderSystem renderSystem = RenderSystem(renderer);
    _world.addSystem(std::make_shared<RenderSystem>(renderSystem));
     ScriptSystem scriptSystem = ScriptSystem();
    _world.addSystem(std::make_shared<ScriptSystem>(scriptSystem));

    

    
    std::cout << "Engine initialized\n";
}
 
void Engine::run() {
    std::cout << "Running engine loop\n";
    _world.startSystems();
    while (_running) {
        renderer->handleEvents(_running, controller);
        renderer->clear();

        _world.updateSystems();
        
        renderer->present(); 
    }
}
  
void Engine::shutdown() {
  _running = false;
  
 
   

    std::cout << "Engine shut down\n";
}
}  
