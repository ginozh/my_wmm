
float normpdf(in float x, in float sigma)
{
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}
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

const int samples = 6;
const float fSamples = float( samples * samples * 2 * 2 );
float aspect = u_resolution.x / u_resolution.y;
const float blurscale = 1.0 / 24.;
float iGlobalTime = PREFIX(global_time);
vec4 FUNCNAME(vec2 tc) 
{
	vec2 uv = tc;
    
    float sampleR = 0.0;
    float sampleG = 0.0;
    float sampleB = 0.0;
    for( int x = -samples; x < samples; x++ )
    {
    	for( int y = -samples; y < samples; y++ )
        {
            vec2 subSampleOffset = ( vec2( float( x ), float( y ) ) / float( samples ) ) 
                / ( u_resolution.xy * blurscale );
            
            float fx = float( x ); float fy = float( y );
            float gaussianLength = sqrt( fx*fx + fy*fy ) / sqrt( fSamples * 0.5 );
            float sampleWeight = GaussianWeight( gaussianLength, 0.0, 0.3 );
            
			vec3 c = INPUT(uv + subSampleOffset ).rgb;
            float subsample = pow( c.r, 2.2 );
    		sampleR += saturate( subsample ) * sampleWeight;
            subsample = pow( c.g, 2.2 );
    		sampleG += saturate( subsample ) * sampleWeight;
            subsample = pow( c.b, 2.2 );
    		sampleB += saturate( subsample ) * sampleWeight;
        }
    }
	float lowVal = 50./255.;
	float highVal = 200./255.;
	float rangeVal = highVal - lowVal;
    float pixelGrey = dot(INPUT(tc).rgb, vec3(0.2126, 0.7152, 0.0722));
	pixelGrey = clamp(pixelGrey, lowVal, highVal);
	pixelGrey = (pixelGrey - lowVal) / rangeVal;
	
    vec3 color = vec3(sampleR, sampleG, sampleB);
    color /= fSamples;
    color *= 1.7;
	color.r = pow( color.r, 1.0 / 2.2 );
	color.g = pow( color.g, 1.0 / 2.2 );
	color.b = pow( color.b, 1.0 / 2.2 );
	color = (color + pixelGrey) / 2.0;

    
	return vec4( color , 1.0 );
}
