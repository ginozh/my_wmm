/*{
	"GUID":"BB101941-A046-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"edgeBurn.png"
		},
		{
			"TYPE":"image",
			"FILE":"hefeMap.png"
		},
		{
			"TYPE":"image",
			"FILE":"hefeGradientMap.png"
		},
		{
			"TYPE":"image",
			"FILE":"hefeSoftLight.png"
		},
		{
			"TYPE":"image",
			"FILE":"hefeMetal.png"
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
     vec3 edge = texture2D(mSample1, tc).rgb;
	texel = texel * edge;
	
	texel = vec3(
                 texture2D(mSample2, vec2(texel.r, .16666)).r,
                 texture2D(mSample2, vec2(texel.g, .5)).g,
                 texture2D(mSample2, vec2(texel.b, .83333)).b);
	
	vec3 luma = vec3(.30, .59, .11);
	vec3 gradSample = texture2D(mSample3, vec2(dot(luma, texel), .5)).rgb;
	vec3 final = vec3(
                      texture2D(mSample4, vec2(gradSample.r, texel.r)).r,
                      texture2D(mSample4, vec2(gradSample.g, texel.g)).g,
                      texture2D(mSample4, vec2(gradSample.b, texel.b)).b
                      );
    
    vec3 metal = texture2D(mSample5, tc).rgb;
    vec3 metaled = vec3(
                        texture2D(mSample4, vec2(metal.r, texel.r)).r,
                        texture2D(mSample4, vec2(metal.g, texel.g)).g,
                        texture2D(mSample4, vec2(metal.b, texel.b)).b
                        );
	
	gl_FragColor = mix(vec4(metaled, 1.0), orig, 1.0 - u_strength);
}