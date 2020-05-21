#ifndef MY_TEXTURE_HPP
#define MY_TEXTURE_HPP

#include <string>

#include "../include/glad/glad.h"

namespace Rend{
  struct Texture{
    GLuint tex_id;
    GLenum internal_image_format;
    std::string path;
    void load_texture(const std::string& path, GLenum internal_image_format = GL_RGB);
    void reload();
    //Texture(std::string path);
    Texture(std::string, GLenum internal_image_format = GL_RGB);
    ~Texture();
    void bind(GLuint slot);
    void set_param(GLuint parameter, GLuint option);
  };
}
#endif
