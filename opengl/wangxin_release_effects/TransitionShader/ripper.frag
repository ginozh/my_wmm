#ifdef GL_ES
precision highp float;
#endif
 
// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;
 
float amplitude = 100.0;
float speed = 50.0;
 
vec4 FUNCNAME(vec2 tc)
{
  vec2 p = tc;
  vec2 dir = p - vec2(.5);
  float dist = length(dir);
  vec2 offset = dir * (sin(progress * dist * amplitude - progress * speed) + .5) / 30.;
  return mix(INPUT1(p + offset), INPUT2(p), smoothstep(0.2, 1.0, progress));
}
