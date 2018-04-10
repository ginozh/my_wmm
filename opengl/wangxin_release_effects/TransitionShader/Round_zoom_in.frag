#ifdef GL_ES
precision highp float;
#endif

// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

vec4 FUNCNAME(vec2 tc) {
  vec2 p =  vec2(gl_FragCoord.xy - 0.5*resolution.xy)/max(resolution.x,resolution.y);
  float t = smoothstep(0.8-progress-0.15,0.8-progress+0.15, length(p));
  return mix(INPUT1(tc), INPUT2(tc), t);
}
