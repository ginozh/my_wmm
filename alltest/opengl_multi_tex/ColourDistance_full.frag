uniform float global_time; // float progress = (p.frameCount % frames) / frames
//uniform vec2 iResolution;
uniform sampler2D from,to;
varying vec2 texCoord;
float PREFIX(float t);
vec4 INPUT1(vec2 p);
vec4 INPUT2(vec2 p);
vec2 iResolution=texCoord;

#ifdef GL_ES
precision highp float;
#endif
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

// default interpolationPower = 5;
float interpolationPower = 5.0;

vec4 FUNCNAME(vec2 tc) {
  vec2 p = gl_FragCoord.xy / resolution.xy;
  vec4 fTex = INPUT1(p);
  vec4 tTex = INPUT2(p);
  gl_FragColor = mix(distance(fTex,tTex)>progress?fTex:tTex,
                     tTex,
                     pow(progress,interpolationPower));
  
  return gl_FragColor;
}

float PREFIX(float t)
{
    return t;
}
vec4 INPUT1(vec2 p)
{
	return texture2D(from, p);
}
vec4 INPUT2(vec2 p)
{
	return texture2D(to, p);
}
void main() {
	gl_FragColor = FUNCNAME(texCoord * 0.5 + 0.5);
}
