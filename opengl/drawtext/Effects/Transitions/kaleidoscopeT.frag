#ifdef GL_ES
precision highp float;
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

float speed = 1.0;
float angle = 2.0;
float power = 2.0;

void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
  vec2 q = p;
  float t = pow(progress, power)*speed;
  p = p -0.5;
  for (int i = 0; i < 7; i++) {
    p = vec2(sin(t)*p.x + cos(t)*p.y, sin(t)*p.y - cos(t)*p.x);
    t += angle;
    p = abs(mod(p, 2.0) - 1.0);
  }
  abs(mod(p, 1.0));
  gl_FragColor = mix(
    mix(texture2D(mSample0, q), texture2D(mSample1, q), progress),
    mix(texture2D(mSample0, p), texture2D(mSample1, p), progress), 1.0 - 2.0*abs(progress - 0.5));
	
  gl_FragColor = gl_FragColor;
}
