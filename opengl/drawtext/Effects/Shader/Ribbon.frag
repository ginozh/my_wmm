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

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_swing;
uniform float u_frequency;

#define FILTER_PI 3.1415926

vec2 correctCoord(vec2 orip, vec2 tc){
    vec2 p = orip;
    if(p.x<0.0) p.x=0.0;
    if(p.y<0.0) p.y=0.0;
    if(p.x>tc.x) p.x=tc.x;
    if(p.y>tc.y) p.y=tc.y;
    return p;
}
void main()
{
	vec2 tc = textureCoord;
	vec2 fragCoord = gl_FragCoord.xy;
  vec4 col;
  float fre = u_frequency/0.2*FILTER_PI/u_resolution.x;
  float y = fragCoord.y + u_swing * 100.0 * sin(fre * fragCoord.x);
  
  if(y < 0.0 || y >= u_resolution.y) 
  {
    col = vec4(0.0, 0.0, 0.0, 1.0);
  }
  else
  {
    vec2 p =vec2(fragCoord.x / u_resolution.x, (fragCoord.y/u_resolution.y + y )/ u_resolution.y) ;
    col = texture2D(mSample0, correctCoord(p, tc));
  }
  
  gl_FragColor = col;
}

