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
	const int Samples = 16; //multiple of 2

	vec2 center = vec2(0.5);
	vec2 dir = (gl_FragCoord.xy) / iResolution.xy - center;
	float r = length(dir);
	
	
	float processWarp = 0.0;//exp((iGlobalTime-0.5)*100.0);// warp amp
	float processShade = pow(cos(PI*(iGlobalTime-0.5)),2.0);//shading amp
	
	float processScal = (1.0-pow(abs(iGlobalTime*5.0),3.5))*0.8+0.2; // scale amp
	float processColorSp = pow(cos(PI*(iGlobalTime-0.5)),2.0)*0.01;
	float processMB = 0.8*(1.0-processScal);// the blur trend is same as scale
	float rotaDegree=0.0;
	float processRota = PI/90.0*rotaDegree*iGlobalTime;//iGlobalTime..[0,0.5]
	
	//time > 0.5
	float processWarpR = 0.0;//pow(cos(PI*(iGlobalTime-0.5)),2.0);// warp amp
	//float processScalR = 2.0-iGlobalTime*2.0; // scale amp
	//float processMBR = (1.0-iGlobalTime)*0.2;
	float processScalR = 7.0*exp(-12.0*(iGlobalTime-0.2))+1.0;
	float processMBR= (processScalR-1.0)*0.1;
	float temp = 1.0-iGlobalTime;
	float processRotaR = PI/90.0*rotaDegree*0.48828125*temp*temp*temp;//iGlobalTime..[0.5,1.0] , (2(1.0-x))^3.0
	
	float shadingXL = 0.03*(normalizedRandom(vec2(1.0,iGlobalTime))-0.5)*processShade;
	float shadingyL = 0.03*(normalizedRandom(vec2(10.0,iGlobalTime/3.0))-0.5)*processShade;
	vec2 shadingVec2L = vec2(shadingXL,shadingyL);
	
	
	float shadingXR = 0.01*(normalizedRandom(vec2(1.0,iGlobalTime))-0.5)*processShade;
	float shadingyR = 0.01*(normalizedRandom(vec2(10.0,iGlobalTime/3.0))-0.5)*processShade;
	vec2 shadingVec2R = vec2(shadingXR,shadingyR);
	
	//uv = m + d/(pow(1.0+6.0*process,nr));
	//uv = (uv - m)/(1.0+0.6*process) + m;
	//uv = center + amp*dir / (2.0*r*2.0*r + r + 1.0);//Scaling and warp

	
	
if (iGlobalTime<0.2)
    { 		
		dir = rotate(dir, vec2(0.0), processRota);
		uv = center+(uv - center)/processScal;//scaling compensation
		vec2 scaleDir =  dir/processScal;
		
		uv = center + scaleDir/(pow(1.0+20.0*processWarp,r));//warp 
		
		vec4 color = vec4(0.0);
		float count = 0.0;
		for (int i = 0; i < Samples; i += 2) //operating at 2 samples for better performance
		{
				vec2 temp1 = mirror(uv + float(i) / float(Samples)*scaleDir*processMB + vec2(0.0));
				vec2 temp2 = mirror(uv + float(i) / float(Samples)*scaleDir*processMB + vec2(processColorSp));
				vec2 temp3 = mirror(uv + float(i) / float(Samples)*scaleDir*processMB + vec2(processColorSp));
				
				color.r += INPUT1(temp1).r;
				color.g += INPUT1(temp2).g;
				color.b += INPUT1(temp3).b;

				temp1 = mirror(uv + float(i+1) / float(Samples)*scaleDir*processMB + vec2(0.0));
				temp2 = mirror(uv + float(i+1) / float(Samples)*scaleDir*processMB + vec2(processColorSp));
				temp3 = mirror(uv + float(i+1) / float(Samples)*scaleDir*processMB + vec2(processColorSp));
				
				color.r += INPUT1(temp1).r;
				color.g += INPUT1(temp2).g;
				color.b += INPUT1(temp3).b;
		}
		return color/float(Samples);
    }
	else {
		
		dir = rotate(dir, vec2(0.0), processRotaR);
		uv = rotate(uv,vec2(0.5),processRotaR);
		//uv=gl_FragCoord.xy/iResolution.xy;
		uv = center+(uv - center)/processScalR;//scaling compensation
		//vec2 scaleDir =  dir*(0.2*exp(-8.0*(x-0.2)));
		vec2 scaleDir =  dir/processScalR;
		vec4 color = vec4(0.0);
		
		float count = 0.0;
		for (int i = 0; i < Samples; i += 2) //operating at 2 samples for better performance
		{
			vec2 temp1 = mirror(uv + float(i) / float(Samples)*scaleDir*processMBR + vec2(0.0));
			vec2 temp2 = mirror(uv + float(i) / float(Samples)*scaleDir*processMBR + vec2(processColorSp));
			vec2 temp3 = mirror(uv + float(i) / float(Samples)*scaleDir*processMBR + vec2(processColorSp));
				
			color.r += INPUT2(temp1).r;
			color.g += INPUT2(temp2).g;
			color.b += INPUT2(temp3).b;

			temp1 = mirror(uv + float(i+1) / float(Samples)*scaleDir*processMBR + vec2(0.0));
			temp2 = mirror(uv + float(i+1) / float(Samples)*scaleDir*processMBR + vec2(processColorSp));
			temp3 = mirror(uv + float(i+1) / float(Samples)*scaleDir*processMBR + vec2(processColorSp));
				
			color.r += INPUT2(temp1).r;
			color.g += INPUT2(temp2).g;
			color.b += INPUT2(temp3).b;
		}
		return color/float(Samples);
		//return INPUT2(uv);
	}   
}