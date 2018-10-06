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

uniform sampler2D mSample0;
uniform sampler2D mSample1;
uniform sampler2D mSample2;
uniform sampler2D mSample3;
uniform sampler2D mSample4;
uniform sampler2D mSample5;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

uniform float u_strength;
 
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
 
void main()
{
	vec2 tc = textureCoord;
	vec4 orig = texture2D(mSample0, tc);
	vec3 texel = orig.rgb;
     
     vec2 lookup;
     lookup.y = 0.5;
     lookup.x = texel.r;
     texel.r = texture2D(mSample1, lookup).r;
     lookup.x = texel.g;
     texel.g = texture2D(mSample1, lookup).g;
     lookup.x = texel.b;
     texel.b = texture2D(mSample1, lookup).b;
     
     texel = saturateMatrix * texel;
     
     
     vec2 tt = (2.0 * tc) - 1.0;
     float d = dot(tt, tt);
     vec3 sampled;
     lookup.y = 0.5;
     lookup.x = texel.r;
     sampled.r = texture2D(mSample2, lookup).r;
     lookup.x = texel.g;
     sampled.g = texture2D(mSample2, lookup).g;
     lookup.x = texel.b;
     sampled.b = texture2D(mSample2, lookup).b;
     float value = smoothstep(0.0, 1.0, d);
     texel = mix(sampled, texel, value);
     
     lookup.x = texel.r;
     texel.r = texture2D(mSample3, lookup).r;
     lookup.x = texel.g;
     texel.g = texture2D(mSample3, lookup).g;
     lookup.x = texel.b;
     texel.b = texture2D(mSample3, lookup).b;
     
     
     lookup.x = dot(texel, luma);
     texel = mix(texture2D(mSample4, lookup).rgb, texel, .5);

     lookup.x = texel.r;
     texel.r = texture2D(mSample5, lookup).r;
     lookup.x = texel.g;
     texel.g = texture2D(mSample5, lookup).g;
     lookup.x = texel.b;
     texel.b = texture2D(mSample5, lookup).b;
	 
	 gl_FragColor = vec4(texel, orig.a);//mix(vec4(texel, 1.0), orig, 1.0 - u_strength);
}