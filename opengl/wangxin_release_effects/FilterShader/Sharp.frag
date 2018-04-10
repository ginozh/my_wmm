/*{
	"GUID":"BAC37FE1-0A77-4dca-9C45-2FDFE66F3D46"
}*/

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 PREFIX(offset)[9];

//This sketch algorithm is our own inverted method of sketch
//Sketch
vec4 FUNCNAME(vec2 tc) 
{
  vec4 tempColor;
    
    vec4 sample[9];
    for(int i=0; i < 9; i++)
    {
        sample[i] = INPUT(tc + PREFIX(offset)[i]);
    }
    tempColor = -sample[0] - sample[1] - sample[2] - sample[3] + sample[4] * 9.0 - sample[5] - sample[6] - sample[7] - sample[8];    
    tempColor.rgb = 0.627* exp(tempColor.rgb * 0.3634)  - 0.5467 * exp(-tempColor.rgb * 10.891);
    return tempColor;
}
