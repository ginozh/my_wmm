attribute vec2 position;
attribute vec2 a_texCoord;

uniform vec2 u_resolution;
uniform float u_first;
varying vec2 texCoord;
varying vec2 textureCoord;
void main(void) {
    //new 
    // 0~1280, 0~720 -> 0.0~1.0
    vec2 zeroToOne = position / u_resolution; // vec2(1280,720);
    //vec2 zeroToOneC = a_texCoord / u_resolution;

    // 0.0~1.0 -> 0.0~2.0
    vec2 zeroToTwo = zeroToOne * 2.0;
    //vec2 zeroToTwoC = zeroToOneC * 2.0;

    // 0.0~2.0 -> -1.0~1.0 
    vec2 clipSpace = zeroToTwo - 1.0;
    //vec2 clipSpaceC = zeroToTwoC - 1.0;

    // lefttop to leftbottom
    // vec2 leftTopC = clipSpaceC * vec2(1, -1);
    gl_Position = vec4(clipSpace * vec2(1, -1), 0, 1);
    //gl_Position = vec4(clipSpace , 0, 1);


    ////gl_Position = vec4(position, 0, 1);
    //texCoord = position;
    //textureCoord = position;
    //texCoord = clipSpaceC * vec2(1, -1) * 0.5 + 0.5;
    //textureCoord = gl_Position.xy * 0.5 + 0.5;
    textureCoord = a_texCoord * 0.5 + 0.5;
    //textureCoord = clipSpaceC * 0.5 + 0.5;
    //textureCoord = clipSpaceC *  0.5 + 0.5;
    //if (u_first==1.0)
    //    textureCoord = vec2(textureCoord.x, 1.0 - textureCoord.y);
    texCoord = textureCoord;
/*
    //original
    gl_Position = vec4(position, 0, 1);
    texCoord = position* 0.5 + 0.5;
    textureCoord = position* 0.5 + 0.5;
    //textureCoord = vec2(textureCoord.s, 1.0 - textureCoord.t);
*/
    if (u_first==1.0)
    {
        textureCoord = vec2(textureCoord.x, 1.0 - textureCoord.y);
    }
}
