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

layout(std140, binding = 2) uniform camera{
  vec4 cam_pos;
};

layout(std140, binding = 3) uniform debug{
  vec4 adsn; //Ambient, Diffuse, Specular on/off
};

layout(binding = 0)uniform sampler2D ambient_t;
layout(binding = 1)uniform sampler2D diffuse_t;
layout(binding = 2)uniform sampler2D specular_t;


vec4 calc_diffuse() {
  //return vec4(0.0f);
  float pfalloff = dot(pos_int.xyz - pos, pos_int.xyz - pos) * fallof_c.x
    + distance(pos_int.xyz, pos) * fallof_c.y + fallof_c.z;
  vec4 plight_c =  vec4(diffuse_c.rgb, 1.0f)*  clamp(dot(normal, normalize(pos_int.xyz - pos))
								   , 0.0f, 1.0f) * vec4(pcolor.xyz, 1.0f) / pfalloff;
  vec4 dlight_c = vec4(diffuse_c.rgb, 1.0f) * clamp(dlight_direction.w * dot(normal, normalize(dlight_direction.xyz)),
						    0.0f, 1.0f) * vec4(dlight_color.rgb, 1.0f);
  vec4 tlight = dlight_c + plight_c;
  float tlight_mod = length(tlight);
  if(tlight_mod < 0.05f)
    tlight = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  else if (tlight_mod < 0.3f)
    tlight = tlight/tlight_mod * 0.3f;
  else if(tlight_mod < 0.6f)
    tlight = tlight/tlight_mod * 0.7f;
  else
    tlight = tlight/tlight_mod * 1.0f;
  return tlight * adsn.y * texture(diffuse_t, uv);
}

vec4 calc_specular() {
  //return vec4(0.0f);
  vec4 plight_c =  vec4(specular_c.rgb, 1.0f) * texture(specular_t, uv) * pow( clamp( dot(
								   normalize( reflect((pos - pos_int.xyz), normal) ),
								   normalize(cam_pos.xyz - pos)), 0.0f, 1.0f), 64)
    * vec4(pcolor.rgb, 1.0f) / (fallof_c.x * dot(pos_int.xyz - pos, pos_int.xyz - pos)
				* fallof_c.y * distance(pos_int.xyz, pos) + fallof_c.z);

  vec4 dlight_c = vec4(specular_c.rgb, 1.0f) * /*texture(specular_t, uv) */
    pow(dlight_direction.w * clamp(dot(normalize(reflect(dlight_direction.xyz, normal)),
		  normalize(cam_pos.xyz - pos)), 0.0f, 1.0f), 64) * vec4( dlight_color.rgb, 1.0f );
  vec4 tlight = plight_c + dlight_c;
  float tlight_mod = length(tlight);
  if(tlight_mod < 0.7)
    tlight = vec4(0.0f, 0.0f, 0.0f, 1.0f);
  else if(tlight_mod < 0.9999999)
    tlight = vec4(vec3(0.8f), 1.0f) * texture(diffuse_t, uv);
  else
    tlight = vec4(1.0f);
  return adsn.z * (tlight);
}

vec4 calc_ambient() {
  //return adsn.x * vec4(0.1f);
  return adsn.x * vec4(ambient_c.rgb, 1.0f)* texture(ambient_t, uv) * pcolor;
}



void main() {
  //color = vec4( pcolor.rgb * clamp(dot(normal, normalize(pos_int.xyz - pos))
  //				   , 0.0f, 1.0f) + ambient_c.rgb*0.1f, pcolor.a);
  //color = texture(diffuse_t, uv);
  color = ( calc_diffuse() + calc_ambient() + calc_specular() );
  float color_mod = length(color);
//  if(color_mod < 0.3)
//    color *= 0.0f;
//  else if(color_mod < 0.7)
//    color *= 0.3f;
//  else
//    color *= 1.0f;
  //color *= texture(diffuse_t, uv);
}
