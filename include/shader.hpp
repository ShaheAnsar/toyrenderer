#ifndef SHADER_HPP
#define SHADER_HPP
#include <string>
#include <vector>
#include <utility>
#include "../include/glad/glad.h"

namespace Rend{
  class Shader{
    GLuint shader_id;
    GLenum shader_type;
    std::string fn;
    std::string entrypoint;
    friend class ShaderProgram;
  public:
    Shader(std::string fn, GLenum shader_type = GL_VERTEX_SHADER, const GLchar* entrypoint = "main", std::pair<std::vector<GLuint>, std::vector<GLuint>> specialization_constants = {{},{}});
    ~Shader();
    void reload();
  };

  class ShaderProgram {
    bool shader_created;
    GLuint program_id;
    std::vector<Shader> shaders;
  public:
    ShaderProgram(std::vector<Shader> shaders);
    ShaderProgram();
    void create_program();
    ~ShaderProgram();
    void use_program();
    void reload();
  };
}

#endif

