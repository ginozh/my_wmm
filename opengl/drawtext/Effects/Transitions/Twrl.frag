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

bool inBounds (vec2 p) {
  float xMax = resolution.x / resolution.y / 2.0;
  return all(lessThan(vec2(-xMax, -0.5), p)) && all(lessThan(p, vec2(xMax, 0.5)));
}
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
	vec2 r = vec2(gl_FragCoord.xy - 0.5*resolution.xy)/resolution.y;
  float xMax = resolution.x / resolution.y;
	vec2 q;
	float angle;
	
	gl_FragColor = vec4(0.0,0.0,0.0,1.0);
	if(progress < 0.5)
	{
	  angle = progress * 10.*3.1415926; 
  
	  q.x =   cos(angle)*r.x + sin(angle)*r.y;
	  q.y = - sin(angle)*r.x + cos(angle)*r.y;
	  q = q * vec2(1.0 + progress *50.);
	  
	  if(inBounds(q))
	  {
      vec2 rp = q+vec2(xMax / 2., 0.5);
      vec2 p=correctCoord(vec2(rp.x / xMax, rp.y), tc);
      gl_FragColor = texture2D(mSample0, p);
	  }
    
	}
	else
	{
	  angle = (1.0 - progress) * 10.*3.1415926; 
  
	  q.x =   cos(angle)*r.x + sin(angle)*r.y;
	  q.y = - sin(angle)*r.x + cos(angle)*r.y;
	  
	  q = q * vec2(50.0 - progress *49.);
	  
	  if(inBounds(q))
	  {
      vec2 rp = q+vec2(xMax / 2., 0.5);
      vec2 p=correctCoord(vec2(rp.x / xMax, rp.y), tc);
      gl_FragColor = texture2D(mSample1, p);
	  }
	}
}
