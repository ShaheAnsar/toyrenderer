#ifndef MY_RESOURCEMAN_HPP
#define MY_RESOURCEMAN_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>

#include <shader.hpp>
#include <mesh.hpp>
#include <texture.hpp>

namespace Rend{
  namespace RMan{
    struct Texture{
      static unsigned char count;
      std::vector<Rend::Texture> textures;
      std::vector<std::string> texture_names; // Name assigned to Texture. Not necessary to be the same as the texture pathname.
      std::unordered_map<std::string, std::size_t> name_map;
      //std::vector<std::size_t> texture_references; //Stores how many references to the texture have been given out
      Texture(); // Initialize singleton class
      ~Texture(); // Uninitialize it
      std::size_t add_texture(const std::string& name, GLenum internal_image_format = GL_RGB);
      std::size_t add_texture(const std::string& name, const std::string& path,
			      GLenum internal_image_format = GL_RGB);
      std::size_t add_texture(const Rend::Texture& o);
      std::size_t add_texture(const Rend::Texture& o, const std::string& name);
      inline Rend::Texture &get(std::size_t i) { return textures[i]; }
      const std::string& get_name(std::size_t n);
      void reload_texture(std::size_t i);
    };
  };
};

#endif
