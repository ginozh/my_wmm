/*{
	"GUID":"08ECFF4C-CD36-4c89-9D4D-F515451BB2DB",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"TuneMap_GloamingCurve.bmp"
		}
	]
}*/

vec4 FUNCNAME(vec2 tc) 
{
    vec4 inBGRA = INPUT1(tc);
    vec4 color = inBGRA;
    vec3 white = vec3(1.0, 1.0, 1.0);

    float lim = 0.0;
    vec3 cmy = white - inBGRA.rgb;
    vec3 cmy1 = vec3(0.0,0.0,0.0);
    vec3 cmy2 = vec3(0.0,0.0,0.0);
    vec3 cmy3 = vec3(0.0,0.0,0.0);

    float maxV = max(max(inBGRA.r, inBGRA.g), inBGRA.b);
    float minV = min(min(inBGRA.r, inBGRA.g), inBGRA.b);
    float midV = max(min(inBGRA.r, inBGRA.g), min(max(inBGRA.r, inBGRA.g), inBGRA.b));
    

    if(maxV == inBGRA.r)
    {
        lim = maxV - midV;
        cmy1.r = min(cmy.r, 0.38) * lim;
        cmy1.g = - min(cmy.g, 0.1) * lim;
        cmy1.b = min(cmy.b, 0.2) * lim;
    }
    

    if (minV == inBGRA.b)
    {
        lim = midV - minV;
        cmy2.r = min(cmy.r, 0.28) * lim;
        cmy2.g = -min(cmy.g, 0.05) * lim;
        cmy2.b = -min(cmy.r, 0.25) * lim;
    }
    
   if(maxV < 0.5)
    {
        lim = 1.0 - maxV - minV;
        cmy3.r = min(cmy.r, 0.06) * lim;
        cmy3.g = -min(cmy.g, 0.07) * lim;
        cmy3.b = min(cmy.r, 0.09) * lim;
    }
    
    if (minV > 0.5) 
    {
        lim = maxV + minV - 1.0;
        cmy3.r = -min(cmy.r, 0.02) * lim;
        cmy3.g = min(cmy.g, 0.03) * lim;
        cmy3.b = min(cmy.r, 0.03) * lim;
    }
    
    color.rgb = clamp(inBGRA.rgb + cmy1 + cmy2 + cmy3, 0.0, 1.0);
	
    color.r = INPUT2(vec2(color.r, .16666)).r;
	color.g = INPUT2(vec2(color.g, .5)).g;
	color.b = INPUT2(vec2(color.b, .83333)).b;
	
    color.a = inBGRA.a;
	return color;
}
