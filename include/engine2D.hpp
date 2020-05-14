#ifndef ENGINE2D_HPP
#define ENGINE2D_HPP


#include <vector>
#include <queue>
#include <optional>
#include <memory>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat2x2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <../include/glad/glad.h>
#include <../include/shader.hpp>

namespace engine2D{

  enum class AttrPri{
		     FIRST = 0,
		     DEFAULT,
		     LAST
  };

  class Attribute {
  protected:
    friend class GameObject;
    friend class CompareAttributePriority;
    AttrPri pri;
    virtual void tick() = 0;
  public:
  };

  class Texture{

    
  };

  class Quad{
    glm::vec2 vertices[4]; // This stores the vertices calculcated from the below variables
    glm::vec2 position; // The position of the center of the quad
    glm::vec2 vertices_c[4]; // The vertices relative to the center
    glm::vec2 scale; // Scale factor
    float angle; // Angle in radians
    void calculate();
    friend class Renderable;
    friend class PhysicsInteractable;
  public:
    Quad(glm::vec2 ll, glm::vec2 dim);
    void translate(glm::vec2);
    void fscale(glm::vec2);
    void rotate(float angle);
  };


  class Renderable : public Attribute {
    Quad quad;
    std::optional<Texture> tex;
    glm::vec4 color;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint ubo;
    GLuint tex_id;
    Rend::ShaderProgram shader_prog;
  public:
    Renderable(glm::vec2 dim, glm::vec2 ll = {0.0f, 0.0f});
    void add_tex(Texture t);
    void add_color(glm::vec4 c);
    void set_shaderprog(Rend::ShaderProgram p);
    void submit_data();
    void tick() override;
    
  };

  class PhysicsInteractor : public Attribute {
    
  };

  class CompareAttributePriority{
    bool operator()(std::shared_ptr<Attribute>& l, std::shared_ptr<Attribute>& r) {
      return l->pri < r->pri;
    }
       
  };

  class GameObject {
    glm::vec2 position;
    std::vector<std::shared_ptr<Attribute>> attributes;
  public:
    GameObject(glm::vec2);
    void add_attribute(std::shared_ptr<Attribute> attr);
    void tick();
  };
}

#endif
