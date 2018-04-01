/*{
	"GUID":"6DE84CB8-C186-4419-8E88-6EE3B2967660",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"TuneMap_HongTian.bmp"
		},
		{
			"TYPE":"image",
			"FILE":"ColorTuneCircle00.png"
		}
	]
}*/

const int u_blendFlags0 = 1;
const int u_blendFlags1 = 0;
const int u_blendFlags2 = 1;
const int u_blendFlags3 = 0;
const int u_blendFlags4 = 1;
const float u_blendFlags5 = 0.6;

//Do overlay using the golden code from gles orange book
vec4 DoOverlay(vec4 base, vec4 blend)
{
    vec4 result;
    vec4 lumCoeff = vec4(0.0721, 0.7154, 0.2125, 0.0);
    vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
    
    float luminance = dot(base, lumCoeff);
    if (luminance < 0.45)
        result = 2.0 * blend * base;
    else if (luminance > 0.55)
        result = white - 2.0 * (white - blend) * (white - base);
    else 
    {
        vec4 result1 = 2.0 * blend * base;
        vec4 result2 = white - 2.0 * (white - blend) * (white - base);
        result = mix(result1, result2, (luminance - 0.45) * 10.0);
    }
    
    return result;
}

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
    
    if(u_blendFlags0 > 0)
    {
		tuneColor.r = INPUT2(vec2(tuneColor.r, .16666)).r;
		tuneColor.g = INPUT2(vec2(tuneColor.g, .5)).g;
		tuneColor.b = INPUT2(vec2(tuneColor.b, .83333)).b;
    }
    
    int SumFlag = u_blendFlags1 + u_blendFlags2 + u_blendFlags3 + u_blendFlags4;
    
    if(SumFlag > 0)
    {
        vec4 blendColor = INPUT3(tc);
        vec4 srcVal = tuneColor;
        
    #if 1
        if(u_blendFlags1 > 0) //Do overlay 
        {
            tuneColor.r = WS_DoOverlayPerChannel(tuneColor.r, blendColor.r);
            tuneColor.g = WS_DoOverlayPerChannel(tuneColor.g, blendColor.g); 
            tuneColor.b = WS_DoOverlayPerChannel(tuneColor.b, blendColor.b); 
        }
    #else
        if(u_blendFlags1 > 0) //Do overlay 
            tuneColor = DoOverlay(tuneColor, blendColor);
    #endif
        
        if(u_blendFlags2 > 0) //Do multiply
            tuneColor = tuneColor * blendColor;
        
        if(u_blendFlags3 > 0) //Do screen
            tuneColor = 1.0 - ((1.0 - blendColor) * (1.0 - tuneColor));
        
        if(u_blendFlags4 > 0) //Do blend
            tuneColor = mix(srcVal, tuneColor, u_blendFlags5);
    }
    
    return vec4(tuneColor.r, tuneColor.g, tuneColor.b, 1.0);
}
