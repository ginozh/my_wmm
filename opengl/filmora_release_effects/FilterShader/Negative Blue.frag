/*{
	"GUID":"51237319-8F37-4593-BBAF-9FF0A6307C03",
	"PARAMS":[
		{
			"NAME":"strength",
			"TYPE":"float",
			"DEFAULT":0.8,
			"MIN":0.0,
			"MAX":1.0
		}
	] 
}*/

vec4 FUNCNAME(vec2 tc)
{
	// Grayscale conversion using NTSC conversion weights
	vec4 color = INPUT(tc);
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));

    float r,g,b, c1, c2;
	c1 = 0.5 + 0.5 * PREFIX(strength);
	c2 = 0.5 - 0.5 * PREFIX(strength);
    if (gray <= 0.5)
    {
		g = c2 * gray * 2.0;
		r = g;
		b = c1 * gray * 2.0;
    }
    else
    {
        g = c2 + (1.0 - c1) * (gray - 0.5) / 0.5;
		r = g;
		b = c1 + (1.0 - c1) * (gray - 0.5) / 0.5;
    }
    
    return vec4(b,g,r,color.a);
}
