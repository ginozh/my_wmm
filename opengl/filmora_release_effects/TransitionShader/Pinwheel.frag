#ifdef GL_ES
precision highp float;
#endif

// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

vec4 FUNCNAME(vec2 tc) {
  vec2 p = tc;
  
  float circPos = atan(p.y - 0.5, p.x - 0.5) + (2.0-progress*2.0);
  float modPos = mod(circPos, 3.1415 / 2.);
  float signed = sign((2.0-progress*2.0) - modPos);
  float smoothed = smoothstep(0., 1., signed);
  
  if (smoothed < 0.5){
    return INPUT2(p);
  } else {
    return INPUT1(p);
  }
}
