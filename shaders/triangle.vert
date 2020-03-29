#version 460 core

layout(location = 0) in vec3 vert;
layout(location = 1) in vec3 col;

layout(location = 0) out vec3 color;


layout(std140, binding=0) uniform uniform_buffer {
  vec4 offset;
};

void main() {
  color = col;
  gl_Position = vec4(vert + offset.xyz, 1.0f);
}
