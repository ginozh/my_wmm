#ifdef GL_ES
precision highp float;
#endif
//varying vec2 textureCoord;
varying vec2 vTexCoord;

uniform sampler2D mSample0;
uniform sampler2D mSample1;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;
void main() 
{
    vec4 color;
    vec4 overlay_color;
    float overlay_alpa;

    color = texture2D(mSample0, vTexCoord);
    overlay_color = texture2D(mSample1, vTexCoord);
    overlay_alpa = overlay_color.a;
    if(overlay_alpa>0.0)
    {
		color = mix(color,overlay_color,overlay_alpa);
    }
    gl_FragColor = color;
}
