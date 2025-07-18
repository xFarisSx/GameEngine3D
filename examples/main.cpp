

#include <engine/engine.hpp>
#include <iostream>

using namespace engine;

// Example Script: Rotator
class Rotator : public Script {
  TransformComponent *transform = nullptr;
  EngineContext *ctx = nullptr;

public:
  Rotator() : Script("Rotator") {}

  void start() override {
    // Get the GameObject wrapper for this script's entity
    GameObject obj = getGameObject();
    transform = &obj.getComponent<TransformComponent>();

    // Get the engine context (for input, etc)
    ctx = world->getContext();
  }

  void update(float dt) override {
    if (ctx && ctx->controller->isKeyPressed(Key::Escape)) {
      // Rotate around Y axis continuously while Escape key is pressed
      transform->rotation.y += dt;
    }
  }
};

// Setup scene manually
void setupScene(World &world) {
  // Create a box GameObject
  GameObject box(world);
  box.getComponent<TransformComponent>().position = Vec3(4, 2, 1);
  box.setMesh(Mesh::createBox(1.0, 2.0, 1.0));

  MaterialComponent boxMaterial;
  boxMaterial.useTexture = false;
  box.setMaterial(boxMaterial);

  // Create a camera GameObject
  GameObject camera(world);
  auto &camTransform = camera.getComponent<TransformComponent>();
  camTransform.position = Vec3(0, 0, -5);
  camTransform.scale = Vec3(100.0f);

  camera.addComponent(CameraComponent{M_PI / 2, 16.f / 9.f, 1.0f, 30.f});
  camera.addComponent(CameraControllerComponent{});
  world.setCameraEntity(camera.getEntity());

  // Create a 3D model GameObject
  GameObject model(world);
  model.getComponent<TransformComponent>().scale = Vec3(0.01f);
  model.setMesh(Mesh::loadFromObj("assets/models/cat.obj"));

  MaterialComponent modelMat;
  modelMat.texture = Texture::loadFromBmp("assets/textures/textcat1.bmp");
  modelMat.useTexture = true;
  model.setMaterial(modelMat);

  model.addScript(std::make_shared<Rotator>());

  // Create a sphere GameObject with default material
  GameObject sphere(world);
  sphere.setMesh(Mesh::createSphere(1, 16, 32));
  sphere.setMaterial(MaterialComponent{});
}

int main() {
  Engine engine;
  engine.init(1600, 900, "My Game");

  auto &world = engine.world();

  // Register scripts
  world.registerScript<Rotator>("Rotator");

  // Option 1: Setup scene programmatically
  // setupScene(world);

  // Option 2: Load scene from JSON file
  world.loadScene("scenes/test.json");

  engine.run();
  engine.shutdown();

  return 0;
}
