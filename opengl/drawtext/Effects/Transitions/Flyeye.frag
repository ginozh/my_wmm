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
 
// Custom parameters
float size = 0.04;
float zoom = 30.0;
float colorSeparation = 0.3;
 
void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
  float inv = 1. - progress;
  vec2 disp = size*vec2(cos(zoom*p.x), sin(zoom*p.y));
  vec4 texTo = texture2D(mSample1, p + inv*disp);
  vec4 texFrom = vec4(
    texture2D(mSample0, p + progress*disp*(1.0 - colorSeparation)).r,
    texture2D(mSample0, p + progress*disp).g,
    texture2D(mSample0, p + progress*disp*(1.0 + colorSeparation)).b,
    1.0);
  gl_FragColor = texTo*progress + texFrom*inv;
}
