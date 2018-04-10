/*{
	"GUID":"BB101941-A166-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"xproMap.png"
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
	 
	 vec2 red = vec2(texel.r, 0.16666);
     vec2 green = vec2(texel.g, 0.5);
     vec2 blue = vec2(texel.b, .83333);
     texel.r = INPUT2(red).r;
     texel.g = INPUT2(green).g;
     texel.b = INPUT2(blue).b;
     
     return mix(vec4(texel, 1.0), orig, 1.0 - PREFIX(fade) * PREFIX(strength));
}