#ifdef GL_ES
precision highp float;
#endif

// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

vec4 FUNCNAME(vec2 tc) {
  
  float y = tc.y;  
  float alpha = 0.0;
  vec2 p;
  vec4 f,t;
  
  
  f = INPUT1(tc);
  t = INPUT2(tc);
  
  alpha = step(0.5,progress);  
  y = abs(y - 0.5);
  progress = min(progress, 1.0 - progress);  
  if(y < progress) 
  {
    alpha = y * 2.0;
  }
  return mix(f, t, alpha);
}
