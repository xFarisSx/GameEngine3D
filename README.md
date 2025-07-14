
# Faris Engine - (under development)

A lightweight C++ game engine built with SDL2.  
Includes: ECS, camera, script system, 3D software renderer, mesh/texture support.

---

## Requirements

- C++17 compiler  
- SDL2 development package  

Fedora:  
    sudo dnf install SDL2-devel  

Ubuntu:  
    sudo apt install libsdl2-dev  

---

## Build

    make

Outputs:  
    build/engine/libengine.a

---

## Install

    sudo make install

Copies:  
    libengine.a → /usr/local/lib/  
    headers     → /usr/local/include/engine/

---

## Uninstall

    sudo make uninstall

Deletes:  
    /usr/local/lib/libengine.a  
    /usr/local/include/engine/

---

## Available Components

- TransformComponent — position, rotation, scale  
- MeshComponent      — shared_ptr to mesh + texture data  
- CameraComponent    — field of view, aspect ratio, near/far planes  
- ScriptComponent    — holds a shared_ptr to user scripts  

---

## Available Systems

- RenderSystem — draws mesh components using camera  
- ScriptSystem — calls `start()` and `update()` on all scripts  

---

## Extensibility: User-Defined Components and Systems

The engine supports **registration of user-defined components and systems**, allowing you to extend the ECS with your own logic and data types. You can create custom components and systems that integrate seamlessly with the engine's update and rendering loops.

This makes the engine flexible for a wide variety of game mechanics beyond the built-in systems.

---

## Supported Model Formats

- Only supports `.obj` files for now  
- Limited feature support (e.g. no normals, no materials)  
- Some `.obj` files may fail or break the loader — still experimental  

---

## Example Usage
```cpp

#include <SDL2/SDL.h>
#include <cmath>
#include <engine/components/components.hpp>
#include <engine/engine.hpp>
#include <iostream>
#include <memory>

using namespace engine;

class OBJController : public Script {
public:
  float speed = 0.1f;
  TransformComponent *transform;

  void start() override { 
    transform = &getComponent<TransformComponent>();
  }

  void update(float dt) override {
    
    //transform->rotation.x+=speed*dt;

  }
};
 
int main() {
  Engine engine;
  engine.init(1600, 900, "My Game");

  auto &world = engine.world();

  Entity test = world.createEntity();

  world.addComponent<TransformComponent>(
      test, TransformComponent{Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(0.01, 0.01, 0.01)});

  Entity cam = world.createEntity();
  world.addComponent<TransformComponent>(
      cam, TransformComponent{Vec3(0, 0, -5), Vec3(0, 0, 0), Vec3(1, 1, 1)});

  world.addComponent<CameraComponent>(
      cam, CameraComponent{M_PI / 2, 16.0f / 9.0f, 1.f, 150.0f});

  world.addComponent(cam, CameraControllerComponent{});
  world.setCameraEntity(cam);



  std::shared_ptr<Mesh> meshPtr = Mesh::loadFromObj("assets/models/cat.obj");

  std::shared_ptr<Texture> texturePtr = Texture::loadFromBmp("assets/textures/textcat1.bmp");

  MaterialComponent mat{};
  mat.texture = texturePtr;
  mat.useTexture = true;

  world.addComponent(test, mat);

  world.addComponent<MeshComponent>(test, MeshComponent{meshPtr});

  world.addScript(test, std::make_shared<OBJController>());

  engine.run();
  engine.shutdown();

  return 0;
}  ```
---

## Compile Your Game

    g++ main.cpp -std=c++17 -I/usr/local/include -L/usr/local/lib -lengine `sdl2-config --cflags --libs` -o game

---

## Makefile Commands

    make                → build static library  
    sudo make install   → install to system  
    sudo make uninstall → remove library + headers  
    make clean          → delete build files

