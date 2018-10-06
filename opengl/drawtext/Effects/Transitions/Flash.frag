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

float flashPhase = 0.8; 
float flashIntensity = 1.0;
float flashZoomEffect = 0.5;
 
const vec3 flashColor = vec3(0.8, 0.8, 0.3);
const float flashVelocity = 3.0;

void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
  //vec4 fc = texture2D(mSample0, p);
  //vec4 tc = texture2D(mSample1, p);
  float intensity = mix(1.0, 2.0*distance(p, vec2(0.5, 0.5)), flashZoomEffect) * flashIntensity * pow(smoothstep(flashPhase, 0.0, distance(0.5, progress)), flashVelocity);
  vec4 c = mix(texture2D(mSample0, p), texture2D(mSample1, p), smoothstep(0.5*(1.0-flashPhase), 0.5*(1.0+flashPhase), progress));
  c += intensity * vec4(flashColor, 1.0);
  gl_FragColor = c;
}
