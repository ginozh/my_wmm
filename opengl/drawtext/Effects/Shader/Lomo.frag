/*{
	"GUID":"BB101941-A076-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"lomoMap.png"
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

uniform sampler2D mSample0;
uniform sampler2D mSample1;
uniform sampler2D mSample2;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_strength;

void main()
{
	vec2 tc = textureCoord;
	 vec4 orig = texture2D(mSample0, tc);
	 vec3 texel = orig.rgb;
     
     texel = vec3(
                  texture2D(mSample1, vec2(texel.r, .16666)).r,
                  texture2D(mSample1, vec2(texel.g, .5)).g,
                  texture2D(mSample1, vec2(texel.b, .83333)).b);
     
     vec2 tt = (2.0 * tc) - 1.0;
     float d = dot(tt, tt);
     vec2 lookup = vec2(d, texel.r);
     texel.r = texture2D(mSample2, lookup).r;
     lookup.y = texel.g;
     texel.g = texture2D(mSample2, lookup).g;
     lookup.y = texel.b;
     texel.b	= texture2D(mSample2, lookup).b;
     
     gl_FragColor = mix(vec4(texel, orig.a), orig, 1.0 - u_strength);
}