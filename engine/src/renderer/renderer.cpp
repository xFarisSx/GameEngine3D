
#include "engine/renderer/renderer.hpp"
#include "SDL_render.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "engine/assets/mesh.hpp"
#include "engine/components/components.hpp"
#include "engine/core/world.hpp"
#include <SDL2/SDL.h>

namespace engine {
 
 
Renderer::Renderer(int width, int height, const char* title)
    : screenWidth(width), screenHeight(height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdlRenderer) {
        std::cerr << "Renderer Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
 
    framebuffer = new uint32_t[screenWidth * screenHeight];
    zBuffer.resize(screenWidth * screenHeight);
}

Renderer::~Renderer() {
    delete[] framebuffer;
    SDL_DestroyTexture(sdlTexture);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::clear(uint32_t color) {
    if (!framebuffer) {
        std::cerr << "Error: framebuffer is null!" << std::endl;
        return;
    }
    if (screenWidth == 0 || screenHeight == 0) {
        std::cerr << "Error: screenWidth or screenHeight is zero!" << std::endl;
        return;
    }

    std::fill(framebuffer, framebuffer + screenWidth * screenHeight, color);
    std::fill(zBuffer.begin(), zBuffer.end(), std::numeric_limits<float>::infinity());
}

void Renderer::present() {
    SDL_UpdateTexture(sdlTexture, nullptr, framebuffer, screenWidth * sizeof(uint32_t));
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);
    SDL_RenderPresent(sdlRenderer);
}

void Renderer::handleEvents(bool& running, Controller& controller) {
    controller.inmotion = false;
    controller.dx = 0;
    controller.dy = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
                    }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
            controller.rc = true;
        }
        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT) {
            controller.rc = false;
        }
        if (event.type == SDL_MOUSEMOTION) {
            controller.inmotion = true;
            controller.dx = event.motion.xrel;
            controller.dy = event.motion.yrel;
        }
    }
    controller.keystate = SDL_GetKeyboardState(NULL);
}



static void updateCameraBasis(const Vec3& rotation, Vec3& forward, Vec3& right, Vec3& up, const Vec3& worldUp = Vec3(0, 1, 0)) {
    float yaw = rotation.y;
    float pitch = rotation.x;

    
    forward.x = cosf(pitch) * sinf(yaw);
    forward.y = sinf(pitch);
    forward.z = cosf(pitch) * cosf(yaw);
    forward = forward.normalized();

    
    right = worldUp.cross(forward);
    right = right.normalized();

    up = forward.cross(right);
    up = up.normalized();
}


Vec3 Renderer::project(const Vec3& point, const TransformComponent& cameraTransform, const CameraComponent& camera) const {
    Vec3 forward, right, up;
    updateCameraBasis(cameraTransform.rotation, forward, right, up);

    Vec3 modif = point - cameraTransform.position;
    float x = modif.dot(right);
    float y = modif.dot(up);
    float z = modif.dot(forward);
    if (z <= camera.nearPlane) return Vec3(0, 0, -1);
    //
    float d = 1/tanf(camera.fov/2);

    float px = (1 / camera.aspectRatio) * d * x / z;
    float py = d * y / z;

    return Vec3(screenWidth * (px + 1) / 2, screenHeight * (1 - py) / 2, z);
}

void Renderer::drawPixel(int x, int y, float z, uint32_t color) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
        int index = y * screenWidth + x;
        if (z < zBuffer[index]) {
            zBuffer[index] = z;
            framebuffer[index] = color;
        }
    } 
} 

float Renderer::edgeFunction(const Vec3& a, const Vec3& b, const Vec3& c) const {
    Vec3 v1 = b - a;
    Vec3 v2 = c - a;
    return v1.x * v2.y - v1.y * v2.x;
}


void Renderer::drawTriangle(Mesh* mesh, const Triangle& tri, const std::vector<Vec3>& vertices,
                            const TransformComponent& entityTransform,
                            const TransformComponent& cameraTransform,
                            const CameraComponent& camera) {
    Vec3 v0 = vertices[tri.i0] + mesh->position + entityTransform.position;
    Vec3 v1 = vertices[tri.i1] + mesh->position + entityTransform.position;
    Vec3 v2 = vertices[tri.i2] + mesh->position + entityTransform.position;

    Vec3 normal = (v1 - v0).cross(v2 - v0);
 
    Vec3 p0 = project(v0, cameraTransform, camera);
    Vec3 p1 = project(v1, cameraTransform, camera);
    Vec3 p2 = project(v2, cameraTransform, camera);


    
    Vec3 uv0 = mesh->textureMap[tri.uv0];
    Vec3 uv1 = mesh->textureMap[tri.uv1];
    Vec3 uv2 = mesh->textureMap[tri.uv2];

    if (p0.z < camera.nearPlane || p1.z < camera.nearPlane || p2.z < camera.nearPlane) return;

    float minX = std::min({p0.x, p1.x, p2.x});
    float maxX = std::max({p0.x, p1.x, p2.x});
    float minY = std::min({p0.y, p1.y, p2.y});
    float maxY = std::max({p0.y, p1.y, p2.y});

    for (int y = static_cast<int>(minY); y <= static_cast<int>(maxY); ++y) {
        for (int x = static_cast<int>(minX); x <= static_cast<int>(maxX); ++x) {
            Vec3 c(x, y, 0);
            float w0 = edgeFunction(p1, p2, c);
            float w1 = edgeFunction(p2, p0, c);
            float w2 = edgeFunction(p0, p1, c);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                float area = edgeFunction(p0, p1, p2);
                float alpha = w0 / area;
                float beta = w1 / area;
                float gamma = w2 / area;

                float u = alpha * uv0.x + beta * uv1.x + gamma * uv2.x;
                float v = alpha * uv0.y + beta * uv1.y + gamma * uv2.y;

                u = std::clamp(u, 0.0f, 1.0f);
                v = std::clamp(v, 0.0f, 1.0f);

                float depth = alpha * p0.z + beta * p1.z + gamma * p2.z;

                int texX = std::clamp(static_cast<int>(u * mesh->texWidth), 0, mesh->texWidth - 1);
                int texY = std::clamp(static_cast<int>((1.0f - v) * mesh->texHeight), 0, mesh->texHeight - 1);

                int index = texY * mesh->texWidth + texX;
                if (index < 0 || index >= static_cast<int>(mesh->texWidth * mesh->texHeight)) continue;

                Uint32 baseColor = mesh->texturePixels[index];

                float intensity = std::max(0.2f, normal.normalized().dot(lightDir));

                Uint8 r = (baseColor >> 16) & 0xFF;
                Uint8 g = (baseColor >> 8) & 0xFF;
                Uint8 b = baseColor & 0xFF;

                r = static_cast<Uint8>(r * intensity);
                g = static_cast<Uint8>(g * intensity);
                b = static_cast<Uint8>(b * intensity);

                Uint32 color = (r << 16) | (g << 8) | b;
                drawPixel(x, y, depth, color);
            }
        }
    }
}

void Renderer::renderMesh(Mesh* mesh, const TransformComponent& entityTransform,
                          const TransformComponent& cameraTransform, const CameraComponent& camera) {
    if (!mesh) return;

    SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
    for (const Triangle& tri : mesh->triangles) {
        drawTriangle(mesh, tri, mesh->vertices, entityTransform, cameraTransform, camera);
    }
}


void Renderer::renderWorld(Mesh* mesh, const TransformComponent& entityTransform,
                           const TransformComponent& cameraTransform, const CameraComponent& camera) {
    renderMesh(mesh, entityTransform, cameraTransform, camera);
}

}
 
