#pragma once

#include <optional>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>

#include <texture.hpp>

struct engine_gl_ubo {
  float time;
  float dt;
}; // Values that are global to the engine

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
};

struct basic_mat {
  GLuint ubo;
  basic_mat_ubo mat_ubo;
  std::optional<Rend::Texture> ambient_t;
  std::optional<Rend::Texture> diffuse_t;
  std::optional<Rend::Texture> specular_t;
};
