#ifdef GL_ES
precision highp float;
#endif

#define PI 3.141592653589

// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

vec4 FUNCNAME(vec2 tc) {
  vec2 p = tc;
  vec2 rp = p;
  float a = atan(rp.y, rp.x);
  float pa = progress*PI*1.0-PI*0.50;
  vec4 fromc = INPUT1(p);
  vec4 toc = INPUT2(p);
  if(a>pa) {
    return mix(toc, fromc, smoothstep(0., 1., (a-pa)));
  }
  return toc;
}
