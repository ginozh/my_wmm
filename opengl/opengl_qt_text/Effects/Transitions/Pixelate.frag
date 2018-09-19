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

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
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
    float revProgress = (1.0 - progress);
    float distFromEdges = min(progress, revProgress);
    float squareSize = (50.0 * distFromEdges) + 1.0;  

    vec2 p = (floor((gl_FragCoord.xy + squareSize * 0.5) / squareSize) * squareSize) / resolution.xy;
    p=correctCoord(p, tc);
    vec4 fromColor = texture2D(mSample0, p);
    vec4 toColor = texture2D(mSample1, p);

    if(progress<0.0) progress=0.0;
    if(progress>1.0) progress=1.0;
    gl_FragColor = mix(fromColor, toColor, progress);
}
