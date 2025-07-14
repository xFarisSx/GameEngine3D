#include "engine/assets/mesh.hpp"
#include "engine/math/vec3.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace engine {

std::shared_ptr<Mesh> Mesh::createCube() {
  auto mesh = std::make_shared<Mesh>();

  // Define 8 cube vertices
  mesh->vertices = {
      Vec3(-1, -1, -1), // 0
      Vec3(1, -1, -1),  // 1
      Vec3(1, 1, -1),   // 2
      Vec3(-1, 1, -1),  // 3
      Vec3(-1, -1, 1),  // 4
      Vec3(1, -1, 1),   // 5
      Vec3(1, 1, 1),    // 6
      Vec3(-1, 1, 1)    // 7
  };

  // Define triangles with CCW winding for each face
  mesh->triangles = {// Back face (z = -1)
                     {2, 1, 0},
                     {3, 2, 0},

                     // Front face (z = +1)
                     {4, 5, 6},
                     {4, 6, 7},

                     // Left face (x = -1)
                     {0, 4, 7},
                     {0, 7, 3},

                     // Right face (x = +1)
                     {6, 5, 1},
                     {6, 1, 2},

                     // Top face (y = +1)
                     {3, 7, 6},
                     {3, 6, 2},

                     // Bottom face (y = -1)
                     {0, 1, 5},
                     {0, 5, 4}};

  // Dummy UVs (all zeros, you can replace these with actual UVs)
  mesh->textureMap.resize(mesh->vertices.size(), Vec3(0, 0, 0));

  return mesh;
}

std::shared_ptr<Mesh> Mesh::loadFromObj(const std::string &filename) {
  auto mesh = std::make_shared<Mesh>();
  std::ifstream file(filename);

  if (!file.is_open()) {
    return mesh;
  }

  std::string line;
  double scale = 1;
  while (std::getline(file, line)) {
    std::istringstream iss(line);

    std::string type, a, b, c;

    iss >> type >> a >> b >> c;

    if (type == "v") {
      mesh->vertices.push_back(
          Vec3(stof(a) * scale, stof(b) * scale, stof(c) * scale));
    } else if (type == "vt") {
      mesh->textureMap.push_back(Vec3(stof(a), stof(b), stof(c)));

    }

    else if (type == "f") {
      std::string af = a;
      std::string bf = b;
      std::string cf = c;
      std::string av = "-1";
      std::string bv = "-1";
      std::string cv = "-1";

      size_t as = a.find('/');
      if (as != std::string::npos) {
        af = a.substr(0, as);
        av = a.substr(as + 1);
      }
      size_t bs = b.find('/');
      if (bs != std::string::npos) {
        bf = b.substr(0, bs);
        bv = b.substr(bs + 1);
      }
      size_t cs = c.find('/');
      if (cs != std::string::npos) {
        cf = c.substr(0, cs);
        cv = c.substr(cs + 1);
      }

      mesh->triangles.push_back({stoi(af) - 1, stoi(bf) - 1, stoi(cf) - 1,
                                 stoi(av) - 1, stoi(bv) - 1, stoi(cv) - 1});
    }
  }

  return mesh;
}
} // namespace engine
