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

	float theta = 0.0;

	vec2 center = vec2(0.5);
	vec2 dir = (gl_FragCoord.xy) / iResolution.xy - center;
	float r = length(dir);
	
	
	float processWarp = exp((iGlobalTime-0.5)*100.0);// warp amp
	float processShade = pow(cos(PI*(iGlobalTime-0.5)),2.0);//shading amp
	float processScal = iGlobalTime*iGlobalTime*iGlobalTime*8.0; // scale amp
	float processColorSp = pow(cos(PI*(iGlobalTime-0.5)),2.0)*0.01;
	float processMB = processScal*0.5;// the blur trend is same as scale
	float rotaDegree=0.0;
	float processRota = PI/90.0*rotaDegree*iGlobalTime;//iGlobalTime..[0,0.5]
	
	//time > 0.5
	float processWarpR = pow(cos(2.0*PI*(iGlobalTime-0.5)),2.0);// warp amp
	//float processScalR = 2.0-iGlobalTime*2.0; // scale amp
	//float processMBR = (1.0-iGlobalTime)*0.2;
	//float processScalR = 4.0*(iGlobalTime-1.0)*(iGlobalTime-1.0);
	float processScalR = 2.0*pow(cos(2.0*PI*(iGlobalTime-0.5)),2.0);
	float processMBR= 4.0*(iGlobalTime-1.0)*(iGlobalTime-1.0);
	float processRotaR = PI/90.0*rotaDegree*(1.0-iGlobalTime);//iGlobalTime..[0.5,1.0]
	
	float shadingXL = 0.03*(normalizedRandom(vec2(1.0,iGlobalTime))-0.5)*processShade;
	float shadingyL = 0.03*(normalizedRandom(vec2(10.0,iGlobalTime/3.0))-0.5)*processShade;
	vec2 shadingVec2L = vec2(shadingXL,shadingyL);
	
	float shadingXR = 0.01*(normalizedRandom(vec2(1.0,iGlobalTime))-0.5)*processShade;
	float shadingyR = 0.01*(normalizedRandom(vec2(10.0,iGlobalTime/3.0))-0.5)*processShade;
	vec2 shadingVec2R = vec2(shadingXR,shadingyR);
	
	//uv = m + d/(pow(1.0+6.0*process,nr));
	//uv = (uv - m)/(1.0+0.6*process) + m;
	//uv = center + amp*dir / (2.0*r*2.0*r + r + 1.0);//Scaling and warp
	
	if (iGlobalTime<0.5)
    { 
		dir = rotate(dir, vec2(0.0), processRota);
		uv = center+(uv - center)*(1.0+0.5*processScal);//scaling compensation
		vec2 scaleDir =  dir*(1.0+0.5*processScal);
		
		uv = center + scaleDir/(pow(1.0+100.0*processWarp,r));//warp 
		scaleDir = scaleDir/(pow(1.0+100.0*processWarp,r));
		
		vec4 color = vec4(0.0);
		float count = 0.0;
		for (int i = 0; i < Samples; i += 2) //operating at 2 samples for better performance
		{
			vec2 temp1 =  mirror(uv + float(i) / float(Samples)*scaleDir*processMB + vec2(0.0)+shadingVec2L);
			vec2 temp2 = mirror(uv + float(i) / float(Samples)*scaleDir*processMB + vec2(processColorSp)+shadingVec2L);
			color.r += INPUT1(temp1).r;
			color.g += INPUT1(temp2).g;
			color.b += INPUT1(temp2).b;

			temp1 =  mirror(uv + float(i+1) / float(Samples)*scaleDir*processMB + vec2(0.0)+shadingVec2L);
			temp2 = mirror(uv + float(i+1) / float(Samples)*scaleDir*processMB + vec2(processColorSp)+shadingVec2L);
			color.r += INPUT1(temp1).r;
			color.g += INPUT1(temp2).g;
			color.b += INPUT1(temp2).b;
		}
		return color/float(Samples);
    }
	else {
		dir = rotate(dir, vec2(0.0), processRotaR);
		
		vec2 scaleDir;
		uv=gl_FragCoord.xy/iResolution.xy;
		if(iGlobalTime<=0.75)
		{
			uv = center+(uv - center)/(1.3+1.0*processScalR);//scaling compensation
			scaleDir =  dir/(1.3+1.0*processScalR);
			
			uv = center + scaleDir/(pow(1.0+0.1*processWarpR,r));//warp 
			scaleDir = scaleDir/(pow(1.0+0.1*processWarpR,r));
		}
		if(iGlobalTime>0.75)//scale hit
		{

			float scaleHit = 0.2*pow(cos(4.0*PI*(iGlobalTime-0.875)),2.0);
			uv = center+(uv - center)/(1.0+scaleHit);//scaling compensation
			scaleDir =  dir/(1.0+scaleHit);
			
			uv = center + scaleDir/(pow(1.0+0.2*scaleHit,r));//warp 
			scaleDir = scaleDir/(pow(1.0+0.2*scaleHit,r));
			
			//particular shading for "hit"
			shadingXR = 0.05*(normalizedRandom(vec2(1.0,iGlobalTime))-0.5);
			shadingyR = 0.025*(normalizedRandom(vec2(10.0,iGlobalTime/3.0))-0.5);
			shadingVec2R = vec2(shadingXR,shadingyR);
		}
		
		vec4 color = vec4(0.0);
		float count = 0.0;
		for (int i = 0; i < Samples; i += 2) //operating at 2 samples for better performance
		{
			vec2 temp1 =  mirror(uv + float(i) / float(Samples)*scaleDir*processMBR + vec2(0.0)+shadingVec2R);
			vec2 temp2 = mirror(uv + float(i) / float(Samples)*scaleDir*processMBR + vec2(processColorSp)+shadingVec2R);
			color.r += INPUT2(temp1).r;
			color.g += INPUT2(temp2).g;
			color.b += INPUT2(temp2).b;

			temp1 =  mirror(uv + float(i+1) / float(Samples)*scaleDir*processMBR + vec2(0.0)+shadingVec2R);
			temp2 = mirror(uv + float(i+1) / float(Samples)*scaleDir*processMBR + vec2(processColorSp)+shadingVec2R);
			color.r += INPUT2(temp1).r;
			color.g += INPUT2(temp2).g;
			color.b += INPUT2(temp2).b;
		}
		return color/float(Samples);
		//return INPUT2(uv);
	}   
}