#ifndef MY_RESOURCEMAN_HPP
#define MY_RESOURCEMAN_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include <shader.hpp>
#include <mesh.hpp>
#include <texture.hpp>

namespace Rend{
  struct MeshResourceMan{
    std::unordered_map<std::string, std::size_t> meta_name;
    std::vector<Rend::Mesh> meshes;
    bool reload(std::size_t i); // Reload mesh with id i. Returns true if succeeded
    bool new_mesh(std::string path); // Load in mesh.
    bool new_mesh(std::vector<float> vertices, std::size_t pos_off = 0,
		  std::size_t norm_off = 0, std::size_t uv_off = 0,
		  std::size_t pos_size = 3, std::size_t norm_size = 3, std::size_t uv_size = 2,
		  std::size_t pos_stride = 8, std::size_t norm_stride = 8,
		  std::size_t uv_stride = 8); // Load in mesh.
    bool remove_mesh(std::size_t i);
    MeshResourceMan();
    ~MeshResourceMan();
  };
};

#endif
