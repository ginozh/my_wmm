/*{
	"GUID":"B23034C1-24C4-4504-9776-6805CFBD7F02",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"sketch.png"
		}
	]
}*/

vec2 u_imgWH = iResolution;
float u_raiusRatio = 0.55;

vec4 Screen(vec4 base, vec4 blend)
{
    vec4 white = vec4(1.0);
    vec4 result = white - ((white - blend) * (white - base));
    return result;
}

vec4 FUNCNAME(vec2 tc) 
{
    float width = u_imgWH.r;
    float height = u_imgWH.g;
    float maxHalfWH = max(width/2.0, height/2.0);
    
    float innerRadius = max(u_raiusRatio-0.4, 0.0) * maxHalfWH;
    float outterRadius = (1.1 * u_raiusRatio + 0.1) * maxHalfWH; 
    
    float innerRadius2 = innerRadius * innerRadius;
    float outterRadius2 = outterRadius * outterRadius;
    
    float curDistX = (tc.x - 0.5) * width;
    float curDistY = (tc.y - 0.5) * height;
    float curDist2 = curDistX * curDistX + curDistY * curDistY;
    float alpha = 0.0;
    
    if(curDist2 > outterRadius2)
    {
        alpha = 0.0; //black
    }
    else
    {
        float curDist = sqrt(curDist2);
        
        innerRadius = outterRadius - innerRadius;
        alpha = (outterRadius - curDist) / innerRadius;
    }
    
    vec4 base = INPUT1(tc);
    float gray = dot(base.rgb, vec3(0.114, 0.587, 0.299));    
    vec4 blend = INPUT2(tc);
    base = Screen(vec4(vec3(gray), 1.0), blend);
    alpha = 1.0 - alpha;
    base = mix(base, blend, alpha);
    return base;
}
