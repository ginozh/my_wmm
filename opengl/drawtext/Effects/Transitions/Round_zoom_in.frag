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

void main() 
{
	vec2 tc = textureCoord;
  vec2 p =  vec2(gl_FragCoord.xy - 0.5*resolution.xy)/resolution.y;
  float t = smoothstep(0.8-progress-0.15,0.8-progress+0.15, length(p));
  gl_FragColor = mix(texture2D(mSample0, tc), texture2D(mSample1, tc), t);
}
