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
 
float grayPhase = 0.3; // if 0.0, the image directly turn grayscale, if 0.9, the grayscale transition phase is very important
 
vec3 grayscale (vec3 color) {
  return vec3(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);
}
 
void main() 
{
	vec2 tc = textureCoord;
  vec2 p = tc;
  vec4 fc = texture2D(mSample0, p);
  vec4 tc1 = texture2D(mSample1, p);
  gl_FragColor = mix(
    mix(vec4(grayscale(fc.rgb), 1.0), texture2D(mSample0, p), smoothstep(1.0-grayPhase, 0.0, progress)),
    mix(vec4(grayscale(tc1.rgb), 1.0), texture2D(mSample1, p), smoothstep(    grayPhase, 1.0, progress)),
    progress);

}
