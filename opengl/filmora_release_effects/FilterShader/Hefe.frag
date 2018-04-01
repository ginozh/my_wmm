/*{
	"GUID":"BB101941-A046-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"edgeBurn.png"
		},
		{
			"TYPE":"image",
			"FILE":"hefeMap.png"
		},
		{
			"TYPE":"image",
			"FILE":"hefeGradientMap.png"
		},
		{
			"TYPE":"image",
			"FILE":"hefeSoftLight.png"
		},
		{
			"TYPE":"image",
			"FILE":"hefeMetal.png"
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
     vec3 edge = INPUT2(tc).rgb;
	texel = texel * edge;
	
	texel = vec3(
                 INPUT3(vec2(texel.r, .16666)).r,
                 INPUT3(vec2(texel.g, .5)).g,
                 INPUT3(vec2(texel.b, .83333)).b);
	
	vec3 luma = vec3(.30, .59, .11);
	vec3 gradSample = INPUT4(vec2(dot(luma, texel), .5)).rgb;
	vec3 final = vec3(
                      INPUT5(vec2(gradSample.r, texel.r)).r,
                      INPUT5(vec2(gradSample.g, texel.g)).g,
                      INPUT5(vec2(gradSample.b, texel.b)).b
                      );
    
    vec3 metal = INPUT6(tc).rgb;
    vec3 metaled = vec3(
                        INPUT5(vec2(metal.r, texel.r)).r,
                        INPUT5(vec2(metal.g, texel.g)).g,
                        INPUT5(vec2(metal.b, texel.b)).b
                        );
	
	return mix(vec4(metaled, 1.0), orig, 1.0 - PREFIX(fade) * PREFIX(strength));
}