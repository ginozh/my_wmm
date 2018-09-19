uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

void main()
{
	vec2 tc = textureCoord;
	
	// Grayscale conversion using NTSC conversion weights
	vec4 color = texture2D(mSample0, tc);
    color.rgb = vec3(1.0,1.0,1.0)-color.rgb;
    
    gl_FragColor = color;
}
