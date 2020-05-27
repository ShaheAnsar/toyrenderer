#version 460 core


layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec2 ouv;





void main() {
  gl_Position = vec4( pos, 0.0f, 1.0f );
  ouv = uv;
}
