vec3 m_R_vec3 = PREFIX(m_R_vec3);
vec3 m_G_vec3 = PREFIX(m_G_vec3);
vec3 m_B_vec3 = PREFIX(m_B_vec3);
vec3 m_A_vec3 = PREFIX(m_A_vec3);

vec4 FUNCNAME(vec2 tc)
{
	vec4 fragColor = INPUT(tc);
	
	float r = fragColor.r;
	float g = fragColor.g;
	float b = fragColor.b;
	float a = fragColor.a;
	
	float rr = r * m_R_vec3[0] + g * m_G_vec3[0] + b * m_B_vec3[0] + m_A_vec3[0];
	float gg = r * m_R_vec3[1] + g * m_G_vec3[1] + b * m_B_vec3[1] + m_A_vec3[1];
	float bb = r * m_R_vec3[2] + g * m_G_vec3[2] + b * m_B_vec3[2] + m_A_vec3[2];
	
	return vec4(rr, gg, bb, a);	
}