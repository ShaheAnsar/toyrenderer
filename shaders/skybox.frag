#version 460 core

layout(location = 0) in vec3 pos;

layout(location = 0) out vec4 ocol;

layout(binding = 3) uniform samplerCube skybox;


void main() {
  ocol = vec4(texture(skybox, pos).rgb, 1.0f);
}
