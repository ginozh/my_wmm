/*{
	"PARAMS":[
		{
			"NAME":"length",
			"TYPE":"int",
			"DEFAULT":10,
			"MIN":1,
			"MAX":20
		}
	] 
}*/

vec2 barrelDistortion(vec2 coord, float amt) {
	vec2 cc = coord - 0.5;
	float dist = dot(cc, cc);
	//return coord + cc * (dist*dist)  * amt;
	return coord + cc * dist * amt;

}

vec4 FUNCNAME(vec2 tc) 
{
	//vec2 uv=(fragCoord.xy/u_resolution.xy);

	vec2 uv=(tc*.5)+.25;
    
    vec4 tx = vec4(0.0);
    for(int i = 0; i < PREFIX(length); i++)
    {
        tx += INPUT(barrelDistortion(uv,0.1*float(i)));
    }
	tx /= float(PREFIX(length));
	return vec4(tx.rgb,1.);
}
