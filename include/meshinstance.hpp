#ifndef MY_MESH_INSTANCE_HPP
#define MY_MESH_INSTANCE_HP

#include <mesh.hpp>



#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <shader.hpp>


namespace Rend{
  template<typename MaterialType, typename FnType>
  struct MeshInstance{
    Mesh<MaterialType, FnType>* mesh_ptr;
    struct ubo{
      glm::mat4 mvp;
      glm::mat4 modelM;
      glm::vec4 texcoord_offset;
      glm::mat4 normalM;
    };
    GLuint ubo_id;
    void (*normal_calc_fn)(glm::mat4* normalM);
    Rend::ShaderProgram prog;
  };
}
#endif
