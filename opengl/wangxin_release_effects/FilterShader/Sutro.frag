/*{
	"GUID":"BB101941-A126-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"vignetteMap.png"
		},
		{
			"TYPE":"image",
			"FILE":"sutroMetal.png"
		},
		{
			"TYPE":"image",
			"FILE":"softLight.png"
		},
		{
			"TYPE":"image",
			"FILE":"sutroEdgeBurn.png"
		},
		{
			"TYPE":"image",
			"FILE":"sutroCurves.png"
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
     
     vec2 tt = (2.0 * tc) - 1.0;
     float d = dot(tt, tt);
     vec2 lookup = vec2(d, texel.r);
     texel.r = INPUT2(lookup).r;
     lookup.y = texel.g;
     texel.g = INPUT2(lookup).g;
     lookup.y = texel.b;
     texel.b	= INPUT2(lookup).b;
     
     vec3 rgbPrime = vec3(0.1019, 0.0, 0.0); 
     float m = dot(vec3(.3, .59, .11), texel.rgb) - 0.03058;
     texel = mix(texel, rgbPrime + m, 0.32);
     
     vec3 metal = INPUT3(tc).rgb;
     texel.r = INPUT4(vec2(metal.r, texel.r)).r;
     texel.g = INPUT4(vec2(metal.g, texel.g)).g;
     texel.b = INPUT4(vec2(metal.b, texel.b)).b;
     
     texel = texel * INPUT5(tc).rgb;
     
     texel.r = INPUT6(vec2(texel.r, .16666)).r;
     texel.g = INPUT6(vec2(texel.g, .5)).g;
     texel.b = INPUT6(vec2(texel.b, .83333)).b;
     
     
     return mix(vec4(texel, 1.0), orig, 1.0 - PREFIX(fade) * PREFIX(strength));
}