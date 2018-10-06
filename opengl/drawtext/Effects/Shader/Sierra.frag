/*{
	"GUID":"BB101941-A116-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"sierraVignette.png"
		},
		{
			"TYPE":"image",
			"FILE":"overlayMap.png"
		},
		{
			"TYPE":"image",
			"FILE":"sierraMap.png"
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

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_strength;

void main()
{
	vec2 tc = textureCoord;
	 vec4 orig = texture2D(mSample0, tc);
	 vec4 texel = orig;
     vec3 bbTexel = texture2D(mSample1, tc).rgb;
     
     texel.r = texture2D(mSample2, vec2(bbTexel.r, texel.r)).r;
     texel.g = texture2D(mSample2, vec2(bbTexel.g, texel.g)).g;
     texel.b = texture2D(mSample2, vec2(bbTexel.b, texel.b)).b;
     
     vec4 mapped;
     mapped.r = texture2D(mSample3, vec2(texel.r, .16666)).r;
     mapped.g = texture2D(mSample3, vec2(texel.g, .5)).g;
     mapped.b = texture2D(mSample3, vec2(texel.b, .83333)).b;
     mapped.a = 1.0;
     
     gl_FragColor = mix(mapped, orig, 1.0 - u_strength);
}