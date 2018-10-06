#ifdef GL_ES
precision highp float;
#endif

// General parameters
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

vec2 resolution = u_resolution;
void main() 
{
    float y = float(int(gl_FragCoord.y));
    if(mod(y, 2.0) == 0.0)
    //if(gl_FragCoord.y >= 512.0)
    {
        //gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        gl_FragColor  = texture2D(mSample0, textureCoord);
    }
    else
    {
        gl_FragColor  = texture2D(mSample1, textureCoord);
    }
}
