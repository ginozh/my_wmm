vec4 FUNCNAME(vec2 tc)
{
	vec4 textureColor = INPUT1(tc);

	float r = INPUT2(vec2(textureColor.r, 0.0)).r;
	float g = INPUT2(vec2(textureColor.g, 0.0)).g;
	float b = INPUT2(vec2(textureColor.b, 0.0)).b;
	
	textureColor = vec4(r, g, b, textureColor.a);
	
	 return textureColor;
}