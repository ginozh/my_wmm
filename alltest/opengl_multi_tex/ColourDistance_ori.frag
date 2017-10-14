#ifdef GL_ES
precision highp float;
#endif
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

// default interpolationPower = 5;
float interpolationPower = 5.0;

vec4 FUNCNAME(vec2 tc) {
  vec2 p = gl_FragCoord.xy / resolution.xy;
  vec4 fTex = INPUT1(p);
  vec4 tTex = INPUT2(p);
  gl_FragColor = mix(distance(fTex,tTex)>progress?fTex:tTex,
                     tTex,
                     pow(progress,interpolationPower));
  
  return gl_FragColor;
}
