//description: denoise. roll diffuseValue[0-100] to control the strength of diffuse. 
//author: Ruan ShengQiang 
//date: 2017/2/22

#define PI 3.1415926
uniform sampler2D mSample0;
uniform sampler2D mSample1;
uniform vec2 u_resolution;
uniform float u_global_time;
varying vec2 textureCoord;

// diffuseValue is the input, range[0:100]
uniform float diffuseValue;

float offset0 = 0.0;
float offset1 = 1.3846153846;
float offset2 = 3.2307692308;

float weight0 = 0.2270270270;
float weight1= 0.3162162162;
float weight2 = 0.0702702703;

float angle = PI / 2.0 - (diffuseValue * PI) / 1440.0;

void main(void)
{
	//linear gaussian, only the approximation
	if (diffuseValue>0.0f)
	{
		gl_FragColor = texture2D( mSample1, textureCoord ) * weight0;
			gl_FragColor = gl_FragColor\
			+ texture2D( mSample1, textureCoord+vec2(0.0, offset1/u_resolution.x))* weight1\
			+ texture2D( mSample1, textureCoord+vec2(0.0, offset2/u_resolution.x))* weight2;
			
			gl_FragColor = gl_FragColor\
			+ texture2D( mSample1, textureCoord-vec2(0.0, offset1/u_resolution.x))* weight1\
			+ texture2D( mSample1, textureCoord-vec2(0.0, offset2/u_resolution.x))* weight2;
	}

	float density = abs(cos(angle));
	vec4 orig = texture2D(mSample1, textureCoord);
	gl_FragColor = vec4(clamp(gl_FragColor.xyz * vec3(1.0+density),0.0,1.0),orig.w);

}
