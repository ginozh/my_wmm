vec4 FUNCNAME(vec2 tc)
{
	vec4 fragColor = INPUT(tc);
	
	if(gl_FragCoord.x < 1.0 || gl_FragCoord.x > (u_resolution.x - 1.0) || \
	   gl_FragCoord.y < 1.0 || gl_FragCoord.y > (u_resolution.y - 1.0))
		return fragColor;
	
	vec2 TopUV0 = vec2(gl_FragCoord.x - 1.0, gl_FragCoord.y + 1.0).xy/u_resolution.xy;
	vec2 TopUV1 = vec2(gl_FragCoord.x, gl_FragCoord.y + 1.0).xy/u_resolution.xy;
	vec2 TopUV2 = vec2(gl_FragCoord.x + 1.0, gl_FragCoord.y + 1.0).xy/u_resolution.xy;
	vec2 MidUV0 = vec2(gl_FragCoord.x - 1.0, gl_FragCoord.y).xy/u_resolution.xy;
	vec2 MidUV1 = tc;
	vec2 MidUV2 = vec2(gl_FragCoord.x + 1.0, gl_FragCoord.y).xy/u_resolution.xy;
	vec2 BomUV0 = vec2(gl_FragCoord.x - 1.0, gl_FragCoord.y - 1.0).xy/u_resolution.xy;
	vec2 BomUV1 = vec2(gl_FragCoord.x, gl_FragCoord.y - 1.0).xy/u_resolution.xy;
	vec2 BomUV2 = vec2(gl_FragCoord.x + 1.0, gl_FragCoord.y - 1.0).xy/u_resolution.xy;
	
	vec4 Top0 = INPUT(TopUV0);
	vec4 Top1 = INPUT(TopUV1);
	vec4 Top2 = INPUT(TopUV2);
	vec4 Mid0 = INPUT(MidUV0);
	vec4 Mid1 = INPUT(MidUV1);
	vec4 Mid2 = INPUT(MidUV2);
	vec4 Bom0 = INPUT(BomUV0);
	vec4 Bom1 = INPUT(BomUV1);
	vec4 Bom2 = INPUT(BomUV2);
	
	fragColor = ((Top0 + 2.0 * Top1 + Top2) + \
				 (2.0 * Mid0 + 4.0 * Mid1 + 2.0 * Mid2) + \
				 (Bom0 + 2.0 * Bom1 + Bom2)) / 16.0;
	
	return fragColor;
}