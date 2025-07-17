#include "engine/engine.hpp"
#include "engine/components/components.hpp"
#include "engine/systems/systems.hpp"

namespace engine {

Engine::Engine() {}

Engine::~Engine() { shutdown(); }

void Engine::init(int width, int height, const char *title) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
    exit(1);
  }
  _world = World();
  renderer = new Renderer(width, height, title);
  controller = new Controller();
  inputManager = InputManager();

 
  _world.registerDefaults(); 

  _world.addSystem(std::make_shared<RenderSystem>(renderer));
  _world.addSystem(std::make_shared<ScriptSystem>());
  CameraControllerSystem cameraControllerSystem{controller};
  _world.addSystem(
    std::make_shared<CameraControllerSystem>(cameraControllerSystem));
  _world.addSystem(std::make_shared<HierarchySystem>());
 
  

  std::cout << "Engine initialized\n";
}

void Engine::run() {
  std::cout << "Running engine loop\n";

  _world.startSystems();

  float dt = 0.0f;
  auto lastTime = std::chrono::high_resolution_clock::now();
  while (_running) {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = now - lastTime;
    dt = delta.count();
    lastTime = now;

    inputManager.pollEvents(_running, controller);
    if (!_running)
      break;

    renderer->clear();

    _world.updateSystems(dt);

    renderer->present();
  }
}

void Engine::shutdown() {
  _running = false;
  delete controller;
  controller = nullptr;
  delete renderer;
  renderer = nullptr;
  SDL_Quit();
}
} // namespace engine
