/*{
	"GUID":"04E1E803-CB09-49cf-91C9-071B1BF67315"
}*/

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 PREFIX(offset)[9];

//This sketch algorithm is our own inverted method of sketch
//Sketch
vec4 FUNCNAME(vec2 tc) 
{
  vec4 tempColor;   
  float sample[9];
  float maxGray = 0.0;
  for(int i=0; i < 9; i++)
  {
    tempColor = INPUT(tc + PREFIX(offset)[i]);
    sample[i] = dot(tempColor.rgb, vec3(0.114, 0.587, 0.299));
    maxGray = max(sample[i], maxGray);
  }
  return vec4(vec3(sample[4]/maxGray),1.0);
}
