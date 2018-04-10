/*{
	"GUID":"BB101941-A136-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"toasterMetal.png"
		},
		{
			"TYPE":"image",
			"FILE":"toasterSoftLight.png"
		},
		{
			"TYPE":"image",
			"FILE":"toasterCurves.png"
		},
		{
			"TYPE":"image",
			"FILE":"toasterOverlayMapWarm.png"
		},
		{
			"TYPE":"image",
			"FILE":"toasterColorShift.png"
		}
	],
	"PARAMS":[
		{
			"NAME":"strength",
			"TYPE":"float",
			"DEFAULT":1.0,
			"MIN":0.0,
			"MAX":1.0
		}
	]
}*/
 
vec4 FUNCNAME(vec2 tc) 
{
	vec4 orig = INPUT1(tc);
	 vec4 texel = orig;
	 /*
     vec3 bbTexel = INPUT2(tc).rgb;
     
     texel.r = INPUT3(vec2(bbTexel.r, texel.r)).r;
     texel.g = INPUT3(vec2(bbTexel.g, texel.g)).g;
     texel.b = INPUT3(vec2(bbTexel.b, texel.b)).b;
	*/
     texel.r = INPUT4(vec2(texel.r, .16666)).r;
     texel.g = INPUT4(vec2(texel.g, .5)).g;
     texel.b = INPUT4(vec2(texel.b, .83333)).b;
     
	 vec2 tt = (2.0 * tc) - 1.0;
     float d = dot(tt, tt);
     vec2 lookup = vec2(d, texel.r);
     texel.r = INPUT5(lookup).r;
     lookup.y = texel.g;
     texel.g = INPUT5(lookup).g;
     lookup.y = texel.b;
     texel.b	= INPUT5(lookup).b;
	 
	 texel.r = INPUT6(vec2(texel.r, .16666)).r;
     texel.g = INPUT6(vec2(texel.g, .5)).g;
     texel.b = INPUT6(vec2(texel.b, .83333)).b;
     
     return mix(texel, orig, 1.0 - PREFIX(fade) * PREFIX(strength));
}