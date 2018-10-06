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
// default amplitude = 1.0
float amplitude = 1.0;
// default waves = 30.
float waves = 30.0;
// default colorSeparation = 0.3
float colorSeparation = 0.3;
float PI = 3.14159265358979323846264;
float compute(vec2 p, float progress, vec2 center) {
	vec2 o = p*sin(progress * amplitude)-center;
	// horizontal vector
	vec2 h = vec2(1., 0.);
	// butterfly polar function (don't ask me why this one :))
	float theta = acos(dot(o, h)) * waves;
	float fx = sin((2.*theta - PI) / 24.);
	if (fx <= 0.0)
		fx = 0.1;
	return (exp(cos(theta)) - 2.*cos(4.*theta) + pow(fx, 5.)) / 10.;
}
void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
  float inv = 1. - progress;
  vec2 dir = p - vec2(.5);
  float dist = length(dir);
  float disp = compute(p, progress, vec2(0.5, 0.5)) ;
  vec4 texTo = texture2D(mSample1, p + inv*disp);
  vec4 texFrom = vec4(
  texture2D(mSample0, p + progress*disp*(1.0 - colorSeparation)).r,
  texture2D(mSample0, p + progress*disp).g,
  texture2D(mSample0, p + progress*disp*(1.0 + colorSeparation)).b,1.0);
  gl_FragColor = texTo*progress + texFrom*inv;
}
