#pragma once
#include <SDL2/SDL.h>

namespace engine {
struct Controller{
  const Uint8* keystate;
  bool rc = false;
  bool lc = false;
  bool inmotion = false;
  int dx = 0;
  int dy = 0;

  

};
} 
