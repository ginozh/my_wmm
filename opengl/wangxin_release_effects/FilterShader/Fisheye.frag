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

vec2 u_imgWH = iResolution;

vec4 Screen(vec4 base, vec4 blend)
{
    vec4 white = vec4(1.0);
    vec4 result = white - ((white - blend) * (white - base));
    return result;
}

vec4 FUNCNAME(vec2 tc) 
{
    vec2 u_center = vec2(0.5, 0.5);              //normalized coord
    float u_radius = 0.25 + 0.25 * PREFIX(radius);

    float u_aspectRatio = u_imgWH.g / u_imgWH.r; //360.0 / 480.0; For height
//    float u_aspectRatio = u_imgWH.r / u_imgWH.g  //480.0 / 360.0; For width
    
    float u_refractiveIndex = 0.75;
    
    vec2 textureCoordinateToUse = vec2(tc.x, (tc.y * u_aspectRatio + 0.5 - 0.5 * u_aspectRatio));
//    vec2 textureCoordinateToUse = vec2((tc.x * u_aspectRatio + 0.5 - 0.5 * u_aspectRatio), tc.y);
    
    float distanceFromCenter = distance(u_center, textureCoordinateToUse);
    float checkForPresenceWithinSphere = step(distanceFromCenter, u_radius);
    
    distanceFromCenter = distanceFromCenter / u_radius;
    
    float normalizedDepth = u_radius * sqrt(1.0 - distanceFromCenter * distanceFromCenter);
    vec3 sphereNormal = normalize(vec3(textureCoordinateToUse - u_center, normalizedDepth));
    
    vec3 refractedVector = 2.0*refract(vec3(0.0, 0.0, -1.0), sphereNormal, u_refractiveIndex);
    refractedVector = -refractedVector;
    
    vec4 baseLay = INPUT1((refractedVector.xy + 1.0) * 0.5) * checkForPresenceWithinSphere;
    
    vec2 upTexCoord = vec2(tc.x, tc.y * u_aspectRatio + 0.5 - 0.5 * u_aspectRatio);
    float texScale = 0.5 / u_radius;
    upTexCoord = (upTexCoord - u_center) * texScale + u_center;
    vec4 upLay = INPUT2(upTexCoord);
    
    vec4 color  = baseLay * (1.0 - upLay.a) + upLay * upLay.a;
	color.a = 1.0;
    return color;
}
