uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

const float flash_len = 0.3;
void main()
{
	vec2 tc = textureCoord;
	vec4 color = texture2D(mSample0, tc);
	float time = mod(u_global_time, flash_len * 2.);
	if (time <= flash_len)
	{
		float t = time / flash_len;
		if (t >= 0.4 && t <= 0.7)
			color.rgb += vec3(0.5, 0.44, 0.84);
		else
			color.rgb += vec3(0.74, 0.8, 0.84);
	}
    gl_FragColor = color;
}
