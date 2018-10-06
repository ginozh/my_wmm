#ifdef GL_ES
precision highp float;
#endif
 
#define QUALITY 8
 
// General parameters
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;
 
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
		sum += texture2D(mSample0, p);
	}
	
	if(1 == fromorto)
	{
		sum += texture2D(mSample1, p);
	}
  }
  return sum / q;
}
 
void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
  float inv = 1.-progress;
  gl_FragColor = inv*blur(0, p, progress*size) + progress*blur(1, p, inv*size);
}
