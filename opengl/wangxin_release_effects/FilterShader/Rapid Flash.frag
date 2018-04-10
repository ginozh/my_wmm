/*{
	"GUID":"51237339-8F37-4593-BBAF-9FF0A6307C07"
}*/
const float exp = 0.2;
const float flash_len = 0.5;
vec4 FUNCNAME(vec2 tc)
{
	vec4 color = INPUT(tc);
	
	float time = mod(PREFIX(global_time), flash_len * 2.);
	if (time <= flash_len)
	{
		float t = time / flash_len;
		float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
		float scale = 1.0;
		
		if (t <= 0.5)
			t = t * 2.0;
		else
			t = (1.0 - t) * 2.0;
		if (t > exp)
		{
			scale = 0.9 + 0.1 * (t - exp) / (1.0 - exp);
		}
		else
		{
			scale = 0.9 * (t / exp);
		}
		color.rgb = mix(color.rgb, vec3(gray,gray,gray), scale);
		if (t > 0.0)
		{
			float strange = t * 20.;
			color.r *= strange;
			color.g *= strange;
			color.b *= strange;
		}
	}
	

    return color;
}