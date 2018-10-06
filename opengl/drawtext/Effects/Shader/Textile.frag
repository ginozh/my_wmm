/*{
	"GUID":"84966D50-730C-4f9b-B10F-B4C94112B77E",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"sketch_black.png"
		}
	]
}*/

uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_strength;

vec4 Screen(vec4 base, vec4 blend)
{
    vec4 white = vec4(1.0);
    vec4 result = white - ((white - blend) * (white - base));
    return result;
}

vec4 ColorDodge(vec4 base, vec4 blend)
{
    vec4 white = vec4(1.0);
    vec4 result = base / clamp((white - blend), 0.000001, 1.0);
    return result;
}

void main()
{
	vec2 tc = textureCoord;
    vec4 base = texture2D(mSample0, tc);
    base = ColorDodge(base, vec4(vec3(0.2863), 1.0));
    
    vec4 blend = texture2D(mSample1, tc);
    gl_FragColor = Screen(base, blend*0.5);
}
