/*{
	"GUID":"BB101941-A136-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"toasterMetal.png"
		},
		{
			"TYPE":"image",
			"FILE":"toasterSoftLight.png"
		},
		{
			"TYPE":"image",
			"FILE":"toasterCurves.png"
		},
		{
			"TYPE":"image",
			"FILE":"toasterOverlayMapWarm.png"
		},
		{
			"TYPE":"image",
			"FILE":"toasterColorShift.png"
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
	 vec4 texel = orig;
	 /*
     vec3 bbTexel = texture2D(mSample1, tc).rgb;
     
     texel.r = texture2D(mSample2, vec2(bbTexel.r, texel.r)).r;
     texel.g = texture2D(mSample2, vec2(bbTexel.g, texel.g)).g;
     texel.b = texture2D(mSample2, vec2(bbTexel.b, texel.b)).b;
	*/
     texel.r = texture2D(mSample3, vec2(texel.r, .16666)).r;
     texel.g = texture2D(mSample3, vec2(texel.g, .5)).g;
     texel.b = texture2D(mSample3, vec2(texel.b, .83333)).b;
     
	 vec2 tt = (2.0 * tc) - 1.0;
     float d = dot(tt, tt);
     vec2 lookup = vec2(d, texel.r);
     texel.r = texture2D(mSample4, lookup).r;
     lookup.y = texel.g;
     texel.g = texture2D(mSample4, lookup).g;
     lookup.y = texel.b;
     texel.b	= texture2D(mSample4, lookup).b;
	 
	 texel.r = texture2D(mSample5, vec2(texel.r, .16666)).r;
     texel.g = texture2D(mSample5, vec2(texel.g, .5)).g;
     texel.b = texture2D(mSample5, vec2(texel.b, .83333)).b;
     
     gl_FragColor = mix(texel, orig, 1.0 - u_strength);
}