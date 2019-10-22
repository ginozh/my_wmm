#ifdef GL_ES
precision mediump float;
#endif
#
uniform sampler2D mSample0;
uniform sampler2D mSample1;
uniform float qt_Opacity;
varying vec2 qt_TexCoord0;

//varying vec2 textureCoord;
//uniform vec4 tint;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

uniform vec2 u_curPos;
uniform vec2 u_nextPos;
uniform float u_alpha;
uniform float u_CurRotate;
uniform float u_nextRotate;
uniform vec2 u_curScale;
uniform vec2 u_nextScale;
#define motionBlur 0.5
#define radialBlur 0.5
#define rotateblur 0.5

const float PI = 3.141592653589793;

//#define  release
#define  debug

#ifdef debug
float iGlobalTime = u_global_time;
vec2 curPos = vec2(iGlobalTime/100.0,iGlobalTime/100.0);
vec2 nextPos = vec2((iGlobalTime+1.0)/100.0,(iGlobalTime+1.0)/100.0);
float alpha = 0.1;//range [0,100]
//mod: a mod b == a - (b * floor(a/b))
float CurRotate = iGlobalTime-1.0;//45.0;//0.0;//iGlobalTime*360.0/25.0;//sin(iGlobalTime*5.9)*360.0;//degree
float nextRotate = 45.0;//(iGlobalTime+1.0)*360.0/25.0;//sin(iGlobalTime*5.0)*360.0;
vec2 curScale = vec2(iGlobalTime/100.0+0.5, iGlobalTime/100.0+0.5);//vec2(1.0,1.0);//vec2(iGlobalTime/26.0, iGlobalTime/26.0);//vec2(sin(iGlobalTime*6.0)); //range [0,1]
vec2 nextScale = vec2(1.0,1.0);//vec2((iGlobalTime+1.0)/26.0, (iGlobalTime+1.0)/26.0);//vec2(sin(iGlobalTime*5.9));//range [0,1]
#else  
vec2 curPos = u_curPos;// current position [(0.0,0.0),(1.0,1.0)]
vec2 nextPos  = u_nextPos;//next position range [(0.0,0.0),(1.0,1.0)]
float alpha = u_alpha;//range [0,100]
float CurRotate = u_CurRotate;//degree
float nextRotate = u_nextRotate;
vec2 curScale = u_curScale ; //range [0,1]
vec2 nextScale = u_nextScale;//range [0,1]
#endif 
#
vec2 iResolution = u_resolution;//gl_FragCoord.xy / qt_TexCoord0;
const int Samples = 32; //multiple of 2
vec4 color = vec4(0.0,0.0,0.0,0.0);
vec2 scaleFunc(vec2 uv, vec2 scale,vec2 center)
{

	return (uv - center)/(scale) + center ;

}
vec2 rotateFunc(vec2 uv, vec2 center, float theta)
{
	vec2 temp;
	vec2 xy = uv*iResolution.xy;
	vec2 unNormalCenter = center*iResolution.xy;
	
	temp.x = dot(vec2(cos(theta), -sin(theta)), xy - unNormalCenter);
	temp.y = dot(vec2(sin(theta), cos(theta)), xy - unNormalCenter);
	return(temp+unNormalCenter)/iResolution.xy;
}

void blur(vec2 uv, int i, vec2 dir, float ratio)
{
	vec2 temp = uv + float(i) / float(Samples)*dir;
	float grid = (step(temp.x,1.0)-step(temp.x,0.0))*(step(temp.y,1.0)-step(temp.y,0.0));
	//color += mix( INPUT1(gl_FragCoord.xy/iResolution.xy), INPUT2( temp*grid ),grid*INPUT2( temp*grid ).a*alpha/100.0)*ratio;
	color += mix( texture2D(mSample0, gl_FragCoord.xy/iResolution.xy), texture2D(mSample1, temp*grid ),grid*texture2D(mSample1, temp*grid ).a*alpha/100.0)*ratio;
}

vec2 rotate_point(float cx,float cy,float angle,vec2 p)
{
  float s = sin(angle);
  float c = cos(angle);

  // translate point back to origin:
  p.x -= cx;
  p.y -= cy;

  // rotate point
  float xnew = p.x * c - p.y * s;
  float ynew = p.x * s + p.y * c;

  // translate point back:
  p.x = xnew + cx;
  p.y = ynew + cy;
  return p;
}
#if 1
void main() 
{
#if 0
    vec2 coord = qt_TexCoord0;
    float u_angle = PI/180.0*(-CurRotate);
    float sin_factor = sin(u_angle);
    float cos_factor = cos(u_angle);
    //coord = (coord - 0.5) * mat2(cos_factor, sin_factor, -sin_factor, cos_factor);
    coord = vec2((coord.x - 0.5) * (u_resolution.x / u_resolution.y), coord.y - 0.5) * mat2(cos_factor, sin_factor, -sin_factor, cos_factor);

    coord += 0.5;

    gl_FragColor = texture2D(mSample1, coord);
    return;
#endif

#if 0
    ////gl_FragColor = texture2D(mSample1, qt_TexCoord0);
    gl_FragColor = vec4(1.0,0.0,0.0,1.0);
    return;
#endif

    //qt_TexCoord0 x:[0.0, 1.0], y:[0.0,1.0]  左上. 取值0.5, 0.5
    //gl_FragCoord x:[0.0, 400.0], y:[0.0,400.0]  左下. 取值200, 200
    //u_resolution: x:width, y:height
    float w = u_resolution.x; 
    float h = u_resolution.y; 
    vec2 uv=qt_TexCoord0;
	vec2 center = vec2(0.5);

    //1, scale
    uv=scaleFunc(uv, curScale, center);
#if 0
    float ToI, ToJ;
    //float dToImgScale=curScale.x;
    ///dToImgScale = h/cropHeight; //cropHeight缩放到h
    float rectToImg_w = w * curScale.x; //320
    float rectToImg_h = h * curScale.y;

    //屏幕坐标                                                     => sample坐标(左下)
    //gl_FragCoord.x:[w/2.0-rectToImg_w/2.0,w/2.0+rectToImg_w/2.0] => [0,1]
    //=>-w/2.0                  [-rectToImg_w/2.0,rectToImg_w/2.0]
    //=> *2.0/rectToImg_w      [-1.0,1.0]
    //=> +1.0                   [0.0,2.0]
    //=> w/2.0                 [0.0,w]
    ToI = ((gl_FragCoord.x-w/2.0)*2.0/rectToImg_w+1.0)*w/2.0;
    ToJ = ((gl_FragCoord.y-h/2.0)*2.0/rectToImg_h+1.0)*h/2.0;

    uv=vec2(ToI / w, 1.0 - ToJ / h);

    //gl_FragColor = texture2D(mSample1, qt_TexCoord0);
    ////gl_FragColor = texture2D(mSample1, vec2(ToI / w, 1.0 - ToJ / h));
#endif 

    //2, ratote
    if (CurRotate != 0.0)
    {
        float rotateDeta = PI/180.0*(-CurRotate);
        //uv=rotate_point(0.5, 0.5, rotateDeta, uv);
        uv=rotateFunc(uv, center, rotateDeta);
    }

    //3, move
	uv = uv +center- curPos;

    //4, alpha
    vec4 color = texture2D(mSample1, uv);
    color.a = color.a * alpha;

    gl_FragColor = color;
    return;
}
#endif

#if 0
void main() 
{
	vec2 tc = qt_TexCoord0;
	vec2 uv=gl_FragCoord.xy/iResolution.xy;

	vec2 center = vec2(0.5);
	//vec2 dir = motionBlur*normalize((nextPos - curPos+0.0001)*iResolution.xy)*length(nextPos - curPos+0.00001);
	vec2 dir = motionBlur*normalize((nextPos - curPos+0.0001)*iResolution.xy)*length(nextPos - curPos+0.00001)* (gl_FragCoord.xy-iResolution.xy/2.0)/iResolution.xy;
	
	float processRota = CurRotate*0.01745329;
	float dirRota = -(180.0-CurRotate)*0.01745329;
	uv = uv +center- curPos;
	//uv = uv +center - vec2(0.5);

	uv = rotateFunc(uv,center,processRota);
	dir = rotateFunc(dir, vec2(0.0), dirRota);
	


	uv = scaleFunc(uv,curScale,center);//scaling
	vec2 scaleDir= dir/(length(curScale)+0.00001);



	
	
	vec2 radialDir = radialBlur*(uv - center)*length(nextScale - curScale) ;
	float detaRotate = (nextRotate - CurRotate);
	vec2 RotateDir = rotateblur*rotateFunc(normalize(uv - center), vec2(0.0), - 1.570796) * length(uv - center)*detaRotate*0.01745329;
	
	vec2 totalDir = radialDir+ RotateDir + dir;
	
	float count = 0.0;
	float grid = (step(uv.x,1.0)-step(uv.x,0.0))*(step(uv.y,1.0)-step(uv.y,0.0));
	
	for (int i = 0; i < Samples; i += 2) //operating at 2 samples for better performance
	{	

		blur(uv, i,totalDir,1.0);
		blur(uv, i+1,totalDir,1.0);

	}
	//return color/float(Samples);
    gl_FragColor = qt_Opacity * color/float(Samples);
}
#endif
