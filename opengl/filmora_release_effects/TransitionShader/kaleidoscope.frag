#ifdef GL_ES
precision highp float;
#endif
// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

float speed = 1.0;
float angle = 2.0;
float power = 2.0;

vec4 FUNCNAME(vec2 tc) {
  vec2 p = gl_FragCoord.xy / resolution.xy;
  vec2 q = p;
  float t = pow(progress, power)*speed;
  p = p -0.5;
  for (int i = 0; i < 7; i++) {
    p = vec2(sin(t)*p.x + cos(t)*p.y, sin(t)*p.y - cos(t)*p.x);
    t += angle;
    p = abs(mod(p, 2.0) - 1.0);
  }
  abs(mod(p, 1.0));
  gl_FragColor = mix(
    mix(INPUT1(q), INPUT2(q), progress),
    mix(INPUT1(p), INPUT2(p), progress), 1.0 - 2.0*abs(progress - 0.5));
	
  return gl_FragColor;
}
