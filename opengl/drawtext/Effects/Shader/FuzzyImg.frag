/*{
	"GUID":"004AF9BF-A233-4f3a-A14F-2E475C732091",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"FuncMap_Fuzzy.bmp"
		}
	]
}*/

uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

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

void main()
{
	vec2 tc = textureCoord;
    vec4 tuneColor = texture2D(mSample0, tc);
    vec4 blendColor = tuneColor;
	
    tuneColor.r = texture2D(mSample1, vec2(tuneColor.r, .16666)).r;
	tuneColor.g = texture2D(mSample1, vec2(tuneColor.g, .5)).g;
	tuneColor.b = texture2D(mSample1, vec2(tuneColor.b, .83333)).b;
    
    tuneColor.r = WS_DoOverlayPerChannel(tuneColor.r, blendColor.r);
    tuneColor.g = WS_DoOverlayPerChannel(tuneColor.g, blendColor.g); 
    tuneColor.b = WS_DoOverlayPerChannel(tuneColor.b, blendColor.b); 
    
    tuneColor.r = texture2D(mSample1, vec2(tuneColor.r, .16666)).r;
	tuneColor.g = texture2D(mSample1, vec2(tuneColor.g, .5)).g;
	tuneColor.b = texture2D(mSample1, vec2(tuneColor.b, .83333)).b;	
    
    gl_FragColor = tuneColor;
}
