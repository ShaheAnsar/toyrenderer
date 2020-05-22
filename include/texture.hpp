#ifndef MY_TEXTURE_HPP
#define MY_TEXTURE_HPP

#include <string>

#include "../include/glad/glad.h"


namespace Rend{
  struct Texture{
    GLuint tex_id;
    GLenum internal_image_format;
    std::string path;
    std::size_t* refc;
    void load_texture(const std::string& path, GLenum internal_image_format = GL_RGB);
    void reload();
    //Texture(std::string path);
    Texture(GLenum internal_image_format = GL_RGB); // Create a null texture for framebuffer use
    Texture(std::string, GLenum internal_image_format = GL_RGB);
    Texture(const Texture&);
    Texture& operator=(const Texture&);
    ~Texture();
    void bind(GLuint slot);
    void set_param(GLuint parameter, GLuint option);
  };
}
#endif
