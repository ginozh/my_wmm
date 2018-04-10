#ifdef GL_ES
precision highp float;
#endif
float progress = PREFIX(global_time);
vec2 resolution = iResolution;
// default amplitude = 1.0
float amplitude = 1.0;
// default waves = 30.
float waves = 30.0;
// default colorSeparation = 0.3
float colorSeparation = 0.3;
float PI = 3.14159265358979323846264;
float compute(vec2 p, float progress, vec2 center) {
vec2 o = p*sin(progress * amplitude)-center;
// horizontal vector
vec2 h = vec2(1., 0.);
// butterfly polar function (don't ask me why this one :))
float theta = acos(dot(o, h)) * waves;
return (exp(cos(theta)) - 2.*cos(4.*theta) + pow(sin((2.*theta - PI) / 24.), 5.)) / 10.;
}
vec4 FUNCNAME(vec2 tc) {
  vec2 p = tc;
  float inv = 1. - progress;
  vec2 dir = p - vec2(.5);
  float dist = length(dir);
  float disp = compute(p, progress, vec2(0.5, 0.5)) ;
  vec4 texTo = INPUT2(p + inv*disp);
  vec4 texFrom = vec4(
  INPUT1(p + progress*disp*(1.0 - colorSeparation)).r,
  INPUT1(p + progress*disp).g,
  INPUT1(p + progress*disp*(1.0 + colorSeparation)).b,1.0);
  return texTo*progress + texFrom*inv;
}
