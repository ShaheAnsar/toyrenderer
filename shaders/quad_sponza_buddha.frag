#version 460 core



layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 col;


layout(binding = 0) uniform sampler2D sponza_color;
layout(binding = 1) uniform sampler2D sponza_depth;
layout(binding = 2) uniform sampler2D buddha_color;
layout(binding = 3) uniform sampler2D buddha_depth;

const float offset = 1.0/1000;
const float threshold = 0.00000001f;


vec4 compute_edge() {
  vec2 offsets[3][3] = {
			{vec2(-offset, -offset), vec2(0, -offset), vec2(offset, -offset)},
			{vec2(-offset, 0), vec2(0,0), vec2(offset, 0)},
			{vec2(-offset, offset), vec2(0,offset), vec2(offset, offset)}
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
      g_x += sobel_x[j][i] * texture(buddha_depth, uv + offsets[j][i]).r;
      g_y += sobel_y[j][i] * texture(buddha_depth, uv + offsets[j][i]).r;
    }
  }
  float g2 = pow(g_x,2) + pow(g_y, 2);
  float e = max(g2, threshold);
  if(e == threshold) e = 0.0f;
  else e = 1.0f;
  return vec4(vec3(1.0f - e), 1.0f);
}

void main() {
  vec4 front_col;
  if(texture(sponza_depth, uv).x < texture(buddha_depth, uv).x) {
    front_col = texture(sponza_color, uv);
  }
  else {
    front_col = texture(buddha_color, uv);
  }
  col = compute_edge() * front_col;
}

