#ifdef GL_ES
precision highp float;
#endif
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

bool inBounds (vec2 p) {
  float xMax = resolution.x / resolution.y / 2.0;
  return all(lessThan(vec2(-xMax, -0.5), p)) && all(lessThan(p, vec2(xMax, 0.5)));
}

vec4 FUNCNAME(vec2 tc)
{
	vec2 r = vec2(gl_FragCoord.xy - 0.5*resolution.xy)/resolution.y;
  float xMax = resolution.x / resolution.y;
	vec2 q;
	float angle;
	if(progress < 0.5)
	{
	  angle = progress * 10.*3.1415926; 
  
	  q.x =   cos(angle)*r.x + sin(angle)*r.y;
	  q.y = - sin(angle)*r.x + cos(angle)*r.y;
	  q = q * vec2(1.0 + progress *50.);
	  
	  if(inBounds(q))
	  {
      vec2 rp = q+vec2(xMax / 2., 0.5);
      gl_FragColor = INPUT1(vec2(rp.x / xMax, rp.y));
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
      gl_FragColor = INPUT2(vec2(rp.x / xMax, rp.y));
	  }
	}
  return gl_FragColor;
}
