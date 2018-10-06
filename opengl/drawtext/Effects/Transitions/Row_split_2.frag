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
  
  float y = tc.y;  
  float alpha = 0.0;
  vec2 p;
  vec4 f,t;
  
  
  f = texture2D(mSample0, tc);
  t = texture2D(mSample1, tc);
  
  alpha = step(0.5,progress);  
  y = abs(y - 0.5);
  progress = min(progress, 1.0 - progress);  
  if(y < progress) 
  {
    alpha = y * 2.0;
  }
  gl_FragColor = mix(f, t, alpha);
}
