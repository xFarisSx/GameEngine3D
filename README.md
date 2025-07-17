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
You can define custom:
    -Components (docs/components.md)
    -Systems (that iterate over ECS components)(docs/systems.md)
    -Scripts (by inheriting from Script)

---

## Example

```cpp
#include <engine/engine.hpp>
#include <engine/components/components.hpp>
using namespace engine;

class Rotator : public Script {
  TransformComponent* transform;
  void start() override {
    transform = &getComponent<TransformComponent>();
  }
  void update(float dt) override {
    transform->rotation.y += dt;
  }
};

int main() {
  Engine engine;
  engine.init(1600, 900, "My Game");

  auto& world = engine.world();
  Entity cam = world.createEntity();
  world.addComponent<TransformComponent>(cam, {Vec3(0, 0, -5)});
  world.addComponent<CameraComponent>(cam, {M_PI / 2, 16.f/9.f, 0.1f, 100.f});
  world.addComponent<CameraControllerComponent>(cam);
  world.setCameraEntity(cam);

  Entity model = world.createEntity();
  world.addComponent<TransformComponent>(model, {Vec3(0), Vec3(0), Vec3(0.01f)});
  world.addComponent<MeshComponent>(model, {Mesh::loadFromObj("assets/models/cat.obj")});

  auto tex = Texture::loadFromBmp("assets/textures/textcat1.bmp");
  MaterialComponent mat{};
  mat.texture = tex;
  mat.useTexture = true;
  world.addComponent<MaterialComponent>(model, mat);

  world.addScript(model, std::make_shared<Rotator>());

  engine.run();
  engine.shutdown();
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


