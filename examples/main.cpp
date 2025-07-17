#include <engine/engine.hpp>
#include <iostream>
using namespace engine;

class Rotator : public Script {
  TransformComponent *transform;

public:
  Rotator() : Script("Rotator") {}

  void start() override { transform = &getComponent<TransformComponent>(); }
  void update(float dt) override { transform->rotation.y += dt; }
};

// int main() {
//   Engine engine;
//   engine.init(1600, 900, "My Game");
//
//   auto &world = engine.world();
//   world.registerScript<Rotator>("Rotator");
//
//   Entity someObj = world.createEntity();
//   world.addComponent<TransformComponent>(
//       someObj, {Vec3(4, 2, 1), Vec3(0.0f), Vec3(1.0f)});
//   world.addComponent<MeshComponent>(someObj, {Mesh::createCube()});
//   MaterialComponent mat1{};
//   mat1.useTexture = false;
//   world.addComponent<MaterialComponent>(someObj, mat1);
//
//   Entity cam = world.createEntity();
//   world.addComponent<TransformComponent>(
//       cam, {Vec3(0, 0, -5), Vec3(0.0f), Vec3(100.0f)});
//
//   world.addComponent<CameraComponent>(cam, {M_PI / 2, 16.f
//   / 9.f, 1.0f, 30.f});
//   world.addComponent<CameraControllerComponent>(cam);
//   world.setCameraEntity(cam);
//
//   Entity model = world.createEntity();
//   world.addComponent<TransformComponent>(model,
//                                          {Vec3(0), Vec3(0), Vec3(0.01f)});
//   world.addComponent<MeshComponent>(
//       model, {Mesh::loadFromObj("assets/models/cat.obj")});
//
//   auto tex = Texture::loadFromBmp("assets/textures/textcat1.bmp");
//   MaterialComponent mat{};
//   mat.texture = tex;
//   mat.useTexture = true;
//   world.addComponent<MaterialComponent>(model, mat);
//
//   world.addScript(model, std::make_shared<Rotator>());
//
//   //world.setParent(cam, model);
//
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
