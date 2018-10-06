varying vec2 textureCoord;

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
    float overlay_alpa;

    color = texture2D(mSample0, textureCoord);
    overlay_alpa = texture2D(mSample1, textureCoord).a;
    //if(overlay_alpa==1.0)
    if(overlay_alpa>0.0)
    {
        //color = texture2D(mSample1, textureCoord);
		color = mix(texture2D(mSample0, textureCoord),texture2D(mSample1, textureCoord),overlay_alpa);
    }
    gl_FragColor = color;
    //gl_FragColor = texture2D(mSample1, textureCoord);
/*
    vec4 color;
    vec2 tc = textureCoord;
    float i = gl_FragCoord.y;
    float j = gl_FragCoord.x;
    float width = resolution.x;
    float height = resolution.y;

    float dToImgScale = 0.7;
    float rectToImg_width = width * dToImgScale;
    float rectToImg_height = height * dToImgScale;
    float rectToImg_x = (width - rectToImg_width) / 2.;
    float rectToImg_y = (height - rectToImg_height) / 2.;
    if (abs(gl_FragCoord.y - height / 2.) < rectToImg_height / 2. &&
            abs(gl_FragCoord.x - width / 2.) < rectToImg_width / 2.) 
    {
        //float ToI = (gl_FragCoord.y - height / 2.) / dToImgScale + height / 2.;
        //float ToJ = (gl_FragCoord.x - width / 2.) / dToImgScale + width / 2.;
        //color = texture2D(mSample1, vec2(ToJ / width, ToI / height));
        color = texture2D(mSample1, textureCoord);
    }
*/
}
