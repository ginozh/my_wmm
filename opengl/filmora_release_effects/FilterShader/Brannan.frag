/*{
	"GUID":"BB101941-A026-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"brannanProcess.png"
		},
		{
			"TYPE":"image",
			"FILE":"brannanBlowout.png"
		},
		{
			"TYPE":"image",
			"FILE":"brannanContrast.png"
		},
		{
			"TYPE":"image",
			"FILE":"brannanLuma.png"
		},
		{
			"TYPE":"image",
			"FILE":"brannanScreen.png"
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
                            1.105150,
                            -0.044850,
                            -0.046000,
                            -0.088050,
                            1.061950,
                            -0.089200,
                            -0.017100,
                            -0.017100,
                            1.132900);
 
 vec3 luma = vec3(.3, .59, .11);
 
vec4 FUNCNAME(vec2 tc) 
{
	vec4 orig = INPUT1(tc);
	vec3 texel = orig.rgb;
     
     vec2 lookup;
     lookup.y = 0.5;
     lookup.x = texel.r;
     texel.r = INPUT2(lookup).r;
     lookup.x = texel.g;
     texel.g = INPUT2(lookup).g;
     lookup.x = texel.b;
     texel.b = INPUT2(lookup).b;
     
     texel = saturateMatrix * texel;
     
     
     vec2 tt = (2.0 * tc) - 1.0;
     float d = dot(tt, tt);
     vec3 sampled;
     lookup.y = 0.5;
     lookup.x = texel.r;
     sampled.r = INPUT3(lookup).r;
     lookup.x = texel.g;
     sampled.g = INPUT3(lookup).g;
     lookup.x = texel.b;
     sampled.b = INPUT3(lookup).b;
     float value = smoothstep(0.0, 1.0, d);
     texel = mix(sampled, texel, value);
     
     lookup.x = texel.r;
     texel.r = INPUT4(lookup).r;
     lookup.x = texel.g;
     texel.g = INPUT4(lookup).g;
     lookup.x = texel.b;
     texel.b = INPUT4(lookup).b;
     
     
     lookup.x = dot(texel, luma);
     texel = mix(INPUT5(lookup).rgb, texel, .5);

     lookup.x = texel.r;
     texel.r = INPUT6(lookup).r;
     lookup.x = texel.g;
     texel.g = INPUT6(lookup).g;
     lookup.x = texel.b;
     texel.b = INPUT6(lookup).b;
	 
	 return mix(vec4(texel, 1.0), orig, 1.0 - PREFIX(fade) * PREFIX(strength));
}