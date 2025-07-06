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

## Supported Model Formats

- Only supports `.obj` files for now
- Limited feature support (e.g. no normals, no materials)
- Some `.obj` files may fail or break the loader — still experimental

## Example Usage

```cpp
#include <engine/engine.hpp>
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>
#include <cmath>

using namespace engine;

  

class SimpleCameraController : public Script {
public:

    int speed = 1;
    TransformComponent* transform;
    

    void start() override {
        transform = &getComponent<TransformComponent>();
    }

    void update(float dt) override {
         
        
       transform->position.x+=speed;
   
    } 
};
 
int main() {
    Engine engine;
    engine.init(1600, 900, "My Game");

    auto& world = engine.world();
    
    Entity test = world.createEntity();

    world.addComponent<TransformComponent>(test, TransformComponent{
        Vec3(0,0,0),
        Vec3(0,0,0),
        Vec3(1,1,1)
    });
     
    Entity cam = world.createEntity();
    world.addComponent<TransformComponent>(cam, TransformComponent{
        Vec3(2,4,-5),
        Vec3(-M_PI/4,-M_PI/5,0),
        Vec3(1,1,1)
    });

    world.addComponent<CameraComponent>(cam, CameraComponent{
        M_PI/2,
        16.0f/9.0f,
        1.5f,
        100.0f,
        1,
        0.01
    });
    world.setCameraEntity(cam);
 
    
    std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>(Mesh::loadFromObj("assets/models/cat.obj"));

    SDL_Surface* loadedSurface = SDL_LoadBMP("assets/textures/textcat1.bmp");
    if (!loadedSurface) {
        std::cerr << "Failed to load BMP: " << SDL_GetError() << std::endl;
        exit(1);

    } 

    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(loadedSurface);

    if (!formattedSurface) {
        std::cerr << "Failed to convert surface: " << SDL_GetError() << std::endl;
        exit(1);
    }

    meshPtr->texturePixels = static_cast<Uint32*>(formattedSurface->pixels);
    meshPtr->texWidth = formattedSurface->w;
    meshPtr->texHeight = formattedSurface->h;

    
    world.addComponent<MeshComponent>(test, MeshComponent{meshPtr});

    world.addScript(cam, std::make_shared<SimpleCameraController>());
  

    engine.run();
    engine.shutdown();

    return 0;
}  
```

---

## Compile Your Game

    g++ main.cpp -std=c++17 -I/usr/local/include -L/usr/local/lib -lengine `sdl2-config --cflags --libs` -o game

---

## Makefile Commands

    make                → build static library
    sudo make install   → install to system
    sudo make uninstall → remove library + headers
    make clean          → delete build files
