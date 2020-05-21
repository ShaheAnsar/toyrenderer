#version 460 core



layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 col;


layout(binding = 0) uniform sampler2D sponza_color;
layout(binding = 1) uniform sampler2D sponza_depth;
layout(binding = 2) uniform sampler2D buddha_color;
layout(binding = 3) uniform sampler2D buddha_depth;

//const float offset = 1.0/1600;
const int offset = 1;
const float threshold = 0.0000005f;


vec4 compute_edge() {
  //return vec4(1.0f);
  ivec2 texture_size = textureSize(sponza_depth, 0);
  ivec2 uv_raw = ivec2(uv * texture_size);
  ivec2 offsets[3][3] = {
			{ivec2(-offset, -offset), ivec2(0, -offset), ivec2(offset, -offset)},
			{ivec2(-offset, 0), ivec2(0,0), ivec2(offset, 0)},
			{ivec2(-offset, offset), ivec2(0,offset), ivec2(offset, offset)}
  };


  
  float sobel_x[3][3] = {
			 {1, 0, -1},
			 {2, 0, -2},
			 {1, 0, -1}
  };
  float sobel_y[3][3] = {
			 {1, 2, 1},
			 {0, 0, 0},
			 {-1, -2, -1}
  };

  float g_x = 0;
  float g_y = 0;
  for(int j = 0; j < 3; j++) {
    for(int i = 0; i < 3; i++) {
      g_x += sobel_x[j][i] * texelFetch(buddha_depth, uv_raw + offsets[j][i], 0).r;
      g_y += sobel_y[j][i] * texelFetch(buddha_depth, uv_raw + offsets[j][i], 0).r;
    }
  }
  float g2 = pow(g_x,2) + pow(g_y, 2);
  float e = max(g2, threshold);
  if(e == threshold) e = 0.0f;
  else e = 1.0f;
  return vec4(vec3(1.0f - e), 1.0f);
}

void main() {
  float screen_offset = 0.00f; // decides how much of the screen to cut away.
  float rx = uv.s - 0.5f;
  float ry = uv.t - 0.5f;
  vec2 uv_offset_dir = normalize( vec2(rx, ry) );
  vec2 uv2 = vec2(rx, ry)*(0.5f - screen_offset)/0.5f + vec2(0.5f);
  float d = ( pow(rx, 2) + pow(ry, 2) )/0.5f;
  float c = 0.00f;
  float r_c = 1*c;
  float g_c = 5*c;
  float b_c = 10*c;

  vec4 front_col;
  if(texture(sponza_depth, uv).x <= texture(buddha_depth, uv).x) {
    front_col = texture(sponza_color,uv2 + uv_offset_dir*r_c*d) * vec4(1.0f, 0.0f, 0.0f, 0.0f);
    front_col += texture(sponza_color,uv2 + uv_offset_dir*g_c*d) * vec4(0.0f, 1.0f, 0.0f, 0.0f);
    front_col += texture(sponza_color,uv2 + uv_offset_dir*b_c*d) * vec4(0.0f, 0.0f, 1.0f, 0.0f);
    front_col += vec4(vec3(0.0f), 1.0f);
  }
  else {
    front_col = texture(buddha_color, uv);
  }
  col = compute_edge() * front_col;
    //col *= vec4(vec3(sin(uv.t * 3.14) * sin(uv.s * 3.14)), 1.0f);

// Manga filter.
//  col = vec4( vec3( col.r + col.g + col.b )/3 , 1.0f);
//  float k = 1000.0f;
//  float bias = - 0.0f;
//  float func_x = k*(pow(2, ( 0.5)* k/log(k)) + pow(2, uv.s));
//  float func_y = k*(pow(2, ( 0.5)* k/log(k)) + pow(2, uv.t));
//  col = col * step(1 -2*col.r + bias, sin(func_x)) * step(1 -2*col.r + bias, sin(func_y));

// Manga filter with an orange tint
//  float k = 1000.0f;
//  float bias = - 0.0f;
//  float func_x = k*(pow(2, ( 0.5)* k/log(k)) + pow(2, uv.s));
//  float func_y = k*(pow(2, ( 0.5)* k/log(k)) + pow(2, uv.t));
//  col = col * step(1 -2*col.r + bias, sin(func_x)) * step(1 -2*col.r + bias, sin(func_y));
}
