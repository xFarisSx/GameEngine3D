# Faris Engine (WIP)

A lightweight C++17 game engine using SDL2.  
Features ECS, 3D software rendering, OBJ mesh loading, textures, and scripting, and a hierarchy system.

---

## Requirements

- C++17 compiler  
- SDL2 development libraries  

### Fedora:
```sh
sudo dnf install SDL2-devel
```

### Ubuntu:
```sh
sudo apt install libsdl2-dev
```

---

## Build

```sh
make
```

Outputs:  
- `build/engine/libengine.a`

---

## Install / Uninstall

```sh
sudo make install     # installs to /usr/local/
sudo make uninstall   # removes installed files
```

---

## Core Features

- Entity-Component-System (ECS)
- Software 3D rendering (OBJ, BMP)
- Lighting & texture support
- Scripting system (`start()` / `update()`)
- Hierarchy system with `ParentComponent`, `ChildrenComponent`, and `GlobalTransform` to handle parent-child relationships and global matrices
- Easy to extend with custom logic
- Scene serialization and deserialization (JSON)
- Component registration and storage management

---

## Components

- `TransformComponent` — position, rotation, and scale 
- `GlobalTransform` — global/world matrix calculated via hierarchy system
- `ParentComponent` — reference to parent entity
- `ChildrenComponent` — list of child entities
- `MeshComponent` — holds a shared mesh reference 
- `MaterialComponent` — texture and lighting parameters 
- `CameraComponent` — FOV, aspect ratio, near/far planes 
- `CameraControllerComponent` — enables movement control 
- `ScriptComponent` — attaches logic via script classes 

---

## Systems

- `RenderSystem` — draws all mesh entities using the active camera 
- `ScriptSystem` — calls `start()` once, then `update(dt)` every frame 
- `HierarchySystem` — updates global transforms based on parent-child hierarchy
- `CameraControllerSystem` — basic WASD + mouse camera movement 

---

## Extensibility

The engine allows custom components and systems.
You can register your own types and logic to extend behavior.
The engine's ECS model ensures that user-defined systems can integrate smoothly into the update loop.
- [Components](docs/components.md)
> ℹ️ **Note:** When registering a component, you must provide a serializer and deserializer using `registerComponent`. See the components guide for examples.
- [Systems](docs/systems.md) (that iterate over ECS components)
- [Scripts](docs/scripts.md) (by inheriting from `Script`)

---

## Example

```cpp
#include <engine/engine.hpp>
#include <iostream>

using namespace engine;

// Example Script: Rotator
class Rotator : public Script {
  TransformComponent* transform = nullptr;
  EngineContext* ctx = nullptr;

public:
  Rotator() : Script("Rotator") {}

  void start() override {
    // Get the GameObject wrapper for this script's entity
    GameObject obj = getGameObject();
    transform = &obj.getComponent<TransformComponent>();

    // Get the engine context (for input, etc)
    ctx = world->getContext();
  }

  void update(float dt) override {
    if (ctx && ctx->controller->isKeyPressed(Key::Escape)) {
      // Rotate around Y axis continuously while Escape key is pressed
      transform->rotation.y += dt;
    }
  }
};

// Setup scene manually
void setupScene(World& world) {
  // Create a box GameObject
  GameObject box(world);
  box.getComponent<TransformComponent>().position = Vec3(4, 2, 1);
  box.setMesh(Mesh::createBox(1.0, 2.0, 1.0));

  MaterialComponent boxMaterial;
  boxMaterial.useTexture = false;
  box.setMaterial(boxMaterial);

  // Create a camera GameObject
  GameObject camera(world);
  auto& camTransform = camera.getComponent<TransformComponent>();
  camTransform.position = Vec3(0, 0, -5);
  camTransform.scale = Vec3(100.0f);

  camera.addComponent(CameraComponent{M_PI / 2, 16.f / 9.f, 1.0f, 30.f});
  camera.addComponent(CameraControllerComponent{});
  world.setCameraEntity(camera.getEntity());

  // Create a 3D model GameObject
  GameObject model(world);
  model.getComponent<TransformComponent>().scale = Vec3(0.01f);
  model.setMesh(Mesh::loadFromObj("assets/models/cat.obj"));

  MaterialComponent modelMat;
  modelMat.texture = Texture::loadFromBmp("assets/textures/textcat1.bmp");
  modelMat.useTexture = true;
  model.setMaterial(modelMat);

  model.addScript(std::make_shared<Rotator>());

  // Create a sphere GameObject with default material
  GameObject sphere(world);
  sphere.setMesh(Mesh::createSphere(1, 16, 32));
  sphere.setMaterial(MaterialComponent{});
}

int main() {
  Engine engine;
  engine.init(1600, 900, "My Game");

  auto& world = engine.world();

  // Register scripts
  world.registerScript<Rotator>("Rotator");

  // Option 1: Setup scene programmatically
  // setupScene(world);

  // Option 2: Load scene from JSON file
  world.loadScene("scenes/test.json");

  engine.run();
  engine.shutdown();

  return 0;
}

```

---

## Compile Your Game

```sh
g++ main.cpp -std=c++17 -I/usr/local/include -L/usr/local/lib -lengine `sdl2-config --cflags --libs` -o game
```

---

## Makefile Commands

- `make` → build static library  
- `make clean` → remove build files  
- `sudo make install` → install headers + lib  
- `sudo make uninstall` → remove them  

---

## Notes

- Only `.obj` files (no .mtl/normals yet) 
- Only `.bmp` textures supported 
- Materials include lighting factors and optional texture
- Software rendering only (OpenGL not yet implemented) 
- Some OBJ files may fail — loader is experimental 


