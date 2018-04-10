#ifdef GL_ES
precision mediump float;
#endif
 
// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;
const float strength=0.1;

vec4 FUNCNAME(vec2 tc) {
  vec2 p = gl_FragCoord.xy / resolution.xy;
  vec4 ca = INPUT1(p);
  vec4 cb = INPUT2(p);
  
  vec2 oa = (((ca.rg+ca.b)*0.5)*2.0-1.0);
  vec2 ob = (((cb.rg+cb.b)*0.5)*2.0-1.0);
  vec2 oc = mix(oa,ob,0.5)*strength;
  
  float w0 = progress;
  float w1 = 1.0-w0;
  return mix(INPUT1(p+oc*w0), INPUT2(p-oc*w1), progress);
}
