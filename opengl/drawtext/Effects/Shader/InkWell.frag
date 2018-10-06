/*{
	"GUID":"BB101941-A066-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"inkwellMap.png"
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
     texel = vec3(dot(vec3(0.3, 0.6, 0.1), texel));
     texel = vec3(texture2D(mSample1, vec2(texel.r, .16666)).r);
	 gl_FragColor = mix(vec4(texel, orig.a), orig, 1.0 - u_strength);
}