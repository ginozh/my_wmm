/*{
	"GUID":"51237329-8F37-4593-BBAF-9FF0A6307C06"
}*/
const float flash_len = 0.3;
vec4 FUNCNAME(vec2 tc)
{
	vec4 color = INPUT(tc);
	float time = mod(PREFIX(global_time), flash_len * 2.);
	if (time <= flash_len)
	{
		float t = time / flash_len;
		if (t >= 0.4 && t <= 0.7)
			color.rgb += vec3(0.5, 0.44, 0.84);
		else
			color.rgb += vec3(0.74, 0.8, 0.84);
	}
    return color;
}
