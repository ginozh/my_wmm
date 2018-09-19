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

// default a = 4
float a = 4.0;
// default b = 1
float b = 1.0;
// default amplitude = 120
float amplitude = 120.0;
// default smoothness = 0.1
float smoothness = 0.1;

void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
  vec2 dir = p - vec2(.5);
  float dist = length(dir);
  float x = (a - b) * cos(progress) + b * cos(progress * ((a / b) - 1.) );
  float y = (a - b) * sin(progress) - b * sin(progress * ((a / b) - 1.));
  vec2 offset = dir * vec2(sin(progress  * dist * amplitude * x), sin(progress * dist * amplitude * y)) / smoothness;
  gl_FragColor = mix(texture2D(mSample0, p + offset), texture2D(mSample1, p), smoothstep(0.2, 1.0, progress));
}
