/*{
	"GUID":"004AF9BF-A233-4f3a-A14F-2E475C732091",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"FuncMap_Fuzzy.bmp"
		}
	]
}*/

float WS_DoOverlayPerChannel(float baseVal, float blendVal)
{
    float result;
    
    if(baseVal < 0.5)
    {
        result = 2.0 * baseVal * blendVal;
    }
    else
        result = 1.0 - 2.0 * (1.0 - baseVal) * (1.0 - blendVal);
    
    return result;
}




vec4 FUNCNAME(vec2 tc) 
{
    vec4 tuneColor = INPUT1(tc);
    vec4 blendColor = tuneColor;
	
    tuneColor.r = INPUT2(vec2(tuneColor.r, .16666)).r;
	tuneColor.g = INPUT2(vec2(tuneColor.g, .5)).g;
	tuneColor.b = INPUT2(vec2(tuneColor.b, .83333)).b;
    
    tuneColor.r = WS_DoOverlayPerChannel(tuneColor.r, blendColor.r);
    tuneColor.g = WS_DoOverlayPerChannel(tuneColor.g, blendColor.g); 
    tuneColor.b = WS_DoOverlayPerChannel(tuneColor.b, blendColor.b); 
    
    tuneColor.r = INPUT2(vec2(tuneColor.r, .16666)).r;
	tuneColor.g = INPUT2(vec2(tuneColor.g, .5)).g;
	tuneColor.b = INPUT2(vec2(tuneColor.b, .83333)).b;	
    
    return vec4(tuneColor.r, tuneColor.g, tuneColor.b, 1.0);
}
