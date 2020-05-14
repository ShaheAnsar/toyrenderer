#ifndef MY_TEXTURE_HPP
#define MY_TEXTURE_HPP

#include <string>

#include "../include/glad/glad.h"

namespace Rend{
  struct Texture{
    GLuint tex_id;
    Texture(std::string path);
    void bind(GLuint slot);
    void set_param(GLuint parameter, GLuint option);
  };
}
#endif
