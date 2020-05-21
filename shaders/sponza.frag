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
  //return vec4(0.0f);
//  vec4 plight_c =  vec4(specular_c.rgb, 1.0f) * texture(specular_t, uv) * pow( clamp( dot(
//								   normalize( reflect((pos - pos_int.xyz), normal) ),
//								   normalize(cam_pos.xyz - pos)), 0.0f, 1.0f), 64)
//    * vec4(pcolor.rgb, 1.0f) / (fallof_c.x * dot(pos_int.xyz - pos, pos_int.xyz - pos)
//				* fallof_c.y * distance(pos_int.xyz, pos) + fallof_c.z);
//
//  vec4 dlight_c = vec4(specular_c.rgb, 1.0f) * /*texture(specular_t, uv) */
//    pow(dlight_direction.w * clamp(dot(normalize(reflect(dlight_direction.xyz, normal)),
//		  normalize(cam_pos.xyz - pos)), 0.0f, 1.0f), 64) * vec4( dlight_color.rgb, 1.0f );
//  vec4 tlight = plight_c + dlight_c;
//  float tlight_mod = length(tlight);
//  if(tlight_mod < 0.7)
//    tlight = vec4(0.0f, 0.0f, 0.0f, 1.0f);
//  else if(tlight_mod < 0.9999999)
//    tlight = vec4(vec3(0.8f), 1.0f) * texture(diffuse_t, uv);
//  else
//    tlight = vec4(1.0f);
//  return adsn.z * (tlight);
  vec4 specular_tex;
  if(specular_c.a > 0.1f)
    specular_tex = vec4( texture(specular_t, uv).rgba );
  else
    specular_tex = vec4( 1.0f );

  float pfalloff = dot(pos_int.xyz - pos, pos_int.xyz - pos) * fallof_c.x
    + distance(pos_int.xyz, pos) * fallof_c.y + fallof_c.z;

  float plight_c = min(1.0/pfalloff, 1.0f) * clamp( dot(normalize(reflect(pos - pos_int.xyz, normal)),
							normalize(camera.position.xyz - pos)) , 0.0f, 1.0f);
  plight_c = pow(plight_c, 8);
  float dlight_c = dlight_direction.w * clamp(dot(normalize(reflect(dlight_direction.xyz, normal)),
						  normalize(camera.position.xyz - pos)) , 0.0f, 1.0f);
  dlight_c = pow(dlight_c, 8);
  vec4 plight_cel = vec4(texture(cel_shade_t, vec2(0, plight_c)).rgb, 1.0f);
  vec4 dlight_cel = vec4(texture(cel_shade_t, vec2(0, dlight_c)).rgb, 1.0f);
  //return adsn.z * (dlight_cel + plight_cel);
  return adsn.z * vec4(vec3( plight_c + dlight_c ), 1.0f ) * specular_tex * vec4(specular_c.rgb, 1.0f); /* /*(specular_c.w * texture(specular_t, uv) )*/
}

vec4 calc_ambient() {
  //return adsn.x * vec4(0.1f);
  return vec4(vec3(0.0f), 1.0f);
  //return adsn.x * vec4(ambient_c.rgb, 1.0f)* texture(ambient_t, uv) * pcolor;
}



void main() {
  //color = vec4( pcolor.rgb * clamp(dot(normal, normalize(pos_int.xyz - pos))
  //				   , 0.0f, 1.0f) + ambient_c.rgb*0.1f, pcolor.a);
  //color = texture(diffuse_t, uv);
  vec4 diff = calc_diffuse();
  vec4 spec = calc_specular();
  vec4 ambi = calc_ambient();
  color = vec4(diff.rgb + spec.rgb + ambi.rgb, diff.a * ambi.a);
  float color_mod = length(color);
//  if(color_mod < 0.3)
//    color *= 0.0f;
//  else if(color_mod < 0.7)
//    color *= 0.3f;
//  else
//    color *= 1.0f;
  //color *= texture(diffuse_t, uv);
}
