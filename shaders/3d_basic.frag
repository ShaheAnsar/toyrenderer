#version 460 core


layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 col;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 ocol;

layout(binding = 3) uniform sampler2D _tex;

layout(std140, binding = 5) uniform uniform_buffer {
  mat4 mvp;
  mat4 model;
  vec4 texcoord_offset;
  //sampler2D _texture;
};


void main() {
  ocol = texture(_tex, uv);
}
