
#include <SDL2/SDL.h>
#include <cmath>
#include <engine/components/components.hpp>
#include <engine/engine.hpp>
#include <iostream>
#include <memory>

using namespace engine;

class OBJController : public Script {
public:
  float speed = 0.3f;
  TransformComponent *transform;

  void start() override { transform = &getComponent<TransformComponent>(); }

  void update(float dt) override {

    // transform->rotation.y+=speed*dt;
  }
};

int main() {
  Engine engine;
  engine.init(1600, 900, "My Game");

  auto &world = engine.world();

  Entity test = world.createEntity();

  world.addComponent<TransformComponent>(
      test,
      TransformComponent{Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(0.01, 0.01, 0.01)});

  Entity cam = world.createEntity();
  world.addComponent<TransformComponent>(
      cam, TransformComponent{Vec3(0, 0, -5), Vec3(0, 0, 0), Vec3(1, 1, 1)});

  world.addComponent<CameraComponent>(
      cam, CameraComponent{M_PI / 2, 16.0f / 9.0f, 1.f, 30.0f});

  world.addComponent(cam, CameraControllerComponent{});
  world.setCameraEntity(cam);

  std::shared_ptr<Mesh> meshPtr = Mesh::loadFromObj("assets/models/cat.obj");

  std::shared_ptr<Texture> texturePtr =
      Texture::loadFromBmp("assets/textures/textcat1.bmp");

  MaterialComponent mat{};
  mat.texture = texturePtr;
  mat.useTexture = true;

  world.addComponent(test, mat);

  world.addComponent<MeshComponent>(test, MeshComponent{meshPtr});

  world.addScript(test, std::make_shared<OBJController>());

  engine.run();
  engine.shutdown();

  return 0;
}
