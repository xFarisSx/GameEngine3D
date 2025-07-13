
#pragma once

#include <SDL2/SDL.h>
#include "controller.hpp"

namespace engine {

class InputManager {
public:
  InputManager() = default;
    void pollEvents(bool& running, Controller* controller); };

}
