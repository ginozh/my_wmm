/*{
	"GUID":"50D37319-8F37-4593-BBAF-9FF0A6307C01"
}*/

float iGlobalTime = PREFIX(global_time);

vec4 FUNCNAME(vec2 tc) 
{
    vec2 uv = tc;

	float amount = 0.0;
	
	amount = (1.0 + sin(iGlobalTime*6.0)) * 0.5;
	amount *= 1.0 + sin(iGlobalTime*16.0) * 0.5;
	amount *= 1.0 + sin(iGlobalTime*19.0) * 0.5;
	amount *= 1.0 + sin(iGlobalTime*27.0) * 0.5;
	amount = pow(amount, 3.0);

	amount *= 0.05;
	
	amount *= PREFIX(fade);
	
    vec3 col;
    col.r = INPUT( vec2(uv.x+amount,uv.y) ).r;
    col.g = INPUT( uv ).g;
    col.b = INPUT( vec2(uv.x-amount,uv.y) ).b;

	col *= (1.0 - amount * 0.5);
	
    return vec4(col,INPUT(tc).a);
}
