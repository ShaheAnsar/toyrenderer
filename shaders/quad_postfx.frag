#version 460 core



layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 col;

layout(binding = 0) uniform sampler2D input_image;
// Trims the algorithm from processing darks.
//   0.0833 - upper limit (default, the start of visible unfiltered edges)
//   0.0625 - high quality (faster)
//   0.0312 - visible limit (slower)
const float threshold = 0.0312;
// The minimum amount of local contrast required to apply algorithm.
//   0.333 - too little (faster)
//   0.250 - low quality
//   0.166 - default
//   0.125 - high quality 
//   0.063 - overkill (slower)
const float rthreshold = 0.166;

ivec2 uv_raw;
const int offset = 1;


vec4 gaussian_blur() {
  return vec4(1.0f);
}

float rgbtolum(vec4 c) {
 return c.g;
}

vec4 fxaa() {
  float m, n, s, e, w, ne, nw, se, sw;
  m = rgbtolum(texelFetch(input_image, uv_raw, 0));
  e = rgbtolum(texelFetch(input_image, uv_raw + ivec2(offset, 0), 0));
  w = rgbtolum(texelFetch(input_image, uv_raw - ivec2(offset, 0), 0));
  s = rgbtolum(texelFetch(input_image, uv_raw - ivec2(0, offset), 0));
  n = rgbtolum(texelFetch(input_image, uv_raw + ivec2(0, offset), 0));
  ne = rgbtolum(texelFetch(input_image, uv_raw + ivec2(offset, offset), 0));
  nw = rgbtolum(texelFetch(input_image, uv_raw + ivec2(-offset, offset), 0));
  se = rgbtolum(texelFetch(input_image, uv_raw + ivec2(offset, -offset), 0));
  sw = rgbtolum(texelFetch(input_image, uv_raw + ivec2(-offset, -offset), 0));
  float lowest = min(m, min(n, min(s, min(e, w))));
  float highest = max(m, max(n, max(s, max(e, w))));
  float contrast = highest - lowest;
  float combined_threshold = max(threshold, rthreshold*highest);
  if(contrast < combined_threshold) {
    return col = texture(input_image, uv);;
  }
  float _filter = ne + nw + se + sw;
  _filter += 2*(n + s + e + w);
  _filter /= 12;
  _filter = abs(_filter - m);
  _filter = clamp(_filter/contrast, 0.0f, 1.0f);
  float blendF = smoothstep(0, 1, _filter);
  blendF *= blendF;
  col = vec4(blendF);
  //col = vec4(contrast);
  col.a = 1.0f;
  return vec4(1.0f);
}

void main() {
  ivec2 uv_scaleF = textureSize(input_image, 0);
  uv_raw = ivec2(floor(uv.x * uv_scaleF.x), floor(uv.y * uv_scaleF.y));
  fxaa();
  col = texture(input_image, uv);
}
