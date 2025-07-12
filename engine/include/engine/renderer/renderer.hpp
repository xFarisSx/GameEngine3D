
#pragma once

#include <SDL2/SDL.h>
#include <cstdint>
#include <limits>
#include <vector>

#include "engine/assets/mesh.hpp"
#include "engine/components/components.hpp"
#include "engine/input/controller.hpp"
#include "engine/math/vec3.hpp"
#include "engine/math/vec4.hpp"

namespace engine {

class Renderer {
public:
  Renderer(int width, int height, const char *title);
  ~Renderer();

  void clear(uint32_t color = 0xFF000000);
  void present();

  void handleEvents(bool &running, Controller &controller);

  Vec3 project(const Vec4 &point, const TransformComponent &cameraTransform,
               const CameraComponent &camera) const;

  void drawPixel(int x, int y, float z, uint32_t color);

  float edgeFunction(const Vec3 &a, const Vec3 &b, const Vec3 &c) const;

  void drawTriangle(Mesh *mesh, const Triangle &tri,
                    const std::vector<Vec3> &vertices,
                    const TransformComponent &entityTransform,
                    const TransformComponent &cameraTransform,
                    const CameraComponent &camera);

  void renderMesh(Mesh *mesh, const TransformComponent &entityTransform,
                  const TransformComponent &cameraTransform,
                  const CameraComponent &camera);

  void renderWorld(Mesh *mesh, const TransformComponent &entityTransform,
                   const TransformComponent &cameraTransform,
                   const CameraComponent &camera);

private:
  SDL_Window *window = nullptr;
  SDL_Renderer *sdlRenderer = nullptr;
  SDL_Texture *sdlTexture = nullptr;

  int screenWidth = 0;
  int screenHeight = 0;

  uint32_t *framebuffer = nullptr;
  std::vector<float> zBuffer;

  Vec3 lightDir = Vec3(0, 0, 1);
};

} // namespace engine
