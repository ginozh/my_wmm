#ifdef GL_ES
precision highp float;
#endif
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;
#define PI 3.141592653589

void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
  
  vec2 q;
  float angle = - PI * 0.5;
  q.x =   cos(angle)*p.x + sin(angle)*p.y;
  q.y = - sin(angle)*p.x + cos(angle)*p.y;
  q = q - vec2(-1.0, 0.0);
  
  vec2 rp = (q*2. - 1.);
  float a = atan(rp.y, rp.x);
  float pa = (progress*PI*2.5-PI*1.25);
  
  vec4 fromc = texture2D(mSample0, p);
  vec4 toc = texture2D(mSample1, p);
  
  if(a>pa) {
    gl_FragColor = mix(toc, fromc, smoothstep(0., 1., (a-pa)));
  } else {
    gl_FragColor = toc;
  }
}
