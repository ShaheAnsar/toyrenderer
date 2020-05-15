#ifndef MY_MESH_HPP
#define MY_MESH_HPP

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <functional>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_transform.hpp>

#undef TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <texture.hpp>
#include <glad/glad.h>


extern std::fstream flog;

struct basic_mat_ubo {
  glm::vec4 ambient_c;
  glm::vec4 diffuse_c;
  glm::vec4 specular_c;
};

struct basic_mat_rend {
  GLuint ubo;
  basic_mat_ubo mat_ubo;
  std::optional<Rend::Texture> ambient_t;
  std::optional<std::size_t> diffuse_t;
  std::optional<std::size_t> specular_t;
};


namespace Rend{
  // UBO Tuple Switch = {UBO GL_ID, Binding Slot, Offset after which its valid when drawing, No. of triangles for
  //                     which it is valid}
  // Use for UBOs that need to be automatically switched. Only one in the tuple can be active.
  using ubo_tup_switch = std::tuple<GLuint, int, std::size_t, std::size_t>;
  /*
   *  UBO Tuple - {UBO GL_ID, Binding Slot}
   *  Use for UBOs that don't need automatic switching. All will be active
   */
  using ubo_tuple = std::pair<GLuint, int>;
  //using tex_tuple = std::
  template<typename MaterialType, typename fn_mat, typename fn>
  struct Mesh {
    /*
     * MAT_TUP - {Index to `mats`, Face no from which this material is valid, no. of faces for which
     *            it will be valid}
     */
    using mat_tup = std::tuple<std::size_t, std::size_t, std::size_t>;
    std::vector<std::size_t> program_ids; // Program ID to give to resourceman
    std::vector<std::size_t> texture_ids; // Texture ID, see above
    std::vector<MaterialType> mats;
    std::vector<ubo_tup_switch> ubo_tup_switches;
    std::vector<ubo_tuple> ubo_tuples;
    std::vector<mat_tup> mat_tuples;
    GLuint vao;
    GLuint vbo;
    bool success;
    Mesh(std::string path, std::string base_dir, fn func, fn_mat func_mat); // Load in mesh.
    //Mesh(std::vector<float>& vertices, std::size_t pos_off = 0,
    //	 std::size_t norm_off = 0, std::size_t uv_off = 0,
    //	 std::size_t pos_size = 3, std::size_t norm_size = 3, std::size_t uv_size = 2,
    //	 std::size_t pos_stride = 8, std::size_t norm_stride = 8,
    //	 std::size_t uv_stride = 8); // Load in mesh.

    //~Mesh();
  };
}
#include "../src/mesh.tpp"
#endif
