/*{
	"GUID":"986D9EB4-F480-4757-B661-10C87DB2EFCB"
}*/
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

vec2 u_zoomRatio = vec2(1.0,1.0);

//.rgb are in range (0.0, 1.0);
vec4 RGBA2HSVA(vec4 inRGBA)
{
    float rgbMax = max(max(inRGBA.r, inRGBA.g), inRGBA.b);
    float rgbMin = min(min(inRGBA.r, inRGBA.g), inRGBA.b);
    float diff = rgbMax - rgbMin;
    
    vec4 outHSVA;
    
    outHSVA.a = inRGBA.a;
    outHSVA.b = rgbMax; //.v
    
    return outHSVA;
}

vec4 HSVA2RGBA(vec4 inHSVA) //.r = .h, .g = .s, .b = .v
{
    vec4 outRGBA = vec4(1.0);
    
    outRGBA.a = inHSVA.a;
    
    if(inHSVA.g <= 0.00001)
    {
        outRGBA.rgb = vec3(inHSVA.b);
    }
    else
    {
        float h = inHSVA.r / 60.0;
        float branch = floor(h);
        float f = h - branch;
        
        float p = inHSVA.b * (1.0 - inHSVA.g);
        float q = inHSVA.b * (1.0 - f * inHSVA.g);
        float t = inHSVA.b * (1.0 - (1.0 - f) * inHSVA.g);
        
        if(branch == 0.0)
        {
            outRGBA.rgb = vec3(inHSVA.b, t, p); 
        }
        else if(branch == 1.0)
        {
            outRGBA.rgb = vec3(q, inHSVA.b, p);
        }
        else if(branch == 2.0)
        {
            outRGBA.rgb = vec3(p, inHSVA.b, t);
        }
        else if(branch == 3.0)
        {
            outRGBA.rgb = vec3(p, q, inHSVA.b);
        }
        else if(branch == 4.0)
        {
            outRGBA.rgb = vec3(t, p, inHSVA.b);
        }
        else //(branch == 5.0)
        {
            outRGBA.rgb = vec3(inHSVA.b, p, q);
        }
    }

    return outRGBA;
}

void main()
{
	vec2 tc = textureCoord;
    vec2 uv = tc;
    vec4 inBGRA = texture2D(mSample0, uv);
    vec4 inHSVA = RGBA2HSVA(inBGRA);
    
    vec2 zoomRatio = 1.0 / u_zoomRatio;
    float startLocTexY = (1.0 - zoomRatio.g) / 2.0;
    float uniformTexCoord = (uv.g - startLocTexY) / zoomRatio.g;
    
    inHSVA.r = 30.0 + (1.0 - uniformTexCoord) * 90.0;
    inHSVA.g = 1.0;
    
    inBGRA.rgb = HSVA2RGBA(inHSVA).rgb;
	gl_FragColor = inBGRA;
}
