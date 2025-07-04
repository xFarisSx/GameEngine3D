#pragma once

#include <SDL2/SDL.h>
#include "engine/components/components.hpp"
#include "engine/math/vec3.hpp"
#include <sys/types.h>
#include <vector>
#include "engine/assets/mesh.hpp"
#include <cstdint>
#include "engine/input/controller.hpp"

// Forward declarations for ECS components and World
namespace engine {
    using Entity = uint32_t;

    struct TransformComponent;
    struct CameraComponent;
    class World;

    class Renderer {
    public:
        Renderer(int width, int height, const char* title);
        ~Renderer();

        void clear(uint32_t color = 0xFF000000);
        void present();

        // Project a 3D point using camera components
        Vec3 project(const Vec3& point, const TransformComponent& transform, const CameraComponent& camera) const;

        void drawPixel(int x, int y, float z, uint32_t color);

        // Drawing primitives now use camera components instead of Camera class
        void drawPoint(const Vec3& point, const TransformComponent& transform, const CameraComponent& camera);
        void drawTriangle(const Mesh& mesh, const Triangle& tri, const std::vector<Vec3>& vertices,
                          const TransformComponent& transform, const CameraComponent& camera);
        void renderMesh(const Mesh& mesh, const TransformComponent& transform, const CameraComponent& camera);

        // Render world given meshes and the camera entity in the ECS world
        void renderWorld(const std::vector<Mesh>& meshes, Entity cameraEntity, World& world);

        float edgeFunction(const Vec3& a, const Vec3& b, const Vec3& c) const;

        // Handle SDL events and input controller (no camera input here, separate system)
        void handleEvents(bool& running, Controller& controller);

    private:
        SDL_Window* window;
        SDL_Renderer* sdlRenderer;
        SDL_Texture* sdlTexture;
        int screenWidth, screenHeight;

        uint32_t* framebuffer;
        std::vector<float> zBuffer;
        Vec3 lightDir = Vec3(0, 0, -1).normalized();
    };
}
