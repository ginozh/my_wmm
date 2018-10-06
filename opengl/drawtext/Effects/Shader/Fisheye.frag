/*{
	"GUID":"A0DFCB73-9C3D-4d18-813E-8CC29D76D406",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"FishEyeNew.png"
		}
	],
	"PARAMS":[
		{
			"NAME":"radius",
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
uniform float u_radius;

vec2 u_imgWH = u_resolution;

vec4 Screen(vec4 base, vec4 blend)
{
    vec4 white = vec4(1.0);
    vec4 result = white - ((white - blend) * (white - base));
    return result;
}

void main()
{
	vec2 tc = textureCoord;
    vec2 u_center = vec2(0.5, 0.5);              //normalized coord
    float radius = 0.25 + 0.25 * u_radius;

    float u_aspectRatio = u_imgWH.g / u_imgWH.r; //360.0 / 480.0; For height
//    float u_aspectRatio = u_imgWH.r / u_imgWH.g  //480.0 / 360.0; For width
    
    float u_refractiveIndex = 0.75;
    
    vec2 textureCoordinateToUse = vec2(tc.x, (tc.y * u_aspectRatio + 0.5 - 0.5 * u_aspectRatio));
//    vec2 textureCoordinateToUse = vec2((tc.x * u_aspectRatio + 0.5 - 0.5 * u_aspectRatio), tc.y);
    
    float distanceFromCenter = distance(u_center, textureCoordinateToUse);
    float checkForPresenceWithinSphere = step(distanceFromCenter, radius);
    
    distanceFromCenter = distanceFromCenter / radius;
    
    float normalizedDepth = radius * sqrt(1.0 - distanceFromCenter * distanceFromCenter);
    vec3 sphereNormal = normalize(vec3(textureCoordinateToUse - u_center, normalizedDepth));
    
    vec3 refractedVector = 2.0*refract(vec3(0.0, 0.0, -1.0), sphereNormal, u_refractiveIndex);
    refractedVector = -refractedVector;
    
    vec4 baseLay = texture2D(mSample0, (refractedVector.xy + 1.0) * 0.5) * checkForPresenceWithinSphere;
    
    vec2 upTexCoord = vec2(tc.x, tc.y * u_aspectRatio + 0.5 - 0.5 * u_aspectRatio);
    float texScale = 0.5 / radius;
    upTexCoord = (upTexCoord - u_center) * texScale + u_center;
    vec4 upLay = texture2D(mSample1, upTexCoord);
    
    vec4 color  = baseLay * (1.0 - upLay.a) + upLay * upLay.a;
	color.a = texture2D(mSample0, tc).a;
	gl_FragColor = color;
}
