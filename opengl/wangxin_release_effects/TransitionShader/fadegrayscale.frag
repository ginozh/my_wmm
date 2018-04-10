#ifdef GL_ES
precision highp float;
#endif
 
// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;
 
float grayPhase = 0.3; // if 0.0, the image directly turn grayscale, if 0.9, the grayscale transition phase is very important
 
vec3 grayscale (vec3 color) {
  return vec3(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);
}
 
vec4 FUNCNAME(vec2 tc) {
  vec2 p = tc;
  vec4 fc = INPUT1(p);
  vec4 tc1 = INPUT2(p);
  gl_FragColor = mix(
    mix(vec4(grayscale(fc.rgb), 1.0), INPUT1(p), smoothstep(1.0-grayPhase, 0.0, progress)),
    mix(vec4(grayscale(tc1.rgb), 1.0), INPUT2(p), smoothstep(    grayPhase, 1.0, progress)),
    progress);
	
  return gl_FragColor;
}
