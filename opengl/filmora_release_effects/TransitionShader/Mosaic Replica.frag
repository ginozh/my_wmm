/*************************************************
author: RuanShengQiang
date: 2017/4/20
**************************************************/
#ifdef GL_ES
precision mediump float;
#endif

float iGlobalTime = PREFIX(global_time);
vec2 mirror(vec2 uv)
{
	vec2 parity = mod(floor(uv),2.0);
	vec2 sign = -2.0*parity+1.0;
	return mod( 2.0*parity+sign*mod(uv,1.0),1.0);
}

vec4 FUNCNAME(vec2 tc) {

    vec2 uv=gl_FragCoord.xy/iResolution.xy;
	float prop = iResolution.y / iResolution.x;
	float progress =  sin(iGlobalTime*1.570796);

	vec2 input1UV = (uv-vec2(0.5))/(1.0- 0.988*progress)+vec2(0.5) ;
	vec2 input2UV = (uv-vec2(0.5))/(1.0- 0.5*(1.0-progress))+vec2(0.5);
	
	float mosicSplit = 400.0*(iGlobalTime-0.5)*(iGlobalTime-0.5)+3.0;
	vec2 mosicXY1 = mirror( floor((input1UV+0.5/mosicSplit)*mosicSplit)/mosicSplit);
	vec2 mosicXY2 = mirror( floor((input2UV+0.5/mosicSplit)*mosicSplit)/mosicSplit);

	
	return INPUT1(mosicXY1)*step(iGlobalTime,0.5)+INPUT2(mosicXY2)*step(0.5,iGlobalTime);
	//return INPUT1(mosicXY);
}