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
const float strength=0.1;

void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
  vec4 ca = texture2D(mSample0, p);
  vec4 cb = texture2D(mSample1, p);
  
  vec2 oa = (((ca.rg+ca.b)*0.5)*2.0-1.0);
  vec2 ob = (((cb.rg+cb.b)*0.5)*2.0-1.0);
  vec2 oc = mix(oa,ob,0.5)*strength;
  
  float w0 = progress;
  float w1 = 1.0-w0;
  gl_FragColor = mix(texture2D(mSample0, p+oc*w0), texture2D(mSample1, p-oc*w1), progress);
}
