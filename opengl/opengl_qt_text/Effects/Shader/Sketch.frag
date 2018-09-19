/*{
	"GUID":"04E1E803-CB09-49cf-91C9-071B1BF67315"
}*/

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

uniform vec2 u_offset[9];

//This sketch algorithm is our own inverted method of sketch
//Sketch
void main()
{
	vec2 tc = textureCoord;
  vec4 tempColor;   
  float sample[9];
  float maxGray = 0.0;
  for(int i=0; i < 9; i++)
  {
    tempColor = texture2D(mSample0, tc + u_offset[i]);
    sample[i] = dot(tempColor.rgb, vec3(0.114, 0.587, 0.299));
    maxGray = max(sample[i], maxGray);
  }
  gl_FragColor = vec4(vec3(sample[4]/maxGray),1.0);
}
