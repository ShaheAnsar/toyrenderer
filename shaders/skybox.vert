#version 460 core

layout(location = 0) in vec3 position;
layout(location = 0) out vec3 opos;

layout(std140, binding = 5) uniform ubo3d{
  mat4 mvp;
  mat4 model;
  mat4 normalM; // normalM will contain the view * model matrix here.
  vec4 texcoord_offset;
};


void main() {
  gl_Position = ( mvp * vec4(position, 1.0f) ).xyww;
  opos =  position;
}

