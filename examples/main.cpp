#include <engine/components/components.hpp>
#include <engine/engine.hpp>
using namespace engine;

class Rotator : public Script {
  TransformComponent *transform;
  void start() override { transform = &getComponent<TransformComponent>(); }
  void update(float dt) override { transform->rotation.y += dt; }
};

int main() {
  Engine engine;
  engine.init(1600, 900, "My Game");

  auto &world = engine.world();
  Entity cam = world.createEntity();
  world.addComponent<TransformComponent>(cam, {Vec3(0, 0, -5)});
  world.addComponent<CameraComponent>(cam, {M_PI / 2, 16.f / 9.f, 0.1f, 100.f});
  world.addComponent<CameraControllerComponent>(cam);
  world.setCameraEntity(cam);

  Entity model = world.createEntity();
  world.addComponent<TransformComponent>(model,
                                         {Vec3(0), Vec3(0), Vec3(0.01f)});
  world.addComponent<MeshComponent>(
      model, {Mesh::loadFromObj("assets/models/cat.obj")});

  auto tex = Texture::loadFromBmp("assets/textures/textcat1.bmp");
  MaterialComponent mat{};
  mat.texture = tex;
  mat.useTexture = true;
  world.addComponent<MaterialComponent>(model, mat);

  world.addScript(model, std::make_shared<Rotator>());

  engine.run();
  engine.shutdown();
}
