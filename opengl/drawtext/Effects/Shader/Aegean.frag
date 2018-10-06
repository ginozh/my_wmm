/*{
	"GUID":"154FD34E-0DF8-481f-A99C-4913DDC4938A",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"TuneMap_BeachCurve.bmp"
		}
	]
}*/

uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

const vec3 vignetteVal = vec3(1.0, 0.6, 0.1);
const vec4 screenVal = vec4(0.1, 0.1, 0.0, 1.0);
const float deSaturate = 0.0;
const int bNeedMerge = 0;


vec2 u_zoomRatio = vec2(1.2, u_resolution.y / u_resolution.x);

void main() 
{
	vec2 tc = textureCoord;
    vec4 tuneColor = texture2D(mSample0, tc);
    float maxTexDist = 0.5 * 1.4142;
    float circleRadius = 0.45;
    float originalA = tuneColor.a;
    
    //1st: Add vignette
    if(vignetteVal.r > 0.0)
    {
        vec2 zoomRatio = 1.0 / u_zoomRatio;
        vec2 startLocTex = (1.0 - zoomRatio) / 2.0;
        vec2 uniformTexCoord = (tc - startLocTex) / zoomRatio;
        
        float d = distance(uniformTexCoord, vec2(0.5, 0.5));
        if(d >= circleRadius) //Do screen-over for outter pixel round a circle
        {
#if 0
            float interpolateLen = maxTexDist - circleRadius; //0.5 * sqrt(2)
            float relativeDist = d - circleRadius;
            float alpha = 1.0 - (relativeDist / interpolateLen) * vignetteVal.g; //alpha = 1.0 - curDist / interpolationLen * blackVal
#else //Use the smooth blend function of glsl
            
            float interpolateVal = smoothstep(circleRadius, maxTexDist, d);
            float alpha = 1.0 - interpolateVal * vignetteVal.g;
#endif
            
            tuneColor = tuneColor * alpha;
        }
        else
        {
#if 0
            float alpha = (1.0 - d / circleRadius) * vignetteVal.b;
#else //Use the smooth blend function of glsl
            
            float interpolateVal = smoothstep(0.0, circleRadius, d);
            float alpha = (1.0 - interpolateVal) * vignetteVal.b;
#endif
            
            tuneColor = tuneColor + alpha; //cO = cA * FA + cB * FB, cA = (1.0, 1.0, 1.0), FA = 1, FB = 1, cB is the source value
            tuneColor = min(tuneColor, vec4(1.0)); //Since the value may be larger than 1.0
        }
    }
    
    //2nd: Do deSaturate or color curve mapping
    if(deSaturate > 0.0)
    {
        float average = (tuneColor.r + tuneColor.g + tuneColor.b) / 3.0;
        tuneColor = vec4(average) * deSaturate + tuneColor * (1.0 - deSaturate);
    }
    else
    {
        tuneColor.r = texture2D(mSample1, vec2(tuneColor.r, .16666)).r;
		tuneColor.g = texture2D(mSample1, vec2(tuneColor.g, .5)).g;
		tuneColor.b = texture2D(mSample1, vec2(tuneColor.b, .83333)).b;
    }
    
    //3rd: Do sreen blend
    if(screenVal.a > 0.0)
    {
        vec4 mergeVal = vec4(screenVal.rgb, 0.0);
        tuneColor = 1.0 - (1.0 - tuneColor) * (1.0 - mergeVal);
    }
    
    //4th: Do merge image blend
    if(bNeedMerge > 0)
    {
        //vec4 mergeVal = texture2D(mSample2, tc);
        //tuneColor = tuneColor * mergeVal;
    }
    tuneColor.a = originalA;
    gl_FragColor = tuneColor;
}
