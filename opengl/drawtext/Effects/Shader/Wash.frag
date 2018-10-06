/*{
	"GUID":"647304F8-B2DC-4200-B8B2-E8FF95020A44"
}*/

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

uniform vec2 u_offset[9];

//This sketch algorithm is our own inverted method of sketch
//Sketch
void main()
{
	vec2 tc = textureCoord;
  vec4 tempColor = texture2D(mSample0, tc);
    vec4 sample[9];
    for(int i=0; i < 9; i++)
    {
        sample[i] = texture2D(mSample0, tc + u_offset[i]);
    }    
    vec4 result = sqrt(    (-sample[0] - sample[1] * 2.0 - sample[2] + sample[6] + sample[7] * 2.0 + sample[8])
                         * (-sample[0] - sample[1] * 2.0 - sample[2] + sample[6] + sample[7] * 2.0 + sample[8]) 
                         + (-sample[0] - sample[3] * 2.0 - sample[6] + sample[2] + sample[5] * 2.0 + sample[8])
                         * (-sample[0] - sample[3] * 2.0 - sample[6] + sample[2] + sample[5] * 2.0 + sample[8]));
    float gray = 1.0 - dot(result.rgb, vec3(0.114, 0.587, 0.299));
    gl_FragColor = gray * 2.0 * tempColor;
	gl_FragColor.a = tempColor.a;
}
