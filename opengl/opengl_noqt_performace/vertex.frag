attribute vec2 position;
attribute vec2 a_texCoord;

uniform vec2 u_resolution;
varying vec2 texCoord;
varying vec2 textureCoord;
void main() {
    // gl_Position = vec4(position, 0, 1);
    // texCoord = position;
    // textureCoord = position* 0.5 + 0.5;

    // 从像素坐标转换到 0.0 到 1.0
    // vec2 zeroToOne = position / vec2(1280,720); // u_resolution;
    vec2 zeroToOne = position / u_resolution;
    vec2 zeroToOneC = a_texCoord / u_resolution;

    // 再把 0->1 转换 0->2
    vec2 zeroToTwo = zeroToOne * 2.0;
    vec2 zeroToTwoC = zeroToOneC * 2.0;

    // 把 0->2 转换到 -1->+1 (裁剪空间)
    vec2 clipSpace = zeroToTwo - 1.0;
    vec2 clipSpaceC = zeroToTwoC - 1.0;

    // gl_Position = vec4(clipSpace, 0, 1);
    // lefttop to leftbottom
    vec2 leftTopC = clipSpaceC * vec2(1, -1);
    gl_Position = vec4(clipSpace * vec2(1, -1), 0, 1);

    //(-1~-1) to uv(0~1) leftbottom
    texCoord = gl_Position.xy;
    //textureCoord = gl_Position.xy * 0.5 + 0.5 ;
    textureCoord = leftTopC * 0.5 + 0.5;
    //textureCoord = a_texCoord * 0.5 + 0.5;
    
}
