#ifndef MY_TEXTURE_HPP
#define MY_TEXTURE_HPP

#include <string>

#include "../include/glad/glad.h"

namespace Rend{
  struct Texture{
    GLuint tex_id;
    void load_texture(std::string path, GLenum internal_image_format = GL_RGB);
    Texture(std::string path);
    Texture(std::string, GLenum internal_image_format);
    void bind(GLuint slot);
    void set_param(GLuint parameter, GLuint option);
  };
}
#endif
