/*{
	"GUID":"BB101941-A036-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"earlyBirdCurves.png"
		},
		{
			"TYPE":"image",
			"FILE":"earlyBirdOverlayMap.png"
		},
		{
			"TYPE":"image",
			"FILE":"vignetteMap.png"
		},
		{
			"TYPE":"image",
			"FILE":"earlyBirdBlowout.png"
		},
		{
			"TYPE":"image",
			"FILE":"earlyBirdMap.png"
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
 
 const mat3 saturate = mat3(
                            1.210300,
                            -0.089700,
                            -0.091000,
                            -0.176100,
                            1.123900,
                            -0.177400,
                            -0.034200,
                            -0.034200,
                            1.265800);
 const vec3 rgbPrime = vec3(0.25098, 0.14640522, 0.0); 
 const vec3 desaturate = vec3(.3, .59, .11);
 
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
     
     float desaturatedColor;
     vec3 result;
     desaturatedColor = dot(desaturate, texel);

     lookup.x = desaturatedColor;
     result.r = texture2D(mSample2, lookup).r;
     lookup.x = desaturatedColor;
     result.g = texture2D(mSample2, lookup).g;
     lookup.x = desaturatedColor;
     result.b = texture2D(mSample2, lookup).b;
     
     texel = saturate * mix(texel, result, .5);
     
     vec2 tt = (2.0 * tc) - 1.0;
     float d = dot(tt, tt);
     
     vec3 sampled;
     lookup.y = .5;
     
     /*
      lookup.x = texel.r;
      sampled.r = texture2D(mSample3, lookup).r;
      
      lookup.x = texel.g;
      sampled.g = texture2D(mSample3, lookup).g;
      
      lookup.x = texel.b;
      sampled.b = texture2D(mSample3, lookup).b;
      
      float value = smoothstep(0.0, 1.25, pow(d, 1.35)/1.65);
      texel = mix(texel, sampled, value);
      */
     
     //---
     
     lookup = vec2(d, texel.r);
     texel.r = texture2D(mSample3, lookup).r;
     lookup.y = texel.g;
     texel.g = texture2D(mSample3, lookup).g;
     lookup.y = texel.b;
     texel.b	= texture2D(mSample3, lookup).b;
     float value = smoothstep(0.0, 1.25, pow(d, 1.35)/1.65);
     
     //---
     
     lookup.x = texel.r;
     sampled.r = texture2D(mSample4, lookup).r;
     lookup.x = texel.g;
     sampled.g = texture2D(mSample4, lookup).g;
     lookup.x = texel.b;
     sampled.b = texture2D(mSample4, lookup).b;
     texel = mix(sampled, texel, value);
     
     
     lookup.x = texel.r;
     texel.r = texture2D(mSample5, lookup).r;
     lookup.x = texel.g;
     texel.g = texture2D(mSample5, lookup).g;
     lookup.x = texel.b;
     texel.b = texture2D(mSample5, lookup).b;
     
     gl_FragColor = mix(vec4(texel, 1.0), orig, 1.0 - u_strength);
}