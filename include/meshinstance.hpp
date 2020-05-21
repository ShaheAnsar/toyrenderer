#ifndef MY_MESH_INSTANCE_HPP
#define MY_MESH_INSTANCE_HPP

#include <mesh.hpp>



#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <shader.hpp>
#include <uniformbuffer.hpp>


namespace Rend{
  template<typename MaterialType, typename FnType>
  struct MeshInstance{
    Mesh<MaterialType, FnType>* mesh_ptr;
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
    Rend::ShaderProgram prog;
    struct ubo_t{
      glm::mat4 mvp;
      glm::mat4 modelM;
      glm::vec4 texcoord_offset;
      glm::mat4 normalM;
    };
    UniformBuffer<ubo_t> ubo;

    MeshInstance(Mesh<MaterialType, FnType>* mesh_ptr = nullptr) :
      mesh_ptr(mesh_ptr), position(0.0f), scale(1.0f), rotation(glm::cos(0.0f), glm::vec3(0.0f))
    {
    }
    
    void (*normal_calc_fn)(glm::mat4* normalM);
    // Stuff that needs to be done every frame. Will calculate the necessary matrices.
    inline void tick(const glm::mat4& perspectiveM, const glm::mat4& viewM) {
      glm::mat4& model = ubo.buffer.modelM;
      model = glm::mat4(1.0f);
      model = glm::scale(glm::mat4(1.0f), scale) * model;
      model = glm::toMat4(rotation) * model;
      model = glm::translate(glm::mat4(1.0f), position) * model;

      ubo.buffer.mvp = perspectiveM * viewM * model;
      ubo.buffer.normalM = glm::mat4(glm::transpose(glm::inverse(glm::mat3(model))));
      ubo.upload();
    }
  };
}
#endif
