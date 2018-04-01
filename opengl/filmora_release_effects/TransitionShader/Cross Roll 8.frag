/*************************************************
author: RuanShengQiang
date: 2017/3/24
**************************************************/
#ifdef GL_ES
precision mediump float;
#endif

float iGlobalTime = PREFIX(global_time);
const float PI = 3.141592653589793;

float normalizedRandom(vec2 co)
{
   return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
//to amp "amp" times the picture;
vec2 scale(vec2 uv, float amp)
{

	return mod((uv + vec2(0.5))*(amp), 1.0);

}
vec2 rotate(vec2 uv, vec2 center, float theta)
{
	vec2 temp;
	temp.x = dot(vec2(cos(theta), -sin(theta)), uv - center);
	temp.y = dot(vec2(sin(theta), cos(theta)), uv - center);
	return(temp)+center;
}

vec2 warp(vec2 uv, vec2 center, float strength, float r, float amp)
{

	return mod(center + (uv - center) / (strength*r*r + strength*r + 1.0), 1.0 / amp);
}

vec2 mirror(vec2 uv)
{
	vec2 parity = mod(floor(uv),2.0);
	vec2 sign = -2.0*parity+1.0;
	return mod( 2.0*parity+sign*mod(uv,1.0),1.0);
}

vec4 FUNCNAME(vec2 tc) {
	
    vec2 uv=gl_FragCoord.xy/iResolution.xy;
	float prop = iResolution.y / iResolution.x;
	
	const float sampleSize = 32.0;
	const int Samples = 16;//multiple of 2

	vec2 center = vec2(1.0,0.0);
	vec2 dir = vec2(0.0,1.0);
	float r = length(dir);
	
	float processColorSp = pow(cos(PI*(iGlobalTime-0.5)),2.0)*0.01;
	float processMB = iGlobalTime*0.5;// the blur trend is same as scale
	
	float temp = cos(PI*3.0/2.0*(iGlobalTime-0.33333));
	vec2 rollDir = 0.5*vec2(0.0,temp*temp);
	//time > 0.5
	float processWarpR = pow(cos(PI*(iGlobalTime-0.5)),2.0);// warp amp
	float processScalR = 4.0*(iGlobalTime-1.0)*(iGlobalTime-1.0);
	float processMBR= 1.0*(iGlobalTime-1.0)*(iGlobalTime-1.0);
	
	
	float x = iGlobalTime-0.3666666;
	float x2 = (iGlobalTime-0.3666666)*(iGlobalTime-0.3666666);
	float x3 = x2*(iGlobalTime-0.3666666);
	float x4 = x3*(iGlobalTime-0.3666666);
	float x5 = x4*(iGlobalTime-0.3666666);
	float x6 = x5*(iGlobalTime-0.3666666);

	vec4 color = vec4(0.0,0.0,0.0,1.0);
		//rollDir =  -vec2( 0.0,93222.0*x6 - 58954.0*x5 + 12738.0*x4 - 782.76*x3 - 84.065*x2 + 0.7151*x - 1e-08 );
	float ratoDeta = 135.0;
	rollDir =  vec2( 0.0,pow(2.0*iGlobalTime,13.0) );
	rollDir = rotate(rollDir, vec2(0.0), ratoDeta*0.0174532925);
	dir = rotate(dir, vec2(0.0), ratoDeta*0.0174532925);
		//processMB = clamp(abs(559332.0*x5-294770.0*x4+50952.0*x3-2348.28*x2-168.13*x+0.7151)*0.01,-0.2,0.2);
	processMB = clamp(pow(2.0*iGlobalTime,12.0),-0.2,0.2);
	processColorSp = clamp(rollDir.y*0.01,-0.01,0.01);
		
	if(iGlobalTime<0.5)
	{
		for (int i = 0; i < Samples; i += 2) //operating at 2 samples for better performance
		{
				vec2 temp1 = mirror(uv + float(i) / float(Samples)*dir*processMB + vec2(0.0)+rollDir);
				vec2 temp2 = mirror(uv + float(i) / float(Samples)*dir*processMB + vec2(processColorSp)+rollDir);
				vec2 temp3 = mirror(uv + float(i) / float(Samples)*dir*processMB + vec2(processColorSp)+rollDir);
				
				color.r += INPUT1(temp1).r;
				color.g += INPUT1(temp2).g;
				color.b += INPUT1(temp3).b;

				temp1 = mirror(uv + float(i+1) / float(Samples)*dir*processMB + vec2(0.0)+rollDir);
				temp2 = mirror(uv + float(i+1) / float(Samples)*dir*processMB + vec2(processColorSp)+rollDir);
				temp3 = mirror(uv + float(i+1) / float(Samples)*dir*processMB + vec2(processColorSp)+rollDir);
				
				color.r += INPUT1(temp1).r;
				color.g += INPUT1(temp2).g;
				color.b += INPUT1(temp3).b;
			
		}
	return color/float(Samples);
	}else
	{
		rollDir = -vec2(0.0, 60.0*(iGlobalTime-1.0)*(iGlobalTime-1.0)*(iGlobalTime-1.0)*(iGlobalTime-1.0));//15.0*(x-1.0)*(x-1.0)
		rollDir = rotate(rollDir, vec2(0.0), ratoDeta*0.0174532925);
		processMB = (iGlobalTime-1.0)*(iGlobalTime-1.0)*(iGlobalTime-1.0)*4.0;
		processColorSp = processMB*0.03;
		
		for (int i = 0; i < Samples; i += 2) //operating at 2 samples for better performance
		{
			vec2 temp1 = mirror(uv + float(i) / float(Samples)*dir*processMB + vec2(0.0)+rollDir);
			vec2 temp2 = mirror(uv + float(i) / float(Samples)*dir*processMB + vec2(processColorSp)+rollDir);
			vec2 temp3 = mirror(uv + float(i) / float(Samples)*dir*processMB + vec2(processColorSp)+rollDir);
				
			color.r += INPUT2(temp1).r;
			color.g += INPUT2(temp2).g;
			color.b += INPUT2(temp3).b;

			temp1 = mirror(uv + float(i+1) / float(Samples)*dir*processMB + vec2(0.0)+rollDir);
			temp2 = mirror(uv + float(i+1) / float(Samples)*dir*processMB + vec2(processColorSp)+rollDir);
			temp3 = mirror(uv + float(i+1) / float(Samples)*dir*processMB + vec2(processColorSp)+rollDir);
				
			color.r += INPUT2(temp1).r;
			color.g += INPUT2(temp2).g;
			color.b += INPUT2(temp3).b;
			
		}
		return color/float(Samples);
		}

}
