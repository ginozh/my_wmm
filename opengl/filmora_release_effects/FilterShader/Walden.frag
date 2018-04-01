/*{
	"GUID":"BB101941-A156-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"waldenMap.png"
		},
		{
			"TYPE":"image",
			"FILE":"vignetteMap.png"
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
	 vec3 texel = orig.rgb;
     
     texel = vec3(
                  INPUT2(vec2(texel.r, .16666)).r,
                  INPUT2(vec2(texel.g, .5)).g,
                  INPUT2(vec2(texel.b, .83333)).b);
     
     vec2 tt = (2.0 * tc) - 1.0;
     float d = dot(tt, tt);
     vec2 lookup = vec2(d, texel.r);
     texel.r = INPUT3(lookup).r;
     lookup.y = texel.g;
     texel.g = INPUT3(lookup).g;
     lookup.y = texel.b;
     texel.b	= INPUT3(lookup).b;
     
     return mix(vec4(texel, 1.0), orig, 1.0 - PREFIX(fade) * PREFIX(strength));
}