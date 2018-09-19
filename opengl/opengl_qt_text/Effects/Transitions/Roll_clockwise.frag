#ifdef GL_ES
precision highp float;
#endif

#define PI 3.141592653589

// General parameters
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;

void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
  vec2 rp = p;
  float a = atan(rp.y, rp.x);
  float pa = (1.0-progress)*PI*1.0-PI*0.50;
  vec4 fromc = texture2D(mSample0, p);
  vec4 toc = texture2D(mSample1, p);
  if(a>pa) {
    gl_FragColor = mix(fromc, toc, smoothstep(0., 1., (a-pa)));
  }else
  {
	gl_FragColor = fromc;
	}
}
