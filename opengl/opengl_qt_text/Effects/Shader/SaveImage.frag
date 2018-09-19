varying vec2 textureCoord;

uniform sampler2D mSample0;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

void main() 
{
    vec2 uv=textureCoord;
    uv=vec2(uv.x, 1.0 - uv.y);
    gl_FragColor = texture2D(mSample0, uv);
}
