float m_mergeAlpha = PREFIX(m_mergeAlpha);

vec4 FUNCNAME(vec2 tc)
{
	vec4 fragColor;
	vec4 orignalColor = INPUT1(tc);
	vec4 overlayColor = INPUT2(tc);
	float sa = overlayColor.a * m_mergeAlpha;
	float da = 1.0 - sa;
	
	fragColor = orignalColor * da + overlayColor * sa;
	fragColor.a = 1.0;
	
	return fragColor;
}