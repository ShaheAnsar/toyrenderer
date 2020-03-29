#version 460 core


layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;


layout(location = 0) out vec3 opos;
layout(location = 1) out vec4 ocol;
layout(location = 2) out vec2 ouv;


layout(std140, binding = 0) uniform uniform_buffer {
  mat4 mvp;
  mat4 model;
  vec4 texcoord_offset;
};


void main() {
  gl_Position = mvp * vec4(position, 1.0f);
  opos = ( model * vec4(position, 1.0f) ).xyz;
  ocol = color;
  ouv = uv + texcoord_offset.xy;
}
