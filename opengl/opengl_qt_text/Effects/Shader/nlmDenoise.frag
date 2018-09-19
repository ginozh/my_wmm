//description: denoise. roll smoothStren[0-100] to control the strength of denoising. 
//author: Ruan ShengQiang 
//date: 2017/2/22

uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

uniform float smoothStren; //range[0:100]

float sigma_spatial=0.01;
float sigma_color=0.1;
float filter_window=0.1;

float random(vec3 scale, float seed) {
	return fract(sin(dot(gl_FragCoord.xyz + seed, scale)) * 43758.5453 + seed);
}

vec3 Filter(vec2 uv, float sigma_spatial,float sigma_color)
{
	float wsize = filter_window/14.0;
    vec3 res_color = vec3(0.0,0.0,0.0);
    float res_weight = 0.0;
    vec3 center_color=texture2D(mSample0,uv).rgb;
    float sigma_i=0.5*wsize*wsize/sigma_spatial/sigma_spatial;
    float offset = random(vec3(12.9898, 78.233, 151.7182), 0.0);
    float offset2 = random(vec3(112.9898, 178.233, 51.7182), 0.0);

    for (float i = -1.0; i <= 1.0; i++) {
        for (float j= -1.0; j<= 1.0; j++) {
            vec2 uv_sample = uv+vec2(float(i+offset-0.5)*wsize,float(j+offset-0.5)*wsize);
        	vec3 tmp_color = texture2D(mSample0,uv_sample).rgb;   
            vec3 diff_color = (tmp_color-center_color);
            float tmp_weight = exp(-(i*i+j*j)*sigma_i);
            tmp_weight *= exp(-(dot(diff_color,diff_color)/2.0/sigma_color/sigma_color));
            res_color += tmp_color * tmp_weight;
            res_weight += tmp_weight;   
        }
    }
	
    vec3 res = res_color/res_weight;
    return res;
}

void main() 
{
	vec4 orig = texture2D(mSample0, textureCoord);
    vec3 result = Filter(textureCoord*vec2(1.0,1.0),sigma_spatial,sigma_color);    
    vec3 base = texture2D(mSample0,textureCoord*vec2(1.0,1.0)).rgb;   
	result = mix(base,result, smoothStren/100.0);
	gl_FragColor = vec4(result,1.0);
}
