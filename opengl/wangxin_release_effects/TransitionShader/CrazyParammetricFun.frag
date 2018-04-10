#ifdef GL_ES
precision highp float;
#endif
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

// default a = 4
float a = 4.0;
// default b = 1
float b = 1.0;
// default amplitude = 120
float amplitude = 120.0;
// default smoothness = 0.1
float smoothness = 0.1;

vec4 FUNCNAME(vec2 tc) {
  vec2 p = tc;
  vec2 dir = p - vec2(.5);
  float dist = length(dir);
  float x = (a - b) * cos(progress) + b * cos(progress * ((a / b) - 1.) );
  float y = (a - b) * sin(progress) - b * sin(progress * ((a / b) - 1.));
  vec2 offset = dir * vec2(sin(progress  * dist * amplitude * x), sin(progress * dist * amplitude * y)) / smoothness;
  return mix(INPUT1(p + offset), INPUT2(p), smoothstep(0.2, 1.0, progress));
}
