#ifdef GL_ES
precision highp float;
#endif
float progress = PREFIX(global_time);
vec2 resolution = iResolution;
float curPos = PREFIX(current_pos);

vec4 FUNCNAME(vec2 tc)
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    
    vec4 y = INPUT1(uv);
    vec4 y1 = INPUT1(uv);
    vec4 y2 = INPUT2(uv);
    
    vec2 centre = resolution.xy / 2.0;
    float nT = 8.0;
    float PI = 3.141592653589793;
    float t = curPos;
    float dl = length(resolution);
    float l = dl / (1.5 * nT);
    float phi = t *  8.0 * PI;
    
    float mt = 1.0 - t / 100.0;
    float R = phi * l / (2.0 * PI);
    float R_sqrt = R * R + 0.5;
    
    if(R_sqrt < 0.0)
    {
       gl_FragColor = vec4(1.0);
    }
    else
    {
    	float h_d = centre.y - (R + 0.5);
    	h_d = max(h_d, 0.0);
    	
    	float w_d = centre.x - (R + 0.5);
        w_d = max(w_d, 0.0);
        
        float tmp_1 = 255.0 * mt / R;
        float tmp_2 = 2.0 * PI / l;
        
   
        float r_sqrt = (centre.x - gl_FragCoord.x) * (centre.x - gl_FragCoord.x) + 
            (centre.y - gl_FragCoord.y) * (centre.y - gl_FragCoord.y);
            
        if(r_sqrt < R_sqrt)
        {
            float r = sqrt(r_sqrt);
            float tmpOffset = 20.0 * r * mt / R * cos(phi - tmp_2 * r);
                
            float u_alpha = tmp_1 * (1.0 - sin(phi - tmp_2 * r)) * r + 0.5;
            u_alpha = clamp(u_alpha, 0.0, 255.0);
			      float alpha = 255.0 - u_alpha;
                
            vec2 newpoint;				              
            newpoint.x = (gl_FragCoord.x + tmpOffset) / resolution.x;
            newpoint.y = (gl_FragCoord.y + tmpOffset) / resolution.y;                                 
                                                 
            vec4 point1 = INPUT1(newpoint) ;
            vec4 point2 = INPUT2(newpoint);  
            y = mix(point1,point2, alpha / 255.);
         }
            
        gl_FragColor = y;
    }
  return gl_FragColor;
}
