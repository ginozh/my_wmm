/********************************************************************
author: RuanShengQiang
date: 2017/3/21
********************************************************************/
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

uniform float Opacity;
uniform float Duration;
uniform float Enable;//if enable > 0.5, the effect is enabled;
uniform float Radius;
uniform vec3 Color;
uniform vec2 iMouse;
uniform float clicked; // it should be used type "bool", but the client only can transimit the type "float".If this variable is set to be">0.5", it shows the mouse is left-clicked. 
void main()
{
    float maxSize = u_resolution.x>=u_resolution.y?u_resolution.x:u_resolution.y;
    float radius = Radius/2.0+0.01;
    vec3 rgb = Color;
    float alpha = 1.0;
    float soft = 100.0*0.5;
    vec4 img = texture2D(mSample0,textureCoord);
    if(Enable>0.5)
    {
		float alpha1;
		float r = length(textureCoord*u_resolution - iMouse.xy)/maxSize;
        //float expo= clamp(exp((soft)*r*r),1.0,2.0);
		//r = (abs(sin(r+iGlobalTime)))*100.0;
		r = r*(100.0/radius)*(1.0+0.3*sin(1.570795*u_global_time/(Duration+0.001)));
		if(clicked>0.5)
		{
			if(r<4.0*3.1415926)
			{
				float expo=  clamp( (-sin(r)/r)* 6.0,0.0,1.0) ;
				alpha1 = clamp( expo*(0.2+Opacity),0.0,1.0);
				gl_FragColor = vec4(mix(img.rgb, rgb,alpha1),1.0);
			}
			else
			{
				gl_FragColor = img;
			}
		}
		else
		{
			gl_FragColor = img;
		}	
    }else
    {
        gl_FragColor = img;
    }
}