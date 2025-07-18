
#include <engine/engine.hpp>
#include <iostream>

using namespace engine;

class Rotator : public Script {
  TransformComponent *transform = nullptr;
  EngineContext *ctx = nullptr;

public:
  Rotator() : Script("Rotator") {}

  void start() override {
    GameObject obj = getGameObject();
    transform = &obj.getComponent<TransformComponent>();
    ctx = world->getContext();
  }

  void update(float dt) override {
    if (ctx && ctx->controller->isKeyPressed(Key::Escape)) {
      transform->rotation.y += dt;
    }
  }
};
//
// int main() {
//   Engine engine;
//   engine.init(1600, 900, "My Game");
//
//   auto &world = engine.world();
//   world.registerScript<Rotator>("Rotator");
//
//   // Box object
//   GameObject box(world);
//   box.getComponent<TransformComponent>().position = Vec3(4, 2, 1);
//   box.setMesh(Mesh::createBox(1.0, 2.0, 1.0));
//
//   MaterialComponent boxMaterial;
//   boxMaterial.useTexture = false;
//   box.setMaterial(boxMaterial);
//
//   // Camera object
//   GameObject camera(world);
//   auto &camTransform = camera.getComponent<TransformComponent>();
//   camTransform.position = Vec3(0, 0, -5);
//   camTransform.scale = Vec3(100.0f);
//
//   camera.addComponent(CameraComponent{M_PI / 2, 16.f / 9.f, 1.0f, 30.f});
//   camera.addComponent(CameraControllerComponent{});
//   world.setCameraEntity(camera.getEntity());
//
//   // 3D Model object
//   GameObject model(world);
//   model.getComponent<TransformComponent>().scale = Vec3(0.01f);
//   model.setMesh(Mesh::loadFromObj("assets/models/cat.obj"));
//
//   MaterialComponent modelMat;
//   modelMat.texture = Texture::loadFromBmp("assets/textures/textcat1.bmp");
//   modelMat.useTexture = true;
//   model.setMaterial(modelMat);
//   model.addScript(std::make_shared<Rotator>());
//
//   // Sphere object
//   GameObject sphere(world);
//   sphere.setMesh(Mesh::createSphere(1, 16, 32));
//   sphere.setMaterial(MaterialComponent{});
//
//   // Save scene
//   world.saveScene("scenes/test.json");
//
//   engine.run();
//   engine.shutdown();
// }
int main() {
  Engine engine;
  engine.init(1600, 900, "My Game");

  auto &world = engine.world();
  world.registerScript<Rotator>("Rotator");
  world.loadScene("scenes/test.json");

  engine.run();
  engine.shutdown();
}
