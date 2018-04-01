#ifdef GL_ES
precision mediump float;
#endif
 
// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;
 
float reflection = 0.4;
float perspective = 0.4;
float depth = 3.0;
 
const vec4 black = vec4(0.0, 0.0, 0.0, 0.0);
const vec2 boundMin = vec2(0.0, 0.0);
const vec2 boundMax = vec2(1.0, 1.0);
 
bool inBounds (vec2 p) {
  return all(lessThan(boundMin, p)) && all(lessThan(p, boundMax));
}
 
vec2 project (vec2 p) {
  return p * vec2(1.0, -1.2) + vec2(0.0, -0.02);
}
 
vec4 bgColor (vec2 p, vec2 pto) {
  vec4 c = black;
  pto = project(pto);
  if (inBounds(pto)) {
    c += mix(black, INPUT2(pto), reflection * mix(1.0, 0.0, pto.y));
  }
  return c;
}
 
vec4 FUNCNAME(vec2 tc) {
  vec2 p = tc;
 
  vec2 pfr = vec2(-1.), pto = vec2(-1.);
 
  float middleSlit = 2.0 * abs(p.x-0.5) - progress;
  if (middleSlit > 0.0) {
    pfr = p + (p.x > 0.5 ? -1.0 : 1.0) * vec2(0.5*progress, 0.0);
    float d = 1.0/(1.0+perspective*progress*(1.0-middleSlit));
    pfr.y -= d/2.;
    pfr.y *= d;
    pfr.y += d/2.;
  }
 
  float size = mix(1.0, depth, 1.-progress);
  pto = (p + vec2(-0.5, -0.5)) * vec2(size, size) + vec2(0.5, 0.5);
 
  if (inBounds(pfr)) {
    gl_FragColor = INPUT1(pfr);
  }
  else if (inBounds(pto)) {
    gl_FragColor = INPUT2(pto);
  }
  else {
    gl_FragColor = bgColor(p, pto);
  }
  return gl_FragColor;
}
