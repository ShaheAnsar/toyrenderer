#version 460 core

layout(location = 0) in vec3 col;
layout(location = 0) out vec4 color;



void main() {
  color = vec4(col, 1.0f);
}
