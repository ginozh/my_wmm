#ifdef GL_ES
precision highp float;
#endif

// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

vec4 FUNCNAME(vec2 tc) {
  vec2 p =  vec2(gl_FragCoord.xy - 0.5*resolution.xy)/max(resolution.x,resolution.y);
  float t = smoothstep(progress-0.15,progress+0.15, length(p));
  return mix(INPUT2(tc), INPUT1(tc), t);
}
