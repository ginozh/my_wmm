/*{
	"GUID":"52D37319-8F37-4593-BBAF-9FF0A6307C01",
	"PARAMS":[
		{
			"NAME":"transparent",
			"TYPE":"float",
			"DEFAULT":0.2,
			"MIN":0.0,
			"MAX":1.0
		},
		{
			"NAME":"size",
			"TYPE":"float",
			"DEFAULT":0.23,
			"MIN":0.0,
			"MAX":1.0
		},
		{
			"NAME":"count",
			"TYPE":"int",
			"DEFAULT":24,
			"MIN":1,
			"MAX":100
		}
	]
}*/

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_transparent;
uniform float u_size;
uniform int u_count;

float time = u_global_time;
vec2 resolution = u_resolution.xy;

float fRadius = 0.04 + u_size;
int count = u_count*2;

void main()
{
	vec2 tc = textureCoord;
    vec2 uv = -1.0 + 2.0*gl_FragCoord.xy / resolution.xy;
    uv.x *=  resolution.x / resolution.y;
    
	vec4 orig = texture2D(mSample0, tc);
    vec3 color = orig.rgb;

        // bubbles
    for( int i=0; i<count; i++ )
    {
            // bubble seeds
        float pha = tan(float(i)*6.+1.0)*0.5 + 0.5;
        float siz = pow( cos(float(i)*2.4+5.0)*0.5 + 0.5, 4.0 );
        float pox = cos(float(i)*3.55+4.1) * resolution.x / resolution.y;
        
            // buble size, position and color
        float rad = fRadius + sin(float(i))*0.12+0.28;
        vec2  pos = vec2( pox+sin(time/30.+pha+siz), -1.0-rad + (2.0+2.0*rad)
                         *mod(pha+0.1*(time/5.)*(0.2+0.8*siz),1.0)) * vec2(1.0, 1.0);
        float dis = length( uv - pos );
        vec3  col = mix( vec3(0.7, 0.2, 0.8), vec3(0.2,0.8,0.6), 0.5+0.5*sin(float(i)*sin(time*pox*0.03)+1.9));
        
            // render
        color += col.xyz *(1.- smoothstep( rad*(0.65+0.20*sin(pox*time)), rad, dis )) * (1.0 - cos(pox*time)) * u_transparent;
    }

	gl_FragColor = vec4(color,orig.a);
}
