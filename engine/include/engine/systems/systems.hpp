#pragma once
#include "engine/ecs/system.hpp"
#include "engine/renderer/renderer.hpp"

namespace engine {
class RenderSystem : public System {
public:
    RenderSystem(Renderer* renderer) : renderer(renderer) {}

    void update(World& world) override;

private:
    Renderer* renderer;
};

class ScriptSystem : public System {

    void update(World& world) override;
    
    

};


} 
