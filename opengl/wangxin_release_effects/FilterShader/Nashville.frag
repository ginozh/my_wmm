/*{
	"GUID":"BB101941-A096-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"nashvilleMap.png"
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
     
     return mix(vec4(texel, 1.0), orig, 1.0 - PREFIX(fade) * PREFIX(strength));
}