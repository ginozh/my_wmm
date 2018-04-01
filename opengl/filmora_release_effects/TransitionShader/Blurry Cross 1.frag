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
	return(temp);
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
	
	float x = iGlobalTime;
	float x2 = (iGlobalTime)*(iGlobalTime);
	float x3 = x2*(iGlobalTime);
	float x4 = x3*(iGlobalTime);
	float x5 = x4*(iGlobalTime);
	float x6 = x5*(iGlobalTime);
	
	float rotateDeta = PI/180.0*(-45.0);
	
	if (iGlobalTime<0.2)
    { 	

		vec4 color = vec4(0.0,0.0,0.0,1.0);
		rollDir =  -vec2( 0.0, 1E+06*x6 - 641535.0*x5 + 138013.0*x4 - 13705.0*x3 + 622.28*x2 - 5.0144*x + 0.00481 )*1.7;
		
		float processMB = -clamp( (6.00E+06*x5-3.21E+06*x4+5.52E+05*x3-4.11E+04*x2+1.24E+03*x-5.01E+00)*0.003,-0.2,0.2);
		processColorSp = clamp(rollDir.y*0.009,-0.01,0.01);
		rollDir= rotate(rollDir, vec2(0.0),rotateDeta)/1.414;
		dir= rotate(dir, vec2(0.0),rotateDeta);
		
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
		//return INPUT1(mirror(uv +rollDir));
    }
	else {
			float x = iGlobalTime-0.3361;
		  x2 = (x)*(x);
		  x3 = x2*(x);
		  x4 = x3*(x);
		  x5 = x4*(x);
		  x6 = x5*(x);

		rollDir = -vec2( 0.0,-148.19*x6 + 238.72*x5 + 25.04*x4 - 256.21*x3 + 180.65*x2 - 47.609*x + 3.9918)*0.5;
			
		processMBR = (-888.6*x5+1193.6*x4+100.16*x3-768.63*x2+361.3*x -47.609)*0.02;
		
		if(iGlobalTime > 0.5870 )
			processMBR = 0.0;
		
		processColorSp = processMBR*0.0001;
			
		rollDir= rotate(rollDir, vec2(0.0),rotateDeta)/1.414;
		dir= rotate(dir, vec2(0.0),rotateDeta);
		
		vec4 color = vec4(0.0,0.0,0.0,1.0);
		for (int i = 0; i < Samples; i += 2) //operating at 2 samples for better performance
		{
			vec2 temp1 = mirror(uv + float(i) / float(Samples)*dir*processMBR + vec2(0.0)+rollDir);
			vec2 temp2 = mirror(uv + float(i) / float(Samples)*dir*processMBR + vec2(processColorSp)+rollDir);
			vec2 temp3 = mirror(uv + float(i) / float(Samples)*dir*processMBR + vec2(processColorSp)+rollDir);
				
			color.r += INPUT2(temp1).r;
			color.g += INPUT2(temp2).g;
			color.b += INPUT2(temp3).b;

			temp1 = mirror(uv + float(i+1) / float(Samples)*dir*processMBR + vec2(0.0)+rollDir);
			temp2 = mirror(uv + float(i+1) / float(Samples)*dir*processMBR + vec2(processColorSp)+rollDir);
			temp3 = mirror(uv + float(i+1) / float(Samples)*dir*processMBR + vec2(processColorSp)+rollDir);
				
			color.r += INPUT2(temp1).r;
			color.g += INPUT2(temp2).g;
			color.b += INPUT2(temp3).b;
		}
		return color/float(Samples);
	}   
}
