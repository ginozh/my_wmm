/*{
	"GUID":"BAC37FE1-0A77-4dca-9C45-2FDFE66F3D46"
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
  vec4 tempColor;
    
    vec4 sample[9];
    for(int i=0; i < 9; i++)
    {
        sample[i] = texture2D(mSample0, tc + u_offset[i]);
    }
    tempColor = -sample[0] - sample[1] - sample[2] - sample[3] + sample[4] * 9.0 - sample[5] - sample[6] - sample[7] - sample[8];    
    tempColor.rgb = 0.627* exp(tempColor.rgb * 0.3634)  - 0.5467 * exp(-tempColor.rgb * 10.891);
    gl_FragColor = tempColor;
}
