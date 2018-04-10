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
vec2 scale(vec2 uv, vec2 center, vec2 amp)
{

	return (uv - center)*amp + center;

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

	vec2 center = vec2(0.5,0.0);
	vec2 dir = vec2(1.0,0.0);
	float r = length(dir);
	
	float processColorSp = pow(cos(PI*(iGlobalTime-0.5)),2.0)*0.001;
	float processMB = iGlobalTime*0.2;// the blur trend is same as scale
	
	float temp = cos(PI*3.0/2.0*(iGlobalTime-0.33333));
	vec2 rollDir = 0.5*vec2(0.0,temp*temp);
	//time > 0.5
	float processWarpR = pow(cos(PI*(iGlobalTime-0.5)),2.0);// warp amp
	float processScalR = 4.0*(iGlobalTime-1.0)*(iGlobalTime-1.0);
	float processMBR= 0.2*(iGlobalTime-1.0)*(iGlobalTime-1.0);
	
	if (iGlobalTime<0.3333333333333)
    { 	
		//float HoriScale = 0.5*cos(4.7123889*(iGlobalTime-0.3333333333333));
		float vertiScale = pow( (3.3333333333*iGlobalTime),7.0)*2.0;
		float HoriScale = 0.3*(1.0-uv.x)*vertiScale+1.0;
		
		vec4 color = vec4(0.0,0.0,0.0,1.0);
		//vec2 amp = vec2(1.0/(1.0+uv.y*3.3*boxFlit),1.0/(1.0+uv.y*0.3*boxFlit));
		vec2 amp = vec2(1.0/HoriScale,1.0/(1.0+(1.0-uv.x)*vertiScale));
		//vec2 amp = vec2(1.0,1.0);
		center = vec2(1.0,0.5);
		uv = scale(uv,center,amp)-vec2(vertiScale*0.1,0.0);
		
		float bottom = 1.0;
		float top = (1.0+(1.0-uv.x)*HoriScale);
		float tan = ((top - bottom)/2.0+0.000001)/vertiScale ;
		float H = 0.5*bottom/tan;
		vec2 center =vec2(1.0+H,0.5);
		dir = normalize(uv-center);
		processMB = clamp(vertiScale*0.1,-0.05,0.05);
		for (int i = 0; i < Samples; i += 2) //operating at 2 samples for better performance
		{
			color.r += INPUT1(mod(uv + float(i) / float(Samples)*dir*processMB + vec2(0.0), 1.0)).r;
			color.g += INPUT1(mod(uv + float(i) / float(Samples)*dir*processMB + vec2(processColorSp), 1.0)).g;
			color.b += INPUT1(mod(uv + float(i) / float(Samples)*dir*processMB + vec2(processColorSp), 1.0)).b;

			color.r += INPUT1(mod(uv + float(i + 1) / float(Samples)*dir*processMB + vec2(0.0), 1.0)).r;
			color.g += INPUT1(mod(uv + float(i + 1) / float(Samples)*dir*processMB + vec2(processColorSp), 1.0)).g;
			color.b += INPUT1(mod(uv + float(i + 1) / float(Samples)*dir*processMB + vec2(processColorSp), 1.0)).b;
		}
		return color/float(Samples);
    }
	else {
			float temp = 1.5*(iGlobalTime-0.3333333333333) - 1.0;
			//rollDir = vec2( 1.0-temp*temp*temp*temp, 0.0)*2.0;
			
			rollDir = vec2( 1.0-temp*temp*temp*temp,0.0)*2.0;;
			vec4 color = vec4(0.0,0.0,0.0,1.0);
			vec2 amp = vec2(1.0,1.0/(1.0+2.0*(uv.x)*(1.0-rollDir.x/2.0)));
			uv = (uv - vec2(0.5))*amp.y+vec2(0.5);

			float bottom = 1.0;
			float top = (1.0+(1.0-uv.x)*1.0);
			float tan = ((top - bottom)/2.0)/1.0 ;
			float H = 0.5*bottom/tan;
			vec2 center =vec2(1.0+H,0.5);
			dir = normalize(uv-center);
			rollDir = -rollDir;
			
			processMB = clamp(rollDir.x*0.1,-0.05,0.05);
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