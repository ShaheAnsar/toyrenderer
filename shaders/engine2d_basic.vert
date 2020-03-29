#version 460 core

layout(location = 0) in vec2 iposition;
layout(location = 1) in vec2 iuv;
layout(location = 2) in vec4 icol;

layout(location = 0) out vec2 ouv;
layout(location = 1) out vec4 ocol;

layout(std140, binding = 0) uniform uniform_buffer{
  vec4 offset_scale;
  vec4 angle;
};


void main() {
  gl_Position = vec4(iposition.x*offset_scale.z + offset_scale.x, iposition.y*offset_scale.w + offset_scale.y, 0.0f, 1.0f);
  //gl_Position = vec4(iposition, 0.0f, 1.0f);
  ouv = iuv;
  ocol = icol;
}
