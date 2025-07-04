#pragma once
#include "engine/math/vec3.hpp"

namespace engine {

  struct TransformComponent {
      Vec3 position;
      Vec3 rotation;
      Vec3 scale = {1.0f, 1.0f, 1.0f};
  };

 struct CameraComponent {
    Vec3 position;
    Vec3 rotation;  // pitch, yaw, roll as (x, y, z)
    
    Vec3 forward;
    Vec3 right;
    Vec3 up;
    Vec3 worldUp = Vec3(0, 1, 0);
    
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    
    double speed;
    double sensitivity;
}; 

}

