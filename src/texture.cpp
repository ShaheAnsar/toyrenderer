#include <texture.hpp>

#include "../include/glad/glad.h"
#include "../include/stb_image.h"

#include <logger.hpp>

using namespace Rend;

extern logger::logger mlog;

Texture::Texture(std::string path){
  glGenTextures(1, &tex_id);
  int tw = 0, th = 0, tn = 0;
  unsigned char *td = stbi_load(path.c_str(), &tw, &th, &tn, 0);
  if(!td) mlog << std::pair<logger::pri, std::string>{logger::pri::ERR, "unable to load texture data"};
  GLuint image_format = GL_RGB;
  if(tn > 3) image_format = GL_RGBA;
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, image_format,
	       GL_UNSIGNED_BYTE, td);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(td);
  mlog << "Successfully loaded texture " + std::to_string(tex_id);
}

void Texture::bind(GLuint slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, tex_id);
}

void Texture::set_param(GLuint parameter, GLuint option) {
  glTexParameteri(GL_TEXTURE_2D, parameter, option);
}

