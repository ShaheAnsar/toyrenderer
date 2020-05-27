#version 460 core

layout(location = 3) in vec3 pos;
layout(location = 4) in vec3 normal;
layout(location = 5) in vec2 uv;

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
  vec4 normal_c; // If vec4(1.0f), texture exists, else it does not
};

layout(std140, binding = 2) uniform camera_t{
  vec4 position;
  vec4 up;
  vec4 dir;
}camera;

layout(std140, binding = 3) uniform debug{
  vec4 adsn; //Ambient, Diffuse, Specular on/off
};

layout(binding = 0) uniform sampler2D ambient_t;
layout(binding = 1) uniform sampler2D diffuse_t;
layout(binding = 2) uniform sampler2D specular_t;
layout(binding = 3) uniform sampler2D cel_shade_t;


vec4 calc_diffuse() {
  //return vec4(0.0f);
  vec4 diffuse_tex;
  if(diffuse_c.a > 0.1f)
    diffuse_tex = texture(diffuse_t, uv);
  else
    diffuse_tex = vec4( 1.0f );
  if(diffuse_tex.a < 0.5f)
    discard;

  float pfalloff = dot(pos_int.xyz - pos, pos_int.xyz - pos) * fallof_c.x
    + distance(pos_int.xyz, pos) * fallof_c.y + fallof_c.z;
  float plight_d = clamp(dot(normal, normalize(pos_int.xyz - pos))
			 , 0.0f, 1.0f)/pfalloff;
  float dlight_d = clamp(dlight_direction.w *
			 dot(normal, normalize(-dlight_direction.xyz)),
			 0.0f, 1.0f);
  vec3 dlight_cel = (texture(cel_shade_t, vec2( 0, dlight_d ) ).xyz);
  vec3 plight_cel = (texture(cel_shade_t, vec2( 0, plight_d ) ).xyz);
  vec4 tlight = vec4( diffuse_c.xyz, 1.0f ) * vec4( plight_cel * pcolor.xyz +
						dlight_cel * dlight_color.xyz, 1.0f);
  //return tlight * adsn.y * diffuse_tex;
  return vec4(vec3( plight_d + dlight_d ), 1.0f) * adsn.y * diffuse_tex * vec4(diffuse_c.rgb, 1.0f); /* texture(diffuse_t, uv)*/;
}

vec4 calc_specular() {
  vec4 specular_tex;
  if(specular_c.a > 0.1f)
    specular_tex = vec4( texture(specular_t, uv).rgba );
  else
    specular_tex = vec4( 1.0f );

  float pfalloff = dot(pos_int.xyz - pos, pos_int.xyz - pos) * fallof_c.x
    + distance(pos_int.xyz, pos) * fallof_c.y + fallof_c.z;
  vec3 half_vec_p = normalize(normalize( pos_int.xyz - pos  ) + normalize( camera.position.xyz - pos ));
  vec3 half_vec_d = normalize(normalize( -dlight_direction.xyz  ) + normalize( camera.position.xyz - pos ));

  //float plight_c = min(1.0/pfalloff, 1.0f) * clamp( dot(normalize(reflect(pos - pos_int.xyz, normal)),
  //							normalize(camera.position.xyz - pos)) , 0.0f, 1.0f);
  float plight_c = min(1.0f/pfalloff, 1.0f) * clamp(dot(half_vec_p,
							normal), 0.0f, 1.0f);
  plight_c = pow(plight_c, 8);
  //float dlight_c = dlight_direction.w * clamp(dot(normalize(reflect(dlight_direction.xyz, normal)),
  //						  normalize(camera.position.xyz - pos)) , 0.0f, 1.0f);
  float dlight_c = dlight_direction.w * clamp(dot(half_vec_d, normal), 0.0f, 1.0f);
  dlight_c = pow(dlight_c, 8);
  //vec4 plight_cel = vec4(texture(cel_shade_t, vec2(0, plight_c)).rgb, 1.0f);
  //vec4 dlight_cel = vec4(texture(cel_shade_t, vec2(0, dlight_c)).rgb, 1.0f);
  return adsn.z * vec4(vec3( plight_c + dlight_c ), 1.0f ) * specular_tex * vec4(specular_c.rgb, 1.0f); /* /*(specular_c.w * texture(specular_t, uv) )*/
}

vec4 calc_ambient() {
  //return adsn.x * vec4(0.1f);
  return vec4(vec3(0.2f), 1.0f) * texture(ambient_t, uv);
  //return adsn.x * vec4(ambient_c.rgb, 1.0f)* texture(ambient_t, uv) * pcolor;
}



void main() {
  vec4 diff = calc_diffuse();
  vec4 spec = calc_specular();
  vec4 ambi = calc_ambient();
  color = vec4(diff.rgb + spec.rgb + ambi.rgb, diff.a * ambi.a);
}
