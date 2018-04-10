/*{
	"GUID":"55D37319-8F37-4593-BBAF-9FF0A6307C01",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"tex12.png"
		}
	],
	"PARAMS":[
		{
			"NAME":"bright",
			"TYPE":"float",
			"DEFAULT":0.0,
			"MIN":0.0,
			"MAX":1.0
		}
	] 
}*/

float iGlobalTime = PREFIX(global_time);
float noise(vec2 p)
{
	vec2 v = vec2(1.,2.*cos(iGlobalTime))*iGlobalTime*8. + p*1.;
	v.x = mod(v.x, 1.0);
	v.y = mod(v.y, 1.0);
	float sample = INPUT2(v).x;
	sample *= sample;
	return sample;
}

float onOff(float a, float b, float c)
{
	return step(c, sin(iGlobalTime + a*cos(iGlobalTime*b)));
}

float ramp(float y, float start, float end)
{
	float inside = step(start,y) - step(end,y);
	float fact = (y-start)/(end-start)*inside;
	return (1.-fact) * inside;
	
}

float stripes(vec2 uv)
{
	
	float noi = noise(uv*vec2(0.5,1.) + vec2(1.,3.));
	return ramp(mod(uv.y*4. + iGlobalTime/2.+sin(iGlobalTime + sin(iGlobalTime*0.63)),1.),0.5,0.6)*noi;
}

vec3 getVideo(vec2 uv)
{
	vec2 look = uv;
	float window = 1./(1.+20.*(look.y-mod(iGlobalTime/4.,1.))*(look.y-mod(iGlobalTime/4.,1.)));
	look.x = look.x + (sin(look.y*10. + iGlobalTime)/50.*onOff(4.,4.,.3)*(1.+cos(iGlobalTime*80.))*window*PREFIX(fade));
	float vShift = 0.4*onOff(2.,3.,.9)*(sin(iGlobalTime)*sin(iGlobalTime*20.) + 
										 (0.5 + 0.1*sin(iGlobalTime*200.)*cos(iGlobalTime)));
	look.y = mod(look.y + (vShift*PREFIX(fade)), 1.);
	vec3 video = vec3(INPUT1(look));
	return video;
}

vec2 screenDistort(vec2 uv)
{
	uv -= vec2(.5,.5);
	uv = uv*1.2*(1./1.2+2.*uv.x*uv.x*uv.y*uv.y);
	uv += vec2(.5,.5);
	return uv;
}

vec4 FUNCNAME(vec2 tc)
{
	vec2 uv = gl_FragCoord.xy / iResolution.xy;
	//uv = screenDistort(uv);
	vec3 video = getVideo(uv);
	float vigAmt = 3.+.3*sin(iGlobalTime + 5.*cos(iGlobalTime*5.));
	float vignette = (1.-vigAmt*(uv.y-.5)*(uv.y-.5))*(1.-vigAmt*(uv.x-.5)*(uv.x-.5));
	
	video += stripes(uv)*PREFIX(fade);
	video += noise(uv*2.)/2.*PREFIX(fade);
	
	float vig = vignette *((12.)+mod(uv.y*30.+iGlobalTime,1.))/(13.-PREFIX(bright)*10.0);
	vig = vig * PREFIX(fade) + (1.0-PREFIX(fade));
	video *= vig;
	
	return vec4(video,1.0);
}