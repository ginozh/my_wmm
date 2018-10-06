#ifdef GL_ES
precision highp float;
#endif

// General parameters
uniform sampler2D mSample0;
uniform sampler2D mSample1;
uniform sampler2D mSample2;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;

const float alphaDist = 0.125;

void main() 
{
	vec2 tc = textureCoord;
	float thresholdVal = (0.98 + (2.0 * alphaDist)) * progress;
	float alphaMux = thresholdVal - alphaDist;
	float alphaAdd = thresholdVal + alphaDist;
	
	float curAlphaData = texture2D(mSample2, tc).r;
	if (curAlphaData < alphaMux)
	{
		gl_FragColor = texture2D(mSample1, tc);
	}
	else if (curAlphaData >= alphaAdd)
	{
		gl_FragColor = texture2D(mSample0, tc);
	}
	else //if (curAlphaData >= alphaMux && curAlphaData < alphaAdd)
	{
		float mx = (curAlphaData - alphaMux) / (alphaAdd - alphaMux);
		gl_FragColor = mix(texture2D(mSample1, tc),texture2D(mSample0, tc),mx);
	}
}