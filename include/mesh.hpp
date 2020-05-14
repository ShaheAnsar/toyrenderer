#ifndef MY_MESH_HPP
#define MY_MESH_HPP

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#undef TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <texture.hpp>
#include <glad/glad.h>


extern std::fstream flog;


namespace Rend{
  using ubo_tuple = std::pair<GLuint, std::size_t>;
  struct Mesh {
    std::vector<std::size_t> program_ids; // Program ID to give to resourceman
    std::vector<std::size_t> texture_ids; // Texture ID, see above
    std::vector<ubo_tuple> ubo_tuples;
    GLuint vao;
    GLuint vbo;
    bool success;
    Mesh(std::string path); // Load in mesh.
    Mesh(std::vector<float> vertices, std::size_t pos_off = 0,
		  std::size_t norm_off = 0, std::size_t uv_off = 0,
		  std::size_t pos_size = 3, std::size_t norm_size = 3, std::size_t uv_size = 2,
		  std::size_t pos_stride = 8, std::size_t norm_stride = 8,
		  std::size_t uv_stride = 8); // Load in mesh.

    ~Mesh();
  };
}

#endif
