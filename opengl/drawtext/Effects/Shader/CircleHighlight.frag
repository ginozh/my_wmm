/********************************************************************
author: RuanShengQiang
date: 2017/3/21
********************************************************************/
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

uniform float Radius;
uniform vec3 Color;
uniform float Opacity;
uniform float Softness;
uniform float Enable;//if enable > 0.5, the effect is enabled
uniform vec2 iMouse;

void main()
{
	
    float radius = Radius/4.0;
    vec3 rgb = Color;
    float soft = Softness*3.0;
    vec4 img = texture2D(mSample0,textureCoord);
	    if(Enable>0.5)
    {
		float alpha = clamp(Opacity,0.0,1.0);
		float r = length(textureCoord*u_resolution - iMouse.xy)/u_resolution.x;
		if(r<radius)
		{
			float expo= clamp(exp(-(soft*soft)*r*r/radius/radius)+Softness*0.1,0.0,1.0);
			alpha = clamp(alpha * expo ,0.0,1.0);
			gl_FragColor = vec4(mix(img.rgb,rgb,alpha),1.0);
		}else
		{
			gl_FragColor = img;
		}
	}else
	{
			gl_FragColor = img;
	}
}