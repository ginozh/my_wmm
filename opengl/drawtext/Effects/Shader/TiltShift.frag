/*{
	"GUID":"8BBD7340-309D-4268-933C-0C97FD49BB8C",
	"PARAMS":[
		{
			"NAME":"pos_x",
			"TYPE":"float",
			"DEFAULT":0.5,
			"MIN":0.0,
			"MAX":1.0
		},
		{
			"NAME":"pos_y",
			"TYPE":"float",
			"DEFAULT":0.5,
			"MIN":0.0,
			"MAX":1.0
		},
		{
			"NAME":"distance",
			"TYPE":"float",
			"DEFAULT":0.2,
			"MIN":0.0,
			"MAX":1.0
		},
		{
			"NAME":"angle",
			"TYPE":"float",
			"DEFAULT":0.0,
			"MIN":0.0,
			"MAX":360.0
		}
	] 
}*/

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_pos_x;
uniform float u_pos_y;
uniform float u_distance;
uniform float u_angle;

#define PI 3.14159265359

float iPosX = u_pos_x;
float iPosY = u_pos_y;
float iDistance = u_distance * u_resolution.x / u_resolution.y;
float iAngle = u_angle;

float normpdf(in float x, in float sigma)
{
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

float rectangle(vec2 r, vec2 topLeft, vec2 bottomRight) {
	float ret;
	float d = 0.05;
	ret = smoothstep(topLeft.x-d, topLeft.x+d, r.x);
	ret *= smoothstep(topLeft.y-d, topLeft.y+d, r.y);
	ret *= 1.0 - smoothstep(bottomRight.y-d, bottomRight.y+d, r.y);
	ret *= 1.0 - smoothstep(bottomRight.x-d, bottomRight.x+d, r.x);
	return ret;
}

void main()
{
	vec2 tc = textureCoord;
	vec2 fragCoord = gl_FragCoord.xy;
    vec2 r = -vec2(fragCoord.xy - 0.5*u_resolution.xy)/u_resolution.y;
	vec3 c = texture2D(mSample0, tc).rgb;
	{

		const int mSize = 11;
		const int kSize = (mSize-1)/2;
		float kernel[mSize];
		vec3 final_colour = vec3(0.0);
        
        r-= vec2(0.5 * u_resolution.x / u_resolution.y, 0.5);
        
        r+= vec2(iPosX * u_resolution.x / u_resolution.y, iPosY);
        
        vec2 q;
        float angle;
		angle = iAngle / 180. * PI; // angle in radians (PI is 180 degrees)

		q.x =   cos(angle)*r.x + sin(angle)*r.y;
		q.y = - sin(angle)*r.x + cos(angle)*r.y;

		float sigma = mix(5.1,0.1,rectangle(q, vec2(-5.5, -iDistance/2.0), vec2(5.5, iDistance/2.0)));
		float Z = 0.0;
		for (int j = 0; j <= kSize; ++j)
		{
			kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), sigma);
		}

		for (int j = 0; j < mSize; ++j)
		{
			Z += kernel[j];
		}

		for (int i=-kSize; i <= kSize; ++i)
		{
			for (int j=-kSize; j <= kSize; ++j)
			{
				final_colour += kernel[kSize+j]*kernel[kSize+i]*texture2D(mSample0, (fragCoord.xy+vec2(float(i),float(j))) / u_resolution.xy).rgb;
	
			}
		}
		
		
		gl_FragColor = vec4(final_colour/(Z*Z), texture2D(mSample0, tc).a);
	}
}