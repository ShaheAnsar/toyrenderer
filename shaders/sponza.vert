#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;


layout(location = 3) out vec3 opos;
layout(location = 4) out vec3 onormal;
layout(location = 5) out vec2 ouv;

layout(std140, binding = 5) uniform uniform_buffer {
  mat4 mvp;
  mat4 model;
  mat4 normalM;
  vec4 texcoord_offset;
};

void main() {
  gl_Position = mvp * vec4(pos, 1.0f);
  opos = ( model * vec4(pos, 1.0f) ).xyz;
  ouv = uv;
  onormal = normalize( ( model*vec4( normal, 0.0f ) ).xyz );
}
