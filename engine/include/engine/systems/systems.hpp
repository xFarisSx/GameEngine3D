#pragma once
#include "engine/ecs/system.hpp"
#include "engine/renderer/renderer.hpp"

namespace engine {
class RenderSystem : public System {
public:
    RenderSystem(Renderer* renderer) : renderer(renderer) {}

    void start(World& world) override{};

    void update(World& world) override;

private:
    Renderer* renderer;
};

class ScriptSystem : public System {

    void start(World& world) override;

    void update(World& world) override;
    
    

};


}  
