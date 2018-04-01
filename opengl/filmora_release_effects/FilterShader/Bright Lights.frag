/*{
	"GUID":"740E5BD0-1BAD-4688-AF49-946FAE0F2E7A",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"TuneMap_Freshness.bmp"
		},
		{
			"TYPE":"image",
			"FILE":"FuncMap_FreshExposure.bmp"
		}
	]
}*/

#define MAKE_GREY(b, g, r) ((29.0 * (b) + 150.0 * (g) + 76.0 * (r) + 255.0) / 256.0)
#define ChannelBlend_SoftLight(B,L)  (((L < 128.0)?(2.0*((B / 2.0)+64.0)*L)/255.0:(255.0-(2.0*(255.0-((B / 2.0)+64.0))*(255.0-L)/255.0))))

vec4 FUNCNAME(vec2 tc) 
{
	vec4 col = INPUT1(tc);
    col = col * vec4(255.0, 255.0, 255.0, 255.0);
    
    float rtemp, gtemp, btemp, gray;
    btemp = abs(col.b - 17.0);
    gtemp = abs(col.g - 39.0);
    rtemp = abs(col.r - 135.0);
    
    col.b = (92.0 * btemp + 163.0 * col.b) / 256.0;
	col.g = (92.0 * gtemp + 163.0 * col.g) / 256.0;
	col.r = (92.0 * rtemp + 163.0 * col.r) / 256.0;
    
    gray = MAKE_GREY(col.b, col.g, col.r);
    
    rtemp = ChannelBlend_SoftLight(INPUT2(vec2(gray/256.0, 0.1666)).r * 256.0, col.r);
	gtemp = ChannelBlend_SoftLight(INPUT2(vec2(gray/256.0, 0.5)).g * 256.0, col.g);
	btemp = ChannelBlend_SoftLight(INPUT2(vec2(gray/256.0, 0.8333)).b * 256.0, col.b);
    
    col.r = INPUT3(vec2(rtemp/256.0, 0.1666)).r;
	col.g = INPUT3(vec2(gtemp/256.0, 0.1666)).r;
	col.b = INPUT3(vec2(btemp/256.0, 0.1666)).r;

	return col;
}
