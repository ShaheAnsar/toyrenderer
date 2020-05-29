#version 460 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;
layout(location = 0) out vec2 opos;
layout(location = 1) out vec2 ouv;


void main() {
  gl_Position = vec4(pos, 0, 1.0f);
  opos = pos;
  ouv = uv;
}
