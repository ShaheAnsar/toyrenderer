#version 460 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;
layout(location = 0) out vec4 color;

layout(binding = 0) uniform sampler2D image;

void main() {
  color = texture(image, uv);
}
