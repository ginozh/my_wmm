#ifdef GL_ES
precision highp float;
#endif
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;   //width, height
uniform float u_global_time; // float progress = (p.frameCount % frames) / frames
uniform float u_total_time;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;

// default interpolationPower = 5;
float interpolationPower = 5.0;

void main() 
{
	vec2 tc = textureCoord;
  vec2 p = gl_FragCoord.xy / resolution.xy;
  vec4 fTex = texture2D(mSample0, p);
  vec4 tTex = texture2D(mSample1, p);
  gl_FragColor =  mix(distance(fTex,tTex)>progress?fTex:tTex,
                     tTex,
                     pow(progress,interpolationPower));
}
