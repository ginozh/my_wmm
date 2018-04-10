/*{
	"GUID":"84966D50-730C-4f9b-B10F-B4C94112B77E",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"sketch_black.png"
		}
	]
}*/

vec4 Screen(vec4 base, vec4 blend)
{
    vec4 white = vec4(1.0);
    vec4 result = white - ((white - blend) * (white - base));
    result.a = 1.0;
    return result;
}

vec4 ColorDodge(vec4 base, vec4 blend)
{
    vec4 white = vec4(1.0);
    vec4 result = base / (white - blend);
    return result;
}

vec4 FUNCNAME(vec2 tc) 
{
    vec4 base = INPUT1(tc);
    base = ColorDodge(base, vec4(vec3(0.2863), 1.0));
    
    vec4 blend = INPUT2(tc);
    return Screen(base, blend);
}
