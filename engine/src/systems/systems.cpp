#include "engine/components/components.hpp"
#include "engine/ecs/system.hpp"
#include "engine/systems/systems.hpp"
#include "engine/assets/mesh.hpp"

#include "engine/core/world.hpp" 
#include "engine/renderer/renderer.hpp"
#include <SDL2/SDL.h>

namespace engine {
    using Entity = uint32_t;
    void RenderSystem::update(World& world) {
    const auto& entities = world.getEntities();

    Entity cameraEntity = world.getCamera(); 
    if (!cameraEntity)return ;


        // Step 2: Get camera components
        auto& cameraTransform = world.getComponent<TransformComponent>(cameraEntity);
        auto& camera = world.getComponent<CameraComponent>(cameraEntity);
            // Step 3: Render all visible meshes
        for (Entity entity : entities) {
            if (world.hasComponent<TransformComponent>(entity) &&
                world.hasComponent<MeshComponent>(entity)) {

                auto& transform = world.getComponent<TransformComponent>(entity);
                auto& meshC = world.getComponent<MeshComponent>(entity);
                
                if (!meshC.mesh) continue;


                renderer->renderMesh(meshC.mesh.get(), transform, cameraTransform, camera);
            }
        }}


    void ScriptSystem::update(World& world){
        const auto& entities = world.getEntities();
        for (Entity entity : entities) {
            if (world.hasComponent<ScriptComponent>(entity)) {
                auto& sc = world.getComponent<ScriptComponent>(entity);
                sc.script->update(1);
            }
        }
    }  

    void ScriptSystem::start(World& world){
        const auto& entities = world.getEntities();
        for (Entity entity : entities) {
            if (world.hasComponent<ScriptComponent>(entity)) {
                auto& sc = world.getComponent<ScriptComponent>(entity);
                sc.script->start();
            }
        }


    }

} 
