/*{
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"ColorTuneCircle00.png"
		}
	]
}*/

uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

void main()
{
	vec2 tc = textureCoord;
    vec4 tuneColor = texture2D(mSample0, tc);
    vec4 blendColor = texture2D(mSample1, tc);
	tuneColor.rgb *= blendColor.rgb;
	gl_FragColor = tuneColor;
}
