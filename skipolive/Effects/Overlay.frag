#ifdef GL_ES
precision highp float;
#endif
//varying vec2 textureCoord;
varying vec2 vTexCoord;

uniform sampler2D mSample0;
uniform sampler2D mSample1;
uniform sampler2D mSample2;
uniform int u_sample_cnt;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;
void main() 
{
    vec4 color;
    vec4 color1;
    vec4 color2;
    vec4 overlay_color;
    float overlay_alpa;

    color = texture2D(mSample0, vTexCoord);
    color1 = texture2D(mSample1, vTexCoord);
    color2 = texture2D(mSample2, vTexCoord);
    if(color1.a>0.0)
    {
		color = mix(color,color1,color1.a);
    }
    if(color2.a>0.0)
    {
		color = mix(color,color2,color2.a);
    }
    gl_FragColor = color;
}
