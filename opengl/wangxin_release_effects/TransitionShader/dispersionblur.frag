#ifdef GL_ES
precision highp float;
#endif
 
#define QUALITY 8
 
// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;
 
// Custom parameters
float size = 0.6;
 
const float GOLDEN_ANGLE = 2.399963229728653; // PI * (3.0 - sqrt(5.0))
 
vec4 blur(int fromorto, vec2 c, float radius) {
  vec4 sum = vec4(0.0);
  float q = float(QUALITY);
  // Using a "spiral" to propagate points.
  for (int i=0; i<QUALITY; ++i) {
    float fi = float(i);
    float a = fi * GOLDEN_ANGLE;
    float r = sqrt(fi / q) * radius;
    vec2 p = c + r * vec2(cos(a), sin(a));
	if(0 == fromorto)
	{
		sum += INPUT1(p);
	}
	
	if(1 == fromorto)
	{
		sum += INPUT2(p);
	}
  }
  return sum / q;
}
 
vec4 FUNCNAME(vec2 tc)
{
  vec2 p = tc;
  float inv = 1.-progress;
  return inv*blur(0, p, progress*size) + progress*blur(1, p, inv*size);
}
