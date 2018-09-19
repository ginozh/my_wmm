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

uniform sampler2D mSample0;
uniform sampler2D mSample1;
uniform sampler2D mSample2;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

#define MAKE_GREY(b, g, r) ((29.0 * (b) + 150.0 * (g) + 76.0 * (r) + 255.0) / 256.0)

float ChannelBlend_SoftLight(float B, float L)  
{
	return (L < 128.0)?(2.0*((B / 2.0)+64.0)*L)/255.0:(255.0-(2.0*(255.0-((B / 2.0)+64.0))*(255.0-L)/255.0));
}

void main()
{
	vec2 tc = textureCoord;
	vec4 col = texture2D(mSample0, tc);
    col = col * vec4(255.0, 255.0, 255.0, 255.0);
    
    float rtemp, gtemp, btemp, gray;
    btemp = abs(col.b - 17.0);
    gtemp = abs(col.g - 39.0);
    rtemp = abs(col.r - 135.0);
    
    col.b = (92.0 * btemp + 163.0 * col.b) / 256.0;
	col.g = (92.0 * gtemp + 163.0 * col.g) / 256.0;
	col.r = (92.0 * rtemp + 163.0 * col.r) / 256.0;
    
    gray = MAKE_GREY(col.b, col.g, col.r);
    
    rtemp = ChannelBlend_SoftLight(texture2D(mSample1, vec2(gray/256.0, 0.1666)).r * 256.0, col.r);
	gtemp = ChannelBlend_SoftLight(texture2D(mSample1, vec2(gray/256.0, 0.5)).g * 256.0, col.g);
	btemp = ChannelBlend_SoftLight(texture2D(mSample1, vec2(gray/256.0, 0.8333)).b * 256.0, col.b);
    
    col.r = texture2D(mSample2, vec2(rtemp/256.0, 0.1666)).r;
	col.g = texture2D(mSample2, vec2(gtemp/256.0, 0.1666)).r;
	col.b = texture2D(mSample2, vec2(btemp/256.0, 0.1666)).r;

	gl_FragColor = col;
}
