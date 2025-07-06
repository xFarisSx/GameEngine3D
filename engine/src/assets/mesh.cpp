#include "engine/assets/mesh.hpp"
#include "engine/math/vec3.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;
namespace engine {
Mesh Mesh::createCube() {
  Mesh mesh;

  mesh.vertices = {
    Vec3(-1, -1, -1), Vec3(1, -1, -1), Vec3(1, 1, -1), Vec3(-1, 1, -1),
    Vec3(-1, -1, 1), Vec3(1, -1, 1), Vec3(1, 1, 1), Vec3(-1, 1, 1)

  };

  mesh.triangles = { 
        {0, 1, 2}, {0, 2, 3}, // back
        {4, 6, 5}, {4, 7, 6}, // front
        {4, 0, 3}, {4, 3, 7}, // left
        {1, 5, 6}, {1, 6, 2}, // right
        {3, 2, 6}, {3, 6, 7}, // top
        {4, 5, 1}, {4, 1, 0}  // bottom
    };

  mesh.textureMap.resize(mesh.vertices.size());

  
  for (size_t i = 0; i < mesh.textureMap.size(); ++i) {
      mesh.textureMap[i] = Vec3(0, 0, 0);   }
 
    return mesh;

}
 
Mesh Mesh::loadFromObj(const string &filename)
{
  Mesh mesh;
  std::ifstream file(filename);

  if (!file.is_open()) {
    return mesh;
  }

  std::string line;
  double scale = 0.01;
  while (std::getline(file, line)) {
    std::istringstream iss(line);

    std::string type , a, b, c;

    iss >> type >> a >> b >> c;


    if (type == "v") {
      mesh.vertices.push_back(Vec3(stof(a)*scale, stof(b)*scale, stof(c)*scale));
    }
    else if (type == "vt"){
      mesh.textureMap.push_back(Vec3(stof(a), stof(b), stof(c)));


    }

    else if (type == "f") {
      string af = a;
      string bf =b;
      string cf = c;
      string av = "-1";
      string bv = "-1";
      string cv = "-1";

      size_t as = a.find('/'); 
      if (as != std::string::npos) {
        af = a.substr(0, as);
        av = a.substr(as+1);
      }
      size_t bs = b.find('/'); 
      if (bs != std::string::npos) {
        bf = b.substr(0, bs);
        bv = b.substr(bs+1);

      }
      size_t cs = c.find('/'); 
      if (cs != std::string::npos) {
        cf = c.substr(0, cs);
        cv = c.substr(cs+1);

      }
      


      mesh.triangles.push_back({stoi(af)-1,stoi(bf)-1,stoi(cf)-1,stoi(av)-1,stoi(bv)-1,stoi(cv)-1});
    }


  }

  return mesh;
}
}
