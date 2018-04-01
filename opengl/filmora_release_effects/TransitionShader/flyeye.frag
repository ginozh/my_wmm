#ifdef GL_ES
precision highp float;
#endif
 
// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;
 
// Custom parameters
float size = 0.04;
float zoom = 30.0;
float colorSeparation = 0.3;
 
vec4 FUNCNAME(vec2 tc) {
  vec2 p = tc;
  float inv = 1. - progress;
  vec2 disp = size*vec2(cos(zoom*p.x), sin(zoom*p.y));
  vec4 texTo = INPUT2(p + inv*disp);
  vec4 texFrom = vec4(
    INPUT1(p + progress*disp*(1.0 - colorSeparation)).r,
    INPUT1(p + progress*disp).g,
    INPUT1(p + progress*disp*(1.0 + colorSeparation)).b,
    1.0);
  return texTo*progress + texFrom*inv;
}
