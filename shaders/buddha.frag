#version 460 core

layout(location = 3) in vec3 pos;
layout(location = 4) in vec3 normal;

layout(location = 0) out vec4 color;

layout(std140, binding = 0) uniform plight{
  vec4 pos_int; // Position and Intensity
  vec4 pcolor;
  vec4 fallof_c;
};

layout(std140, binding = 4) uniform dlight {
  vec4 dlight_direction;
  vec4 dlight_color;
};

layout(std140, binding = 1) uniform mat{
  vec4 ambient_c; // <R, G, B, [Texture exists? 1 if true, 0 if yes]>
  vec4 diffuse_c;
  vec4 specular_c;
};

layout(std140, binding = 2) uniform camera{
  vec4 cam_pos;
};

layout(std140, binding = 3) uniform debug{
  vec4 adsn; //Ambient, Diffuse, Specular on/off
};

layout(binding = 0) uniform sampler2D cel_shade;



vec4 calc_ambient(){
  return vec4(vec3(1.0f), 1.0f);
}

vec4 calc_diffuse(){
  vec4 _color = vec4(0.6f);
  _color.a = 1.0f;
  float pfalloff = dot(pos_int.xyz - pos, pos_int.xyz - pos) * fallof_c.x
    + distance(pos_int.xyz, pos) * fallof_c.y + fallof_c.z;

  float t = min(1.0f, 1.0f/pfalloff) * clamp(dot(normal, normalize( pos_int.xyz - pos )), 0.0f, 1.0f);
  t += clamp(dlight_direction.w * dot(normal, normalize( -dlight_direction.xyz)), 0.0f, 1.0f);
  //return vec4(vec3(t), 1.0f);
  return vec4( texture(cel_shade, vec2(0.0f, t)).rgb, 1.0f ) * _color;

}

vec4 calc_specular(){
  float pfalloff = dot(pos_int.xyz - pos, pos_int.xyz - pos) * fallof_c.x
    + distance(pos_int.xyz, pos) * fallof_c.y + fallof_c.z;

  float t = min(1.0f, 1.0f/pfalloff) * clamp(dot(normalize( reflect(pos - pos_int.xyz, normal) ),
		      normalize( cam_pos.xyz - pos)), 0.0f, 1.0f);
  t = pow(t, 32);
  //return vec4(vec3(t), 1.0f);
  //return vec4( texture(cel_shade, vec2(0, t)).rgb, 1.0f );
  return vec4(vec3( step(0.2f, t) ), 1.0f);
}


void main() {
  color = adsn.x * calc_ambient() + adsn.y*calc_diffuse() + adsn.z*calc_specular();
}
