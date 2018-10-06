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
 
void main()
{
	vec2 tc = textureCoord;
	vec4 orig = texture2D(mSample0, tc);
	 vec3 texel = orig.rgb;
     
     vec2 tt = (2.0 * tc) - 1.0;
     float d = dot(tt, tt);
     vec2 lookup = vec2(d, texel.r);
     texel.r = texture2D(mSample1, lookup).r;
     lookup.y = texel.g;
     texel.g = texture2D(mSample1, lookup).g;
     lookup.y = texel.b;
     texel.b	= texture2D(mSample1, lookup).b;
     
     vec3 rgbPrime = vec3(0.1019, 0.0, 0.0); 
     float m = dot(vec3(.3, .59, .11), texel.rgb) - 0.03058;
     texel = mix(texel, rgbPrime + m, 0.32);
     
     vec3 metal = texture2D(mSample2, tc).rgb;
     texel.r = texture2D(mSample3, vec2(metal.r, texel.r)).r;
     texel.g = texture2D(mSample3, vec2(metal.g, texel.g)).g;
     texel.b = texture2D(mSample3, vec2(metal.b, texel.b)).b;
     
     texel = texel * texture2D(mSample4, tc).rgb;
     
     texel.r = texture2D(mSample5, vec2(texel.r, .16666)).r;
     texel.g = texture2D(mSample5, vec2(texel.g, .5)).g;
     texel.b = texture2D(mSample5, vec2(texel.b, .83333)).b;
     
     
     gl_FragColor = mix(vec4(texel, 1.0), orig, 1.0 - u_strength);
}