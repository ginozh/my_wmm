/*{
	"GUID":"647304F8-B2DC-4200-B8B2-E8FF95020A44"
}*/

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 PREFIX(offset)[9];

//This sketch algorithm is our own inverted method of sketch
//Sketch
vec4 FUNCNAME(vec2 tc) 
{
  vec4 tempColor = INPUT(tc);
    vec4 sample[9];
    for(int i=0; i < 9; i++)
    {
        sample[i] = INPUT(tc + PREFIX(offset)[i]);
    }    
    vec4 result = sqrt(    (-sample[0] - sample[1] * 2.0 - sample[2] + sample[6] + sample[7] * 2.0 + sample[8])
                         * (-sample[0] - sample[1] * 2.0 - sample[2] + sample[6] + sample[7] * 2.0 + sample[8]) 
                         + (-sample[0] - sample[3] * 2.0 - sample[6] + sample[2] + sample[5] * 2.0 + sample[8])
                         * (-sample[0] - sample[3] * 2.0 - sample[6] + sample[2] + sample[5] * 2.0 + sample[8]));
    float gray = 1.0 - dot(result.rgb, vec3(0.114, 0.587, 0.299));
    return gray * 2.0 * tempColor;
}
