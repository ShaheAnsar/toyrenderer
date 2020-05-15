#version 460 core

layout(location = 0) out vec4 color;

layout(std140, binding = 6) uniform outline_ubo{
  vec4 outline_color;
};

void main() {
  color = outline_color;
}
