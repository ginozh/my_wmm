/*{
	"GUID":"51D37319-8F37-4593-BBAF-9FF0A6307C01",
	"PARAMS":[
		{
			"NAME":"transparent",
			"TYPE":"float",
			"DEFAULT":0.5,
			"MIN":0.0,
			"MAX":1.0
		},
		{
			"NAME":"height",
			"TYPE":"float",
			"DEFAULT":0.99,
			"MIN":0.0,
			"MAX":1.0
		},
		{
			"NAME":"speed",
			"TYPE":"float",
			"DEFAULT":0.18,
			"MIN":0.0,
			"MAX":1.0
		},
		{
			"NAME":"count",
			"TYPE":"int",
			"DEFAULT":30,
			"MIN":1,
			"MAX":100
		}
	]
}*/

float time = PREFIX(global_time);
vec2 resolution = iResolution.xy;

const float fRadius = 0.08;
float ySpeed = PREFIX(speed) * 3.0;
float bubblePopHeight = 3.0-PREFIX(height)*2.0;
const float bubbleSpreadDist = 2.0;
int count = PREFIX(count) / 2;
vec4 FUNCNAME(vec2 tc) 
{
    vec2 uv = -1.0 + 2.0*gl_FragCoord.xy / resolution.xy;
    uv.x *=  resolution.x / resolution.y;
    
    vec3 color = INPUT(tc).rgb;

        // bubbles
    for( int i=0; i<count; i++ )
    {
            // bubble seeds
        float pha = tan(float(i)*6.+1.0)*0.5 + 0.5;
        float siz = pow( cos(float(i)*2.4+4.0) * 0.5 + ySpeed, 4.0 );
        float pox = cos(float(i)*2.55+3.1) * resolution.x / resolution.y;
        
            // buble size, position and color
        float rad = fRadius + sin(float(i))*0.12+0.29;
        vec2  pos = vec2( pox+sin(time/50.+pha+siz), -abs(bubblePopHeight)-rad + (bubbleSpreadDist+2.0*rad)
                         *mod(pha+0.1*(time/5.)*(0.2+0.8*siz),1.0)) * vec2(1.0, 1.0);
        float dis = length( uv - pos );
        vec3  col = mix( vec3(0.9, 0.2, 0.0), vec3(0.0,0.3,0.6), 0.5+0.5*sin(float(i)*sin(time*pox*0.03)+1.9));
        
            // render
        color += col.xyz *(1.- smoothstep( rad*(0.65+0.20*sin(pox*time)), rad, dis )) * (1.0 - cos(pox*time)) * PREFIX(fade) * PREFIX(transparent);
    }

    return vec4(color,INPUT(tc).a);
}

