uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

const int nSamples = 123;
float iGlobalTime = u_global_time;

vec4 getTex2D(vec2 uv, float jerk)
{
	vec2 tc = mod(uv + vec2(0.0, jerk), vec2(1.0, 1.0));
	return texture2D(mSample0, tc);
}

void main()
{
	vec2 tc = textureCoord;
	float time = mod(iGlobalTime, 2.3);
	float yOffset = 0.0;
	float length = 0.0;
	if (time < 0.25)
	{
	}
	else if (time < 0.29)
	{
		yOffset = 0.5;
		length = 0.07;
	}
	else if (time < 0.336)
	{
		yOffset = 0.15;
		length = 0.04;
	}
	else if (time < 0.375)
	{
		yOffset = 0.0;
		length = 0.1;
	}
	else if (time < 0.417)
	{
		yOffset = 0.1;
		length = 0.02;
	}
	else if (time < 0.458)
	{
		yOffset = 0.8;
		length = 0.03;
	}
	else if (time < 0.5)
	{
		yOffset = 0.9;
		length = 0.03;
	}
	else if (time < 0.542)
	{
		yOffset = 0.15;
		length = 0.04;
	}
	else if (time < 0.584)
	{
		yOffset = 0.0;
		length = 0.09;
	}
	else if (time < 0.86)
	{
	}
	else if (time < 0.917)
	{
		yOffset = 0.05;
		length = 0.01;
	}
	else if (time < 0.959)
	{
		yOffset = 0.4;
		length = 0.01;
	}
	else if (time < 1.01)
	{
		yOffset = 0.0;
		length = 0.05;
	}
	else if (time < 1.04)
	{
	}
	else if (time < 1.083)
	{
		yOffset = 0.8;
		length = 0.03;
	}
	else if (time < 1.125)
	{
		yOffset = 0.0;
		length = 0.07;
	}
	else if (time < 1.16)
	{
	}
	else if (time < 1.20)
	{
		yOffset = 0.97;
		length = 0.05;
	}
	else if (time < 1.367)
	{
	}
	else if (time < 1.417)
	{
		yOffset = 0.92;
		length = 0.015;
	}
	else if (time < 1.5)
	{
	}
	else if (time < 1.542)
	{
		yOffset = 0.04;
		length = 0.005;
	}
	else if (time < 1.91)
	{
	}
	else if (time < 1.96)
	{
		yOffset = 0.09;
		length = 0.024;
	}
	else if (time < 2.02)
	{
		yOffset = 0.0;
		length = 0.024;
	}
	else if (time < 2.076)
	{
	}
	else if (time < 2.126)
	{
		yOffset = 0.92;
		length = 0.014;
	}
	length *= 4.0;
	
	vec4 color = texture2D(mSample0, tc);
	vec4 result = getTex2D(tc,yOffset);
	vec2 blurVec = vec2(0.0, length);
	for(int i = 1; i < nSamples; i++) {
		vec2 offset = blurVec * (float(i) / float(nSamples - 1) - 0.5);
		result += getTex2D(tc + offset,yOffset);
	}
	result /= float(nSamples);
	
	float gray1 = max(max(result.r,result.g),result.b);
	float gray2 = dot(color.rgb, vec3(0.299, 0.587, 0.114));
	float gray = max(gray1, gray2);
	
	gl_FragColor = color + result * gray1;
}