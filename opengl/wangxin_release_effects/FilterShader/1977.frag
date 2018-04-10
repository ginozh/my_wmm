/*{
	"GUID":"BB101941-A006-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"1977map.png"
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
     
     vec4 texel = vec4(
                  INPUT2(vec2(orig.r, .16666)).r,
                  INPUT2(vec2(orig.g, .5)).g,
                  INPUT2(vec2(orig.b, .83333)).b,
				  orig.a);
     
     return mix(texel, orig, 1.0 - PREFIX(fade) * PREFIX(strength));
}