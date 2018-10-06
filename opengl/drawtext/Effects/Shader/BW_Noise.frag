/*{
	"GUID":"3E3535CE-7881-4e9e-A1EE-D5220E323EB7"
}*/
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

float normpdf(in float x, in float sigma)
{
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

float hash( float n )
{
	return fract( sin(n)*54671.57391);
}

float noise( vec2 p )
{
	return hash( p.x + p.y * 57.1235 );
}
vec2 correctCoord(vec2 orip, vec2 tc){
    vec2 p = orip;
    if(p.x<0.0) p.x=0.0;
    if(p.y<0.0) p.y=0.0;
    if(p.x>tc.x) p.x=tc.x;
    if(p.y>tc.y) p.y=tc.y;
    return p;
}

void main()
{
	vec2 tc = textureCoord;
	vec3 c = texture2D(mSample0, tc).rgb;
		
		//declare stuff
		const int mSize = 5;
		const int kSize = (mSize-1)/2;
		float kernel[mSize];
		vec3 final_colour = vec3(0.0);
        vec3 col;
        vec2 offset;
        float ns;
		
		//create the 1-D kernel
		float sigma = 7.0;
		float Z = 0.0;
		for (int j = 0; j <= kSize; ++j)
		{
			kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), sigma);
		}
		
		//get the normalization factor (as the gaussian has been clamped)
		for (int j = 0; j < mSize; ++j)
		{
			Z += kernel[j];
		}
		
		//read out the texels
		for (int i=-kSize; i <= kSize; ++i)
		{
			for (int j=-kSize; j <= kSize; ++j)
			{
                offset = (gl_FragCoord.xy+vec2(float(i),float(j))) / u_resolution.xy;
                offset=correctCoord(offset, tc);
				col = texture2D(mSample0, offset).rgb;
                ns = noise( offset*(u_global_time+100.) ) * 0.28;
    			col = col * 0.78 + vec3(ns,ns,ns);
				final_colour += kernel[kSize+j]*kernel[kSize+i]*col.bbb;
			}
		}

		gl_FragColor = vec4(final_colour/(Z*Z), 1.0);
}
