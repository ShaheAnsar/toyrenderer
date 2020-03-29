#version 460 core

layout(location = 0) in vec3 col;

layout(location = 0) out vec4 ocol;

void main() {
  ocol = vec4(col.zyx, 1.0f);
}
