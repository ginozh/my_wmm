/*{
	"GUID":"BB101941-A146-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"valenciaMap.png"
		},
		{
			"TYPE":"image",
			"FILE":"valenciaGradientMap.png"
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

mat3 saturateMatrix = mat3(
                            1.1402,
                            -0.0598,
                            -0.061,
                            -0.1174,
                            1.0826,
                            -0.1186,
                            -0.0228,
                            -0.0228,
                            1.1772);
 
 vec3 lumaCoeffs = vec3(.3, .59, .11);
 
void main()
{
	vec2 tc = textureCoord;
	 vec4 orig = texture2D(mSample0, tc);
	 vec3 texel = orig.rgb;
     
     texel = vec3(
                  texture2D(mSample1, vec2(texel.r, .16666)).r,
                  texture2D(mSample1, vec2(texel.g, .5)).g,
                  texture2D(mSample1, vec2(texel.b, .83333)).b);
     
     texel = saturateMatrix * texel;
     float luma = dot(lumaCoeffs, texel);
     texel = vec3(
                  texture2D(mSample2, vec2(luma, texel.r)).r,
                  texture2D(mSample2, vec2(luma, texel.g)).g,
                  texture2D(mSample2, vec2(luma, texel.b)).b);
     
     gl_FragColor = mix(vec4(texel, 1.0), orig, 1.0 - u_strength);
}