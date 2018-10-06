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
  vec2 p = tc;
  
  float circPos = atan(p.y - 0.5, p.x - 0.5) + (2.0-progress*2.0);
  float modPos = mod(circPos, 3.1415 / 2.);
  float fsigned = sign((2.0-progress*2.0) - modPos);
  float smoothed = smoothstep(0., 1., fsigned);
  
  if (smoothed < 0.5){
    gl_FragColor = texture2D(mSample1, p);
  } else {
    gl_FragColor = texture2D(mSample0, p);
  }
}
