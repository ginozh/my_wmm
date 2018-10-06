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
 
float amplitude = 100.0;
float speed = 50.0;
 
void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
  vec2 dir = p - vec2(.5);
  float dist = length(dir);
  vec2 offset = dir * (sin(progress * dist * amplitude - progress * speed) + .5) / 30.;
  gl_FragColor = mix(texture2D(mSample0, p + offset), texture2D(mSample1, p), smoothstep(0.2, 1.0, progress));
}
