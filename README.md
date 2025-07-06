
# Faris Engine

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

## Example Usage

```cpp
#include <engine/engine.hpp>
using namespace engine;

class MyScript : public Script {
public:
    TransformComponent& transform;

    void start() override {
        transform = getComponent<TransformComponent>();
    }

    void update(float dt) override {
        transform.position.x += 0.01f;
    }
};

int main() {
    Engine engine;
    engine.init(1280, 720, "My Game");

    auto& world = engine.world();
    Entity e = world.createEntity();
    world.addComponent<TransformComponent>(e, TransformComponent{});
    world.addScript(e, std::make_shared<MyScript>());

    engine.run();
    engine.shutdown();
}
```

---

## Compile Your Game

    g++ main.cpp -std=c++17 -I/usr/local/include -L/usr/local/lib -lengine `sdl2-config --cflags --libs` -o game

---

## Makefile Commands

    make              → build static library
    sudo make install → install to system
    sudo make uninstall → remove library + headers
    make clean        → delete build files
