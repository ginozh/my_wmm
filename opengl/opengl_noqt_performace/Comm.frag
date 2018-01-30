varying vec2 textureCoord;

uniform sampler2D mSample0;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;
void main() 
{
    gl_FragColor = texture2D(mSample0, textureCoord);
}
void test()
{
    vec4 color;
    float dFromImgScale = 1. + 10. * progress;
    float FromI = (gl_FragCoord.y - resolution.y / 2.) / dFromImgScale + resolution.y / 2.;
    float FromJ = (gl_FragCoord.x- resolution.x / 2.) / dFromImgScale + resolution.x / 2.;
    color = texture2D(mSample0, vec2(FromJ / resolution.x, FromI / resolution.y));
    gl_FragColor = color;
	// vec4 orig = texture2D(mSample0, textureCoord);
    // gl_FragColor = vec4(orig.rgb,1);
}
void test1()
{
    // 0~1
    float x = gl_FragCoord.x/resolution.x;
    float y = gl_FragCoord.y/resolution.y;
    float z = gl_FragCoord.z; // Already in range [0,1]
    if(x>=0.5 && y>=0.5)
    {
        gl_FragColor = texture2D(mSample0, textureCoord);
    }
    else
    {
        gl_FragColor = vec4(0, 0, 0, 1);
    }
}
