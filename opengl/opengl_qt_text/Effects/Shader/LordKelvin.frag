/*{
	"GUID":"BB101941-A086-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"kelvinMap.png"
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

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_strength;

void main()
{
	vec2 tc = textureCoord;
	 vec4 orig = texture2D(mSample0, tc);
	 vec3 texel = orig.rgb;
     
     vec2 lookup;
     lookup.y = .5;
     
     lookup.x = texel.r;
     texel.r = texture2D(mSample1, lookup).r;
     
     lookup.x = texel.g;
     texel.g = texture2D(mSample1, lookup).g;
     
     lookup.x = texel.b;
     texel.b = texture2D(mSample1, lookup).b;
     
     gl_FragColor = mix(vec4(texel, 1.0), orig, 1.0 - u_strength);
}