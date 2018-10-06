int  m_bSrcLeft = PREFIX(m_bSrcLeft);
int  m_bSplit = PREFIX(m_bSplit);
vec3 m_redCoeffs   = PREFIX(redCoeffs);
vec3 m_greenCoeffs = PREFIX(greenCoeffs);
vec3 m_blueCoeffs  = PREFIX(blueCoeffs);

float Parabola(float dX, vec3 dCoeffs)
{
	return (dCoeffs[0] * dX + dCoeffs[1]) * dX + dCoeffs[2];
}

vec4 FUNCNAME(vec2 tc)
{
	vec4 fragColor = INPUT(tc);
	bool bSplit = m_bSplit == 1 ? true : false;
	bool bSrcLeft = m_bSrcLeft == 1 ? true : false;
	bool copyPixel = bSplit && (bSrcLeft && gl_FragCoord.x < (u_resolution.x / 2.0) || !bSrcLeft && gl_FragCoord.x >= (u_resolution.x / 2.0));
	
	if(copyPixel)
	{
		float r = Parabola(fragColor.r, m_redCoeffs);
		float g = Parabola(fragColor.g, m_greenCoeffs);
		float b = Parabola(fragColor.b, m_blueCoeffs);
		
		fragColor.r = r;
		fragColor.g = g;
		fragColor.b = b;	
	}	
	
	return fragColor;
}











