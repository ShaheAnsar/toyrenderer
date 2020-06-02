#pragma once

#include <optional>
#include <iostream>
#include <fstream>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>

#include <texture.hpp>

extern std::fstream flog;

struct engine_gl_ubo {
  float time;
  float dt;
}; // Values that are global to the engine and passed to the shaders

struct engine_globals {
  std::size_t width;
  std::size_t height;
}; // Globals that will be used by code in the engine

struct uniform_buffer_3d {
  glm::mat4 mvp;
  glm::mat4 model;
  glm::mat4 normalM;
  glm::vec4 texcoord_offset;
};
struct point_light {
  glm::vec4 position_intensity;
  glm::vec4 color;
  glm::vec4 falloff_constants;
};

struct directional_light {
  glm::vec4 direction_intensity;
  glm::vec4 color;
};
struct debug_ubo {
  glm::vec4 adsn;
};


struct basic_mat_ubo {
  glm::vec4 ambient_c;
  glm::vec4 diffuse_c;
  glm::vec4 specular_c;
  glm::uvec4 bitmap; // BITMAP. Bit 0 corresponds to the existence of a normal map
};

struct basic_mat {
  GLuint ubo;
  basic_mat_ubo mat_ubo;
  char ambient_t;
  char diffuse_t;
  char specular_t;
  char normal_t;
};

extern void log_recurse();

template<typename T, typename... Args>
static void log_recurse(T f, Args ... args) {
  flog << f;
  log_recurse(args ...);
}

template<typename T, typename... Args>
void err(T f, Args ... args) {
  flog << "[ERROR] ";
  log_recurse(f, args ...);
}

template<typename T, typename... Args>
void log(T f, Args ... args) {
  flog << "[DEBUG] ";
  log_recurse(f, args ...);
}




extern const engine_globals eGlobals;
