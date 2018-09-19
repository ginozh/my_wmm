#ifdef GL_ES
precision highp float;
#endif
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;

// default interpolationPower = 5;
float interpolationPower = 5.0;

vec2 correctCoord(vec2 orip, vec2 tc){
    vec2 p = orip;
    if(p.x<0.0) p.x=0.0;
    if(p.y<0.0) p.y=0.0;
    if(p.x>tc.x) p.x=tc.x;
    if(p.y>tc.y) p.y=tc.y;
    return p;
}

void main() 
{
    vec2 tc = textureCoord;
    vec2 p = gl_FragCoord.xy / resolution.xy;
    p=correctCoord(p, tc);
    vec4 fTex = texture2D(mSample0, p);
    vec4 tTex = texture2D(mSample1, p);
    gl_FragColor =  mix(distance(fTex,tTex)>progress?fTex:tTex,
            tTex,
            pow(progress+0.1,interpolationPower));
}
