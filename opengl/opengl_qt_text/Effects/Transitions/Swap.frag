#ifdef GL_ES
precision mediump float;
#endif
 
// General parameters
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;
 
float reflection = 0.4;
float perspective = 0.2;
float depth = 3.0;
 
const vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
const vec2 boundMin = vec2(0.0, 0.0);
const vec2 boundMax = vec2(1.0, 1.0);
 
bool inBounds (vec2 p) {
  return all(lessThan(boundMin, p)) && all(lessThan(p, boundMax));
}
 
vec2 project (vec2 p) {
  return p * vec2(1.0, -1.2) + vec2(0.0, -0.02);
}
 
vec4 bgColor (vec2 p, vec2 pfr, vec2 pto) {
  vec4 c = black;
  pfr = project(pfr);
  if (inBounds(pfr)) {
    c += mix(black, texture2D(mSample0, pfr), reflection * mix(1.0, 0.0, pfr.y));
	if(pfr.y <= 0.01)
    {
      float a = smoothstep(0., 0.01, pfr.y);
	  c = mix(vec4(vec3(0.0), 1.0), c, a);
    }
  }
  pto = project(pto);
  if (inBounds(pto)) {
    c += mix(black, texture2D(mSample1, pto), reflection * mix(1.0, 0.0, pto.y));
	if(pto.y <= 0.01)
    {
      float a = smoothstep(0., 0.01, pto.y);
	  c = mix(vec4(vec3(0.0), 1.0), c, a);
    }
  }
  return c;
}
 
void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
 
  vec2 pfr, pto = vec2(-1.);
 
  float size = mix(1.0, depth, progress);
  float persp = perspective * progress;
  pfr = (p + vec2(-0.0, -0.5)) * vec2(size/(1.0-perspective*progress), size/(1.0-size*persp*p.x)) + vec2(0.0, 0.5);
 
  size = mix(1.0, depth, 1.-progress);
  persp = perspective * (1.-progress);
  pto = (p + vec2(-1.0, -0.5)) * vec2(size/(1.0-perspective*(1.0-progress)), size/(1.0-size*persp*(0.5-p.x))) + vec2(1.0, 0.5);
 
  bool fromOver = progress < 0.5;
 
 float a = 1.0;
  if (fromOver) {
    if (inBounds(pfr)) {
	  if (pfr.y >= 0.99)
	  {
        a = 1.0 - smoothstep(0.99, 1.0, pfr.y);
      }
      else if(pfr.y <= 0.01)
      {
        a = smoothstep(0., 0.01, pfr.y);
      }
      gl_FragColor = mix(vec4(vec3(0.0), 1.0), texture2D(mSample0, pfr), a);
    }
    else if (inBounds(pto)) {
	  if (pto.y >= 0.99)
	  {
        a = 1.0 - smoothstep(0.99, 1.0, pto.y);
      }
      else if(pto.y <= 0.01)
      {
        a = smoothstep(0., 0.01, pto.y);
      }
      gl_FragColor = mix(vec4(vec3(0.0), 1.0), texture2D(mSample1, pto), a);
    }
    else {
      gl_FragColor = bgColor(p, pfr, pto);
    }
  }
  else {
    if (inBounds(pto)) {
      if (pto.y >= 0.99)
	  {
        a = 1.0 - smoothstep(0.99, 1.0, pto.y);
      }
      else if(pto.y <= 0.01)
      {
        a = smoothstep(0., 0.01, pto.y);
      }
      gl_FragColor = mix(vec4(vec3(0.0), 1.0), texture2D(mSample1, pto), a);
    }
    else if (inBounds(pfr)) {
      if (pfr.y >= 0.99)
	  {
        a = 1.0 - smoothstep(0.99, 1.0, pfr.y);
      }
      else if(pfr.y <= 0.01)
      {
        a = smoothstep(0., 0.01, pfr.y);
      }
      gl_FragColor = mix(vec4(vec3(0.0), 1.0), texture2D(mSample0, pfr), a);
    }
    else {
      gl_FragColor = bgColor(p, pfr, pto);
    }
  }
}
