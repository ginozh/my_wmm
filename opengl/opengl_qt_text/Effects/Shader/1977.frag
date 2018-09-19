/*{
	"GUID":"BB101941-A006-4332-B70F-22DD4B4236BD",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"1977map.png"
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

void main() 
{
	vec4 orig = texture2D(mSample0, textureCoord);
     
     gl_FragColor = vec4(
                  texture2D(mSample1, vec2(orig.r, .16666)).r,
                  texture2D(mSample1, vec2(orig.g, .5)).g,
                  texture2D(mSample1, vec2(orig.b, .83333)).b,
				  orig.a);
}