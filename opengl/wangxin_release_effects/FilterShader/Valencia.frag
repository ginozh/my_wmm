/*{
	"GUID":"BB101941-A146-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"valenciaMap.png"
		},
		{
			"TYPE":"image",
			"FILE":"valenciaGradientMap.png"
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

mat3 saturateMatrix = mat3(
                            1.1402,
                            -0.0598,
                            -0.061,
                            -0.1174,
                            1.0826,
                            -0.1186,
                            -0.0228,
                            -0.0228,
                            1.1772);
 
 vec3 lumaCoeffs = vec3(.3, .59, .11);
 
vec4 FUNCNAME(vec2 tc) 
{
	 vec4 orig = INPUT1(tc);
	 vec3 texel = orig.rgb;
     
     texel = vec3(
                  INPUT2(vec2(texel.r, .16666)).r,
                  INPUT2(vec2(texel.g, .5)).g,
                  INPUT2(vec2(texel.b, .83333)).b);
     
     texel = saturateMatrix * texel;
     float luma = dot(lumaCoeffs, texel);
     texel = vec3(
                  INPUT3(vec2(luma, texel.r)).r,
                  INPUT3(vec2(luma, texel.g)).g,
                  INPUT3(vec2(luma, texel.b)).b);
     
     return mix(vec4(texel, 1.0), orig, 1.0 - PREFIX(fade) * PREFIX(strength));
}