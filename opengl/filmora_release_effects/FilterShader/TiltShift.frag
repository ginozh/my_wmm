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

#define PI 3.14159265359

float iPosX = PREFIX(pos_x);
float iPosY = PREFIX(pos_y);
float iDistance = PREFIX(distance) * iResolution.x / iResolution.y;
float iAngle = PREFIX(angle);

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

vec4 FUNCNAME(vec2 tc)
{
    vec2 r = -vec2(gl_FragCoord.xy - 0.5*iResolution.xy)/iResolution.y;
	vec3 c = INPUT(tc).rgb;
	{

		const int mSize = 11;
		const int kSize = (mSize-1)/2;
		float kernel[mSize];
		vec3 final_colour = vec3(0.0);
        
        r-= vec2(0.5 * iResolution.x / iResolution.y, 0.5);
        
        r+= vec2(iPosX * iResolution.x / iResolution.y, iPosY);
        
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
				final_colour += kernel[kSize+j]*kernel[kSize+i]*INPUT((gl_FragCoord.xy+vec2(float(i),float(j))) / iResolution.xy).rgb;
	
			}
		}
		
		
		return vec4(final_colour/(Z*Z), INPUT(tc).a);
	}
}