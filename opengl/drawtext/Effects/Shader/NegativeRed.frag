/*{
	"PARAMS":[
		{
			"NAME":"strength",
			"TYPE":"float",
			"DEFAULT":0.8,
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

void main()
{
	vec2 tc = textureCoord;
	
	// Grayscale conversion using NTSC conversion weights
	vec4 color = texture2D(mSample0, tc);
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));

    float r,g,b, c1, c2;
	c1 = 0.5 + 0.5 * u_strength;
	c2 = 0.5 - 0.5 * u_strength;
    if (gray <= 0.5)
    {
		g = c2 * gray * 2.0;
		b = g;
		r = c1 * gray * 2.0;
    }
    else
    {
        g = c2 + (1.0 - c2) * (gray - 0.5) / 0.5;
		b = g;
		r = c1 + (1.0 - c1) * (gray - 0.5) / 0.5;
    }
    
    gl_FragColor = vec4(r,g,b,color.a);
}
