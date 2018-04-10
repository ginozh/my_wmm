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
	vec2 dir = (gl_FragCoord.xy) / iResolution.xy - center;
	float r = length(dir);
	
	
	float processWarp = exp((iGlobalTime-0.5)*100.0);// warp amp
	float processShade = pow(cos(PI*(iGlobalTime-0.5)),2.0);//shading amp
	float processScal = iGlobalTime*2.0; // scale amp
	float processColorSp = pow(cos(PI*(iGlobalTime-0.5)),2.0)*0.01;
	float processMB = iGlobalTime*0.5;// the blur trend is same as scale
	float rotaDegreeL=-70.0;
	float processRota = PI/90.0*rotaDegreeL*iGlobalTime;//iGlobalTime..[0,0.5]
	
	//time > 0.5
	float processWarpR = pow(cos(PI*(iGlobalTime-0.5)),2.0);// warp amp
	float processScalR = 4.0*(iGlobalTime-1.0)*(iGlobalTime-1.0);
	float processMBR= 10.0*(iGlobalTime-1.0)*(iGlobalTime-1.0);
		
	float rotaDegreeR=-74.9;
	float processRotaR = PI/90.0*rotaDegreeR*0.5*sin(PI*(iGlobalTime-0.5));//iGlobalTime..[0.5,1.0]
	
	float shadingXL = 0.1*(normalizedRandom(vec2(1.0,iGlobalTime))-0.5)*processShade;
	float shadingyL = 0.1*(normalizedRandom(vec2(10.0,iGlobalTime/3.0))-0.5)*processShade;
	vec2 shadingVec2L = vec2(shadingXL,shadingyL);
	
	
	float shadingXR = 0.1*(normalizedRandom(vec2(1.0,iGlobalTime))-0.5)*processShade;
	float shadingyR = 0.1*(normalizedRandom(vec2(10.0,iGlobalTime/3.0))-0.5)*processShade;
	vec2 shadingVec2R = vec2(shadingXR,shadingyR);
	
	
	if (iGlobalTime<0.5)
    { 	
		uv = rotate(uv, center, processRota);
		vec2 warpDir = rotate(dir, vec2(0.0), processRota);
		dir = rotate(warpDir, vec2(0.0),1.570795);
		//uv = center+(uv - center)/(1.0+1.0*processScal);//scaling compensation
		//vec2 scaleDir = dir; //dir/(1.0+1.0*processScal);
		
		uv = center + warpDir/(pow(1.0+20.0*processWarp,r));//warp
		
		vec4 color = vec4(0.0);
		float count = 0.0;
		for (int i = 0; i < Samples; i += 2) //operating at 2 samples for better performance
		{
			vec2 temp1 =  mirror(uv + float(i) / float(Samples)*dir*processMB + vec2(0.0)+shadingVec2L);
			vec2 temp2 = mirror(uv + float(i) / float(Samples)*dir*processMB + vec2(processColorSp)+shadingVec2L);
			color.r += INPUT1(temp1).r;
			color.g += INPUT1(temp2).g;
			color.b += INPUT1(temp2).b;

			temp1 =  mirror(uv + float(i+1) / float(Samples)*dir*processMB + vec2(0.0)+shadingVec2L);
			temp2 = mirror(uv + float(i+1) / float(Samples)*dir*processMB + vec2(processColorSp)+shadingVec2L);
			color.r += INPUT1(temp1).r;
			color.g += INPUT1(temp2).g;
			color.b += INPUT1(temp2).b;
		}
		return color/float(Samples);
    }
	else {
		center = vec2(1.0,1.0);
		dir = (gl_FragCoord.xy) / iResolution.xy - center;
		uv = rotate(uv, center, 1.307251);//pre-Rotate 74.9 degree
		vec2 dir = rotate(dir, vec2(0.0), 1.307251);
		
		uv = rotate(uv, center, processRotaR);
		vec2 warpDir = rotate(dir, vec2(0.0), processRotaR);
		uv = center + warpDir/(pow(1.0+2.0*processWarpR,r));//warp 
		
		//warpDir = warpDir/(pow(1.0+2.0*processWarpR,r));
		dir = rotate(warpDir, vec2(0.0),1.570795);
		
		vec2 scaleDir = dir; //dir/(1.0+1.0*processScal);
		
		vec4 color = vec4(0.0);
		
		float count = 0.0;
		for (int i = 0; i < Samples; i += 2) //operating at 2 samples for better performance
		{
			vec2 temp1 =  mirror(uv + float(i) / float(Samples)*scaleDir*processMBR + vec2(0.0)+shadingVec2L);
			vec2 temp2 = mirror(uv + float(i) / float(Samples)*scaleDir*processMBR + vec2(processColorSp)+shadingVec2L);
			color.r += INPUT2(temp1).r;
			color.g += INPUT2(temp2).g;
			color.b += INPUT2(temp2).b;

			temp1 =  mirror(uv + float(i+1) / float(Samples)*scaleDir*processMBR + vec2(0.0)+shadingVec2L);
			temp2 = mirror(uv + float(i+1) / float(Samples)*scaleDir*processMBR + vec2(processColorSp)+shadingVec2L);
			color.r += INPUT2(temp1).r;
			color.g += INPUT2(temp2).g;
			color.b += INPUT2(temp2).b;
		}
		return color/float(Samples);
		//return INPUT2(uv);
	}   
}