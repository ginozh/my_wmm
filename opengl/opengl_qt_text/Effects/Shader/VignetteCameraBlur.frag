/*{
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
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_strength;

#ifdef GL_ES
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
float aspect = u_resolution.x / u_resolution.y;

vec3 tex2DBlur( vec2 uv, float blurscale )
{
   
    vec3 sample;
    for( int x = -samples; x < samples; x++ )
    {
    	for( int y = -samples; y < samples; y++ )
        {
            vec2 subSampleOffset = ( vec2( float( x ), float( y ) ) / float( samples ) ) 
                / ( u_resolution.xy * blurscale );
            
            float fx = float( x ); float fy = float( y );
            float gaussianLength = sqrt( fx*fx + fy*fy ) / sqrt( fSamples * 0.5 );
            float sampleWeight = GaussianWeight( gaussianLength, 0.0, 0.3 );
            
            vec3 col = texture2D(mSample0, uv + subSampleOffset ).rgb;
    		sample += saturate( col ) * sampleWeight;
        }
    }
    sample /= fSamples;
    sample *= 3.7;

	return sample;//vec3( pow( sample.r, 1.0 / 2.2 ),pow( sample.g, 1.0 / 2.2 ),pow( sample.b, 1.0 / 2.2 ) );
}
#else
vec3 texture(vec2 uv) {
    return texture2D(mSample0, vec2(uv.x,uv.y)).rgb;
    //return texture2D(iChannel0,uv).rgb;
}

float grid(float var, float size) {
    return floor(var*size)/size;
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 texBlur(vec2 uv, float bluramount)
{
    //float dists = 5.;
    vec3 blurred_image = vec3(0.);
    float repeats = 100.;
    for (float i = 0.; i < 100.; i++) {
        //Older:
        //vec2 q = vec2(cos(degrees((grid(i,dists)/repeats)*360.)),sin(degrees((grid(i,dists)/repeats)*360.))) * (1./(1.+mod(i,dists)));
        vec2 q = vec2(cos(degrees((i/repeats)*360.)),sin(degrees((i/repeats)*360.))) *  (rand(vec2(i,uv.x+uv.y))+bluramount);
        vec2 uv2 = uv+(q*bluramount);
        blurred_image += texture(uv2)/2.;
        //One more to hide the noise.
        q = vec2(cos(degrees((i/repeats)*360.)),sin(degrees((i/repeats)*360.))) *  (rand(vec2(i+2.,uv.x+uv.y+24.))+bluramount);
        uv2 = uv+(q*bluramount);
        blurred_image += texture(uv2)/2.;
    }
    blurred_image /= repeats;
    return blurred_image;
}
#endif

void main()
{
	vec2 tc = textureCoord;
    float ratio = 1.0 - min(1.0, texture2D(mSample1, tc).r + 0.5 * (1.0-u_strength));
#ifdef GL_ES
	gl_FragColor = vec4(tex2DBlur(tc, 1.0 / (ratio * 15.0)), 1.0);
#else
    gl_FragColor = vec4(texBlur(tc, ratio * 0.03), 1.0);
#endif
}
