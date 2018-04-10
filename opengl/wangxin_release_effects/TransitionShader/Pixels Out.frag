/*************************************************
author: RuanShengQiang
date: 2017/4/20
**************************************************/
#ifdef GL_ES
precision mediump float;
#endif

float iGlobalTime = PREFIX(global_time);

vec4 FUNCNAME(vec2 tc) {

    vec2 uv=gl_FragCoord.xy/iResolution.xy;
	float prop = iResolution.y / iResolution.x;
	float progress = iGlobalTime*iGlobalTime;//sin(iGlobalTime*1.570796);
	
	vec2 input1UV = (uv-vec2(0.5))*(1.0- 0.5*progress)+vec2(0.5) ;
	vec2 input2UV = (uv-vec2(0.5))*(1.0- 0.5*(1.0-progress))+vec2(0.5);
	
	float mosicSplit = 200.0*(iGlobalTime-0.7)*(iGlobalTime-0.7)+5.0;
	vec2 mosicXY1 = floor((input1UV+0.5/mosicSplit)*mosicSplit)/mosicSplit;
	vec2 mosicXY2 = floor((input2UV+0.5/mosicSplit)*mosicSplit)/mosicSplit;

	
	return mix(INPUT1(mosicXY1),INPUT2(input2UV),progress) ;
	//return INPUT1(mosicXY);
}