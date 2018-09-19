varying vec2 textureCoord;

uniform sampler2D mSample0;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;
uniform float u_first;

uniform float u_posX;
uniform float u_posY;
uniform float u_cropWidth;
uniform float u_cropHeight;
uniform float u_oriWidth;
uniform float u_oriHeight;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;
/*
float u_posX=512.0;
float u_posY=100.0;
float u_cropWidth=512.0;
float u_cropHeight=386.0;
float u_oriWidth=1280.0;
float u_oriHeight=720.0;
*/
void main() 
{
    if(u_oriWidth>0.0 && u_oriHeight>0.0)
    {
        float ToI = gl_FragCoord.x + u_posX;
        float ToJ = gl_FragCoord.y + u_oriHeight - u_cropHeight - u_posY;
        if (u_first==1.0)
            gl_FragColor = texture2D(mSample0, vec2(ToI / u_oriWidth, 1.0 - ToJ / u_oriHeight)); //no use textureCoord
        else
            gl_FragColor = texture2D(mSample0, vec2(ToI / u_oriWidth, ToJ / u_oriHeight));
    }
    else
    {
        //same with basic
        vec2 uv=textureCoord;
        gl_FragColor = texture2D(mSample0, uv);
    }
    
}
