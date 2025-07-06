#pragma once
#include "engine/math/vec3.hpp"
#include <vector>
#include <string>
#include <SDL2/SDL.h>


using namespace std;

namespace engine{

struct Triangle{
  int i0, i1, i2;
  int uv0,uv1,uv2=0;

};

struct Mesh {
  vector<Vec3> vertices;
  vector<Triangle> triangles;
  vector<Vec3> textureMap;
  string textureFileName;
  Vec3 position = {0,0,0};

 SDL_Surface* textureSurface = nullptr;
    Uint32* texturePixels = nullptr;
    int texWidth = 0;
    int texHeight = 0;

    
    ~Mesh() {
        if (textureSurface) SDL_FreeSurface(textureSurface);
    } 
  static Mesh createCube();
  static Mesh loadFromObj(const string& filename);

};
}
