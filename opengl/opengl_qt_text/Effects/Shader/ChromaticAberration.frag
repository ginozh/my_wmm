/*{
	"GUID":"50D37319-8F37-4593-BBAF-9FF0A6307C01"
}*/

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

float iGlobalTime = u_global_time;

void main()
{
	vec2 tc = textureCoord;
    vec2 uv = tc;

	float amount = 0.0;
	
	amount = (1.0 + sin(iGlobalTime*6.0)) * 0.5;
	amount *= 1.0 + sin(iGlobalTime*16.0) * 0.5;
	amount *= 1.0 + sin(iGlobalTime*19.0) * 0.5;
	amount *= 1.0 + sin(iGlobalTime*27.0) * 0.5;
	amount = pow(amount, 3.0);

	amount *= 0.05;
	
	//amount *= PREFIX(fade);
	
    vec3 col;
    col.r = texture2D(mSample0, vec2(uv.x+amount,uv.y) ).r;
    col.g = texture2D(mSample0, uv ).g;
    col.b = texture2D(mSample0, vec2(uv.x-amount,uv.y) ).b;

	col *= (1.0 - amount * 0.5);
	
    gl_FragColor = vec4(col,texture2D(mSample0, tc).a);
}
