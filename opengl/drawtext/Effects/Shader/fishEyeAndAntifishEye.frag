//description: denoise. roll strength[-100-100] to control the strength of fisheye.if strength<0, it will be antifisheye 
//author: Ruan ShengQiang 
//date: 2017/2/22

uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float strength; //range[-100,100]

void main() 
{
	vec2 p = textureCoord.xy ;
	float prop = u_resolution.x / u_resolution.y;
	vec2 m = vec2(0.5, 0.5);
	vec2 d = (p - m);
	float r = sqrt(dot(d, d)); 
	float power = ( 2.0 * 3.141592 / (2.0 * sqrt(dot(m, m))) ) *
				(0.004*strength);
	float bind;
	if (power > 0.0) bind = sqrt(dot(m, m));
	else {if (prop < 1.0) bind = m.x; else bind = m.y;}
	vec2 uv;
	if (power > 0.0)//fisheye
		uv = m + normalize(d) * tan(r * power) * bind / tan( bind * power);
	else if (power < 0.0)//antifisheye
		uv = m + normalize(d) * atan(r * -power * 10.0) * bind / atan(-power * bind * 10.0);
	else uv = p;
	gl_FragColor = texture2D(mSample0,vec2(uv.x, uv.y));
	
}
