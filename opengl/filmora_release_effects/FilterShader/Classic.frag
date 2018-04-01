/*{
	"GUID":"49D37319-8F37-4593-BBAF-9FF0A6307C01",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"sketch.png"
		}
	]
}*/

uniform vec2 PREFIX(offset)[9];

vec4 Screen(vec4 base, vec4 blend)
{
    vec4 white = vec4(1.0);
    vec4 result = white - ((white - blend) * (white - base));
    return result;
}

vec4 softlight(vec4 base, vec4 blend)
{
    vec4 result = 2.0 * base * blend + base * base - 2.0 * base * base * blend;

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
    vec4 tempColor;
    vec4 result = INPUT1(tc);
    vec4 blend = INPUT2(tc);
    float maxGray = 0.0;
    float sample[9];
    for(int i=0; i < 9; i++)
    {
        tempColor = INPUT1(tc + PREFIX(offset)[i]);
        sample[i] = dot(tempColor.rgb, vec3(0.114, 0.587, 0.299));
        maxGray = max(sample[i], maxGray);
    }
    float minGray = 1.0 - maxGray;
    vec4 min = vec4(vec3(minGray), 1.0);
    result = softlight(vec4(vec3(sample[4]), 1.0), blend);
    result = ColorDodge(result, min);
	result.a = 1.0;
	return result;
}
