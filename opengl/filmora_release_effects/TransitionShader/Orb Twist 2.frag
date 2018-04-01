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

vec2 fishEye (vec2 p, vec2 m,float prop, float strength)
{
	vec2 d = (p - m);
	float r = sqrt(dot(d, d)); 
	float power = ( 2.0 * 3.141592 / (2.0 * sqrt(dot(m, m))) ) *
				(0.004*strength);
	float bind;
	if (power > 0.0) bind = sqrt(dot(m, m));
	else {if (prop < 1.0) bind = m.x; else bind = m.y;}
	vec2 uv;
	if (power > 0.0)//fisheye
		uv = m + normalize(d) * tan(r * power) * bind / tan( bind * power);
	else if (power < 0.0)//antifisheye
		uv = m + normalize(d) * atan(r * -power * 10.0) * bind / atan(-power * bind * 10.0);
	else uv = p;
	return uv;  
}


vec4 FUNCNAME(vec2 tc) {
	
    vec2 uv=gl_FragCoord.xy/iResolution.xy;
	float prop = iResolution.y / iResolution.x;
	
	const float sampleSize = 32.0;
	const int Samples = 16;//multiple of 2

	vec2 center = vec2(0.5,0.5);
	vec2 dir = vec2(0.0,1.0);
	float r = length(dir);
	
	float processColorSp = pow(cos(PI*(iGlobalTime-0.5)),2.0)*0.01;
	float processMB = iGlobalTime*0.5;// the blur trend is same as scale
	
	float rotateDeta = PI/180.0*-45.0;
	vec2 rollDir = vec2(0.0);
	
	float processMBR= 0.0;//1.0*(iGlobalTime-1.0)*(iGlobalTime-1.0);
	
	float x = iGlobalTime;
	float x2 = (iGlobalTime)*(iGlobalTime);
	float x3 = x2*(iGlobalTime);
	float x4 = x3*(iGlobalTime);
	float x5 = x4*(iGlobalTime);
	float x6 = x5*(iGlobalTime);
	float fishEyeAmp = (28302.0*x5 - 15699.0*x4 + 2423.3*x3 - 44.757*x2 + 0.3137*x - 0.0016);
	
if (iGlobalTime<0.2)
    { 	
		uv = fishEye (uv,center,prop,70.0*fishEyeAmp);
		
		vec4 color = vec4(0.0,0.0,0.0,1.0);
		rollDir =  vec2( 0.0,1.0- pow(abs(sin(PI/2.0*(5.2*(iGlobalTime-1.0/5.2)))),0.5) );
		processMB = clamp(rollDir.y,-0.01,0.01);
		processColorSp = clamp(fishEyeAmp,-0.005,0.005);
		
		uv = rotate(uv, vec2(1.0), -rollDir.y);
		
		//rollDir= rotate(rollDir, vec2(0.0),rotateDeta);
		dir= rotate(dir, vec2(0.0),rotateDeta);
		rollDir = vec2(0.0);
		
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
    }
	else {

		fishEyeAmp =pow((-(iGlobalTime-1.0)*1.3),1.8);
		processColorSp =fishEyeAmp*0.005;
		uv = fishEye (uv,center,prop,100.0*fishEyeAmp);
		
		uv = rotate(uv, vec2(0.0,0.0), rollDir.y);
		
		//if(iGlobalTime<0.4)//0.3-0.6

		rollDir = vec2( 0.0,-pow(1.25*(iGlobalTime-1.0),6.0));//-(1.25(x-1.0))^6.0
			
		processMBR = pow(1.25*(iGlobalTime-1.0),5.0)*0.1;//0.4-iGlobalTime;
			
		vec4 color = vec4(0.0,0.0,0.0,1.0);
		rollDir= rotate(rollDir, vec2(0.0),rotateDeta);
		dir= rotate(dir, vec2(0.0),rotateDeta);
		uv = rotate(uv, vec2(1.0), -rollDir.y);
		rollDir = vec2(0.0);
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

		//return INPUT2(uv);
	}   
}