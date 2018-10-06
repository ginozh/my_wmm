/*{
	"GUID":"54D37319-8F37-4593-BBAF-9FF0A6307C01",
	"PARAMS":[
		{
			"NAME":"speed",
			"TYPE":"float",
			"DEFAULT":0.2,
			"MIN":0.1,
			"MAX":1.0
		},
		{
			"NAME":"distance",
			"TYPE":"float",
			"DEFAULT":0.5,
			"MIN":0.0,
			"MAX":1.0
		}
	]
}*/

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_strength;
uniform float u_speed;
uniform float u_distance;

float iGlobalTime = u_global_time;
float speed = u_speed*5.0;

// This shader useds noise shaders by stegu -- http://webstaff.itn.liu.se/~stegu/
// This is supposed to look like snow falling, for example like http://24.media.tumblr.com/tumblr_mdhvqrK2EJ1rcru73o1_500.gif

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 mod289(vec3 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

vec4 permute(vec4 x) {
  return mod((34.0 * x + 1.0) * x, 289.0);
}

vec4 taylorInvSqrt(vec4 r)
{
	return 1.79284291400159 - 0.85373472095314 * r;
}

float cellular2x2(vec2 P)
{
		#define K 0.142857142857 // 1/7
		#define K2 0.0714285714285 // K/2
		#define jitter 0.8 // jitter 1.0 makes F1 wrong more often
		
		vec2 Pi = mod(floor(P), 289.0);
		vec2 Pf = fract(P);
		vec4 Pfx = Pf.x + vec4(-0.5, -1.5, -0.5, -1.5);
		vec4 Pfy = Pf.y + vec4(-0.5, -0.5, -1.5, -1.5);
		vec4 p = permute(Pi.x + vec4(0.0, 1.0, 0.0, 1.0));
		p = permute(p + Pi.y + vec4(0.0, 0.0, 1.0, 1.0));
		vec4 ox = mod(p, 7.0)*K+K2;
		vec4 oy = mod(floor(p*K),7.0)*K+K2;
		vec4 dx = Pfx + jitter*ox;
		vec4 dy = Pfy + jitter*oy;
		vec4 d = dx * dx + dy * dy; // d11, d12, d21 and d22, squared
		// Sort out the two smallest distances
		
		// Cheat and pick only F1
		d.xy = min(d.xy, d.zw);
		d.x = min(d.x, d.y);
		return d.x; // F1 duplicated, F2 not computed
}

void main()
{
	vec2 tc = textureCoord;
		vec2 uv = gl_FragCoord.xy / u_resolution.xy;
		
		uv.x*=(u_resolution.x/u_resolution.y);

		vec2 GA;
		GA.x-=iGlobalTime*0.8;
		GA.y+=iGlobalTime*0.9;
		GA*=speed;
	
		float F1=0.0,F2=0.0,F3=0.0,F4=0.0,F5=0.0,N1=0.0,N2=0.0,N3=0.0,N4=0.0,N5=0.0;
		float A=0.0,A1=0.0,A2=0.0,A3=0.0,A4=0.0,A5=0.0;


		// Attentuation
		A = (uv.x-(uv.y*0.3));
		A = clamp(A,0.0,u_distance);

		// Snow layers, somewhat like an fbm with worley layers.
		F1 = 1.0-cellular2x2((uv+(GA*0.1))*8.0);	
		A1 = 1.0-(A*1.0);
		N1 = smoothstep(0.998,1.0,F1)*1.0*A1;	

		F2 = 1.0-cellular2x2((uv+(GA*0.2))*6.0);	
		A2 = 1.0-(A*0.8);
		N2 = smoothstep(0.995,1.0,F2)*0.85*A2;				

		F3 = 1.0-cellular2x2((uv+(GA*0.4))*4.0);	
		A3 = 1.0-(A*0.6);
		N3 = smoothstep(0.99,1.0,F3)*0.65*A3;				

		F4 = 1.0-cellular2x2((uv+(GA*0.6))*3.0);	
		A4 = 1.0-(A*1.0);
		N4 = smoothstep(0.98,1.0,F4)*0.4*A4;				

		F5 = 1.0-cellular2x2((uv+(GA))*1.2);	
		A5 = 1.0-(A*1.0);
		N5 = smoothstep(0.98,1.0,F5)*0.45*A5;				
						
		float Snowout=N5+N4+N3+N2+N1;
		
		float alpha = Snowout;
						
		Snowout = N1+N2+N3+N4+N5 + 0.55;
		
		gl_FragColor = mix(texture2D(mSample0, tc), vec4(Snowout*0.9, Snowout, Snowout*1.1, 1.0), alpha);

}