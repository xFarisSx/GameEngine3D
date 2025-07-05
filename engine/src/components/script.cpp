#include "engine/components/script.hpp"
#include "engine/core/world.hpp"

namespace engine {
void Script::setContext(uint32_t id, World* w) {
        entityId = id;
        world = w;
    } 

} 
