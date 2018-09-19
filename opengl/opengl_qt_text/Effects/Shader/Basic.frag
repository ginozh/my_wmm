varying vec2 textureCoord;

uniform sampler2D mSample0;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;
uniform float u_first;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;
void main() 
{
    vec4 color;
    float alpa;

    color = texture2D(mSample0, textureCoord);
    //alpa = texture2D(mSample0, textureCoord).a;
    alpa = color.a;
    if(alpa==0.0)
    {
        //color=vec4(255.0, 255.0, 255.0, 0.0);
        color=vec4(0.0, 0.0, 0.0, 0.0);
    }

    gl_FragColor = color;
}
