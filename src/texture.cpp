#include <texture.hpp>

#include "../include/glad/glad.h"
#include "../include/stb_image.h"

#include <fstream>

#include <common.hpp>
#include <logger.hpp>


using namespace Rend;

extern logger::logger mlog;
extern std::fstream flog;

void Texture::load_texture(const std::string& path, GLenum internal_image_format){
  glGenTextures(1, &tex_id);
  int tw = 0, th = 0, tn = 0;
  unsigned char *td = stbi_load(path.c_str(), &tw, &th, &tn, 0);
  if(!td) mlog << std::pair<logger::pri, std::string>{logger::pri::ERR, "unable to load texture data"};
  GLuint image_format = GL_RGB;
  if(tn > 3) image_format = GL_RGBA;
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, internal_image_format, tw, th, 0, image_format,
	       GL_UNSIGNED_BYTE, td);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(td);
  mlog << "Successfully loaded texture " + std::to_string(tex_id);
  flog << "Texture: " << path << ", Image Format: " << std::to_string(tn) << std::endl;
}

void Texture::reload(){
  glDeleteTextures(1, &tex_id);
  flog << "Reloading Texture" << std::endl;
  this->load_texture(path, internal_image_format);
}


Texture::Texture(std::string path, GLenum internal_image_format) :
  internal_image_format(internal_image_format), path(path.c_str()), refc(nullptr) {
  refc = new std::size_t;
  if(!refc)
    flog << "Found nullpointer!";
  *refc = 1;
  load_texture(path, internal_image_format);
}

Texture::Texture(GLenum internal_image_format):
  internal_image_format(internal_image_format), path("FrameBufferTexture"), refc(nullptr)
{
  refc = new std::size_t;
  if(!refc) {
    flog << "Found nullpointer Texture: " << path << std::endl;
  }
  *refc = 1;
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, internal_image_format,
	       eGlobals.width, eGlobals.height,
	       0, internal_image_format,
	       GL_UNSIGNED_BYTE, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const Texture& t) :
  tex_id(t.tex_id), internal_image_format(t.internal_image_format), path(t.path.c_str()), refc(t.refc)
{
  (*refc)++;
}

Texture& Texture::operator=(const Texture& t) {
  if(*refc <= 1) {
    flog << "Deleting Texture" << std::endl;
    glDeleteTextures(1, &tex_id);
    delete refc;
    refc = nullptr;
  } else {
    ( *refc )--;
  }
  refc = t.refc;
  (*refc)++;
  tex_id = t.tex_id;
  internal_image_format = t.internal_image_format;
  path = t.path.c_str();
  return *this;
}

Texture::~Texture() {
  if (*refc <= 1) {
    flog << "Deleting Texture: " << path << std::endl;
    glDeleteTextures(1, &tex_id);
    delete refc;
  }
  else {
    (*refc)--;
  }
}

void Texture::bind(GLuint slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, tex_id);
}

void Texture::set_param(GLuint parameter, GLuint option) {
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexParameteri(GL_TEXTURE_2D, parameter, option);
}
