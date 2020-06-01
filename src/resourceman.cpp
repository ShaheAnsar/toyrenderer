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

std::size_t RMan::Texture::add_texture(const std::string& name, GLenum internal_image_format) {
  if(name_map.contains(name)) {
    flog << "[WARN] Texture " << name << " already exists" << std::endl;
    return name_map.at(name);
  }
  textures.push_back(Rend::Texture(name, internal_image_format));
  texture_names.push_back(name.c_str());
  name_map.emplace(name, textures.size() - 1);
  return textures.size() - 1;
}

std::size_t RMan::Texture::add_texture(const Rend::Texture &o) {
  if(name_map.contains(o.path)) {
    flog << "[WARN] Texture " << o.path << " already exists" << std::endl;
    return name_map.at(o.path);
  }
  textures.push_back(o);
  texture_names.push_back(o.path);
  name_map.emplace(o.path, textures.size() - 1);
  return textures.size() - 1;
}

std::size_t RMan::Texture::add_texture(const Rend::Texture &o, const std::string& name) {
  if(name_map.contains(name)) {
    flog << "[WARN] Texture " << name << " already exists" << std::endl;
    return name_map.at(name);
  }
  textures.push_back(o);
  texture_names.push_back(o.path);
  name_map.emplace(name, textures.size() - 1);
  return textures.size() - 1;
}

std::size_t RMan::Texture::add_texture(const std::string& name, const std::string& path,
				GLenum internal_image_format) {
  if(name_map.contains(name)) {
    flog << "[WARN] Texture " << name << " already exists" << std::endl;
    return name_map.at(name);
  }
  textures.push_back(Rend::Texture{path, internal_image_format});
  texture_names.push_back(name);
  name_map.emplace(name, textures.size() - 1);
  return textures.size() - 1;
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
