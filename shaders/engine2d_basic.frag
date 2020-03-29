#version 460 core

layout(location = 0) in vec2 iuv;
layout(location = 1) in vec4 icol;

layout(location = 0) out vec4 ocol;


void main() {
  ocol = icol;
}
