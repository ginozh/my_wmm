/*{
	"GUID":"83A3EBFF-0A46-465b-B201-1E8A7DD5DA1B",
  "PARAMS":[
    {
      "NAME":"swing",
      "TYPE":"float",
      "DEFAULT":0.3,
      "MIN":0.0,
      "MAX":1.0
    },
    {
      "NAME":"frequency",
      "TYPE":"float",
      "DEFAULT":0.41,
      "MIN":0.0,
      "MAX":1.0
    }
  ]
}*/


#define FILTER_PI 3.1415926

vec4 FUNCNAME(vec2 tc) 
{ 
  vec4 col;
  float fre = PREFIX(frequency)/0.2*FILTER_PI/iResolution.x;
  float y = gl_FragCoord.y + PREFIX(swing) * 100.0 * sin(fre * gl_FragCoord.x);
  
  if(y < 0.0 || y >= iResolution.y) 
  {
    col = vec4(0.0, 0.0, 0.0, 1.0);
  }
  else
  {
    col = INPUT(vec2(gl_FragCoord.x / iResolution.x, (gl_FragCoord.y/iResolution.y + y )/ iResolution.y));
  }
  
  return col;
}

