/*{
	"GUID":"51237539-8F37-4593-BBAF-9FF0A6307C09",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"blurOverlay.jpg"
		}
	],
	"PARAMS":[
		{
			"NAME":"strength",
			"TYPE":"float",
			"DEFAULT":1.0,
			"MIN":0.0,
			"MAX":1.0
		}
	] 
}*/

float distance_x = 0.01;
const float distance_y = 0.00;
const vec3 desaturate = vec3(.3, .59, .11);

#define saturate(i) ( clamp( i, 0.0, 1.0 ) )
const float pi = 3.14159265;

float GaussianWeight( float x, float mean, float deviation )
{
	float c = deviation;
	float a = c * sqrt( 2.0 * pi );
	float b = x - mean;
	float v = a * exp( -( ( b * b ) / ( 2.0*c*c ) ) );
	return v / a;
}

const int samples = 7;
const float fSamples = float( samples * samples * 2 * 2 );
float aspect = iResolution.x / iResolution.y;

vec3 tex2DBlur( vec2 uv, float blurscale )
{
   
    vec3 sample;
    for( int x = -samples; x < samples; x++ )
    {
    	for( int y = -samples; y < samples; y++ )
        {
            vec2 subSampleOffset = ( vec2( float( x ), float( y ) ) / float( samples ) ) 
                / ( iResolution.xy * blurscale );
            
            float fx = float( x ); float fy = float( y );
            float gaussianLength = sqrt( fx*fx + fy*fy ) / sqrt( fSamples * 0.5 );
            float sampleWeight = GaussianWeight( gaussianLength, 0.0, 0.3 );
            
            vec3 col = INPUT1(uv + subSampleOffset ).rgb;
    		sample += saturate( col ) * sampleWeight;
        }
    }
    sample /= fSamples;
    sample *= 3.7;

	return sample;//vec3( pow( sample.r, 1.0 / 2.2 ),pow( sample.g, 1.0 / 2.2 ),pow( sample.b, 1.0 / 2.2 ) );
}


vec4 FUNCNAME(vec2 tc)
{
	float ratio = 1.0 - min(1.0, INPUT2(tc).r + 0.5 * (1.0-PREFIX(strength)));
	
	return vec4(tex2DBlur(tc, 1.0 / (ratio * 15.0)), 1.0);
}
