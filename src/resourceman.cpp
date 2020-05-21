#include <resourceman.hpp>

#include <fstream>



using namespace Rend;

extern std::fstream flog;
extern std::fstream glog;

unsigned char RMan::Texture::count = 0;


// Texture manager
RMan::Texture::Texture() :
  textures({}), texture_names({})
{
  if(RMan::Texture::count >= 1) {
    flog << "[ERROR] Texture manager already active." << std::endl;
    std::exit(-1);
  }
  RMan::Texture::count++;
};

RMan::Texture::~Texture(){
  RMan::Texture::count--;
}

void RMan::Texture::add_texture(const std::string& name, GLenum internal_image_format) {
  textures.push_back(Rend::Texture(name, internal_image_format));
  texture_names.push_back(name.c_str());
}

Rend::Texture& RMan::Texture::get(std::size_t i) {
  return textures[i];
}

const std::string& RMan::Texture::get_name(std::size_t i) {
  return texture_names[i];
}

void RMan::Texture::reload_texture(std::size_t i) {
  textures[i].reload();
}
