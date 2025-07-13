
#include "engine/renderer/renderer.hpp"
#include "SDL_render.h"
#include "engine/assets/mesh.hpp"
#include "engine/components/components.hpp"
#include "engine/core/world.hpp"
#include "engine/math/general.hpp"
#include "engine/math/mat4.hpp"
#include "engine/math/vec4.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

namespace engine {

Renderer::Renderer(int width, int height, const char *title)
    : screenWidth(width), screenHeight(height) {

  window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

  sdlTexture =
      SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);

  framebuffer = new uint32_t[screenWidth * screenHeight];
  zBuffer.resize(screenWidth * screenHeight);
}

Renderer::~Renderer() {
  delete[] framebuffer;
  SDL_DestroyTexture(sdlTexture);
  SDL_DestroyRenderer(sdlRenderer);
  SDL_DestroyWindow(window);
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
  std::fill(zBuffer.begin(), zBuffer.end(), std::numeric_limits<float>::max());
}

void Renderer::present() {
  SDL_UpdateTexture(sdlTexture, nullptr, framebuffer,
                    screenWidth * sizeof(uint32_t));
  SDL_RenderClear(sdlRenderer);
  SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);
  SDL_RenderPresent(sdlRenderer);
}

Vec3 Renderer::project(const Vec4 &point,
                       const TransformComponent &cameraTransform,
                       const CameraComponent &camera) const {
  Vec3 forward, right, up;
  math::updateCameraBasis(cameraTransform.rotation, forward, right, up);

  Mat4 viewM = Mat4::lookAt(cameraTransform.position,
                            cameraTransform.position + forward, Vec3(0, 1, 0));
  Mat4 perspM = Mat4::perspective(camera.fov, camera.aspectRatio,
                                  camera.nearPlane, camera.farPlane);
  Vec4 projected4 = perspM * viewM * point;

  if (projected4.w <= 0.0f)
    return Vec3(-1, -1, -1);
  Vec3 pr = projected4.toVec3();

  return Vec3(screenWidth * (pr.x + 1) / 2, screenHeight * (1 - pr.y) / 2,
              pr.z);
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

float Renderer::edgeFunction(const Vec3 &a, const Vec3 &b,
                             const Vec3 &c) const {
  Vec3 v1 = b - a;
  Vec3 v2 = c - a;
  return v1.x * v2.y - v1.y * v2.x;
}

auto isValid = [](const Vec3 &v) {
  return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
};

void Renderer::drawTriangle(Mesh *mesh, const Triangle &tri,
                            const std::vector<Vec3> &vertices,
                            const TransformComponent &entityTransform,
                            const TransformComponent &cameraTransform,
                            const CameraComponent &camera) {
  Vec3 v0 = vertices[tri.i0] + mesh->position + entityTransform.position;
  Vec3 v1 = vertices[tri.i1] + mesh->position + entityTransform.position;
  Vec3 v2 = vertices[tri.i2] + mesh->position + entityTransform.position;

  Vec3 forward, right, up;
  math::updateCameraBasis(cameraTransform.rotation, forward, right, up);
  Vec3 normal = (v1 - v0).cross(v2 - v0);
  if (normal.normalized().dot(forward) > 0) return;

  Vec4 v04 = Vec4(v0.x, v0.y, v0.z, 1);
  Vec4 v14 = Vec4(v1.x, v1.y, v1.z, 1);
  Vec4 v24 = Vec4(v2.x, v2.y, v2.z, 1);

  Vec3 p0 = project(v04, cameraTransform, camera);

  Vec3 p1 = project(v14, cameraTransform, camera);

  Vec3 p2 = project(v24, cameraTransform, camera);

  if (p0.z < 0.0f || p0.z > 1.0f || p0.x < 0 || p0.x >= screenWidth ||
      p0.y < 0 || p0.y >= screenHeight)
    return;
  if (p1.z < 0.0f || p1.z > 1.0f || p1.x < 0 || p1.x >= screenWidth ||
      p1.y < 0 || p1.y >= screenHeight)
    return;
  if (p2.z < 0.0f || p2.z > 1.0f || p2.x < 0 || p2.x >= screenWidth ||
      p2.y < 0 || p2.y >= screenHeight)
    return;

  if (!isValid(p0) || !isValid(p1) || !isValid(p2)) {
    std::cout << "Skipping triangle due to invalid projection values\n";
    return;
  }

  //std::cout << "Projected coords: p0(" << p0.x << "," << p0.y << "," << p0.z
    //        << "), "
     //       << "p1(" << p1.x << "," << p1.y << "," << p1.z << "), "
       //     << "p2(" << p2.x << "," << p2.y << "," << p2.z << ")\n";

  if (!mesh->texturePixels || mesh->texWidth <= 0 || mesh->texHeight <= 0) {
    std::cerr << "Invalid texture in mesh\n";
    return;
  }

  Vec3 uv0 = mesh->textureMap[tri.uv0];
  Vec3 uv1 = mesh->textureMap[tri.uv1];
  Vec3 uv2 = mesh->textureMap[tri.uv2];

  float minX = std::min({p0.x, p1.x, p2.x});
  float maxX = std::max({p0.x, p1.x, p2.x});
  float minY = std::min({p0.y, p1.y, p2.y});
  float maxY = std::max({p0.y, p1.y, p2.y});

  int minXInt = std::max(0, static_cast<int>(std::floor(minX)));
  int maxXInt = std::min(screenWidth - 1, static_cast<int>(std::ceil(maxX)));
  int minYInt = std::max(0, static_cast<int>(std::floor(minY)));
  int maxYInt = std::min(screenHeight - 1, static_cast<int>(std::ceil(maxY)));

  float area = edgeFunction(p0, p1, p2);
  if (std::abs(area) < 1e-6f) {
          return;
        }
  for (int y = minYInt; y <= maxYInt; ++y) {
    for (int x = minXInt; x <= maxXInt; ++x) {
      Vec3 c(x, y, 0);
      float w0 = edgeFunction(p1, p2, c);
      float w1 = edgeFunction(p2, p0, c);
      float w2 = edgeFunction(p0, p1, c);

      if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
        
        float alpha = w0 / area;
        float beta = w1 / area;
        float gamma = w2 / area;

        float u = alpha * uv0.x + beta * uv1.x + gamma * uv2.x;
        float v = alpha * uv0.y + beta * uv1.y + gamma * uv2.y;

        u = std::clamp(u, 0.0f, 1.0f);
        v = std::clamp(v, 0.0f, 1.0f);

        float depth = alpha * p0.z + beta * p1.z + gamma * p2.z;
        if (!std::isfinite(depth) || depth < 0 || depth > 1) {
          continue;
        }

        int texX = std::clamp(static_cast<int>(u * mesh->texWidth), 0,
                              mesh->texWidth - 1);
        int texY = std::clamp(static_cast<int>((1.0f - v) * mesh->texHeight), 0,
                              mesh->texHeight - 1);

        int index = texY * mesh->texWidth + texX;
        if (index < 0 ||
            index >= static_cast<int>(mesh->texWidth * mesh->texHeight))
          continue;

        Uint32 baseColor = mesh->texturePixels[index];

        float intensity =
            std::max(0.2f, normal.normalized().dot(lightDir * -1));

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

void Renderer::renderMesh(Mesh *mesh, const TransformComponent &entityTransform,
                          const TransformComponent &cameraTransform,
                          const CameraComponent &camera) {
  if (!mesh)
    return;

  SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
  for (const Triangle &tri : mesh->triangles) {
    drawTriangle(mesh, tri, mesh->vertices, entityTransform, cameraTransform,
                 camera);
  }
}

void Renderer::renderWorld(Mesh *mesh,
                           const TransformComponent &entityTransform,
                           const TransformComponent &cameraTransform,
                           const CameraComponent &camera) {
  renderMesh(mesh, entityTransform, cameraTransform, camera);
}

} // namespace engine
