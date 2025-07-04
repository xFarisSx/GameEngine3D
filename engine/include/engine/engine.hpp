#pragma once
#include "engine/core/world.hpp"
#include "engine/math/vec3.hpp"
#include "engine/components/components.hpp"
#include <string>

namespace engine{
    using Entity = uint32_t;
    class Engine{
        public:
            Engine();
            ~Engine();
            
            void init(int width, int height, std::string name);
            void run();
            void shutdown(); 

            World& world(){ return _world; }
            const World& world() const { return _world; }
        private:
            World _world;

    };
}
