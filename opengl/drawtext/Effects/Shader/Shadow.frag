/*{
	"GUID":"D4A1FDF7-FD14-490e-8D29-177AEC4DED39"
}*/

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

#define MAKE_GREY_EASY(b, g, r) (((b) + (g) + (g) + (r)) / 4.0)
#define AmMAX(a, b) ((a) < (b) ? (b) : (a))
#define AmMAX3(a, b, c) AmMAX(AmMAX(a, b), c)
#define ChannelBlend_Overlay(B,L) ((B) < 128.0) ? ((B) * (L) / 128.0 ): (255.0 - (((255.0 - (B)) * (255.0 - (L))) / 128.0)) 

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
	vec3 fragCoord = gl_FragCoord.xyz;
	
    vec2 pt0 = vec2(fragCoord.x, fragCoord.y + 1.0)/u_resolution.xy;
    vec2 pt1 = vec2(fragCoord.x + 1.0, fragCoord.y + 1.0)/u_resolution.xy;
    vec2 pt2 = vec2(fragCoord.x + 2.0, fragCoord.y + 1.0)/u_resolution.xy;
    vec4 top0 = texture2D(mSample0, correctCoord(pt0, tc) ) * vec4(255.0);
    vec4 top1 = texture2D(mSample0, correctCoord(pt1, tc)) * vec4(255.0);
    vec4 top2 = texture2D(mSample0, correctCoord(pt2, tc)) * vec4(255.0);
    
    float ntop0 = MAKE_GREY_EASY(top0.b, top0.g, top0.r);
    float ntop1 = MAKE_GREY_EASY(top1.b, top1.g, top1.r);
    float ntop2 = MAKE_GREY_EASY(top2.b, top2.g, top2.r);
    vec2 p1=vec2(fragCoord.x, fragCoord.y)/u_resolution.xy;
    vec2 p2=vec2(fragCoord.x + 1.0, fragCoord.y)/u_resolution.xy;
    vec2 p3=vec2(fragCoord.x + 2.0, fragCoord.y)/u_resolution.xy;
    vec4 mid0 = texture2D(mSample0, correctCoord(p1,tc)) * vec4(255.0);
    vec4 mid1 = texture2D(mSample0, correctCoord(p2,tc)) * vec4(255.0);
    vec4 mid2 = texture2D(mSample0, correctCoord(p3,tc)) * vec4(255.0);
    
    float nmid0 = MAKE_GREY_EASY(mid0.b, mid0.g, mid0.r);
    float nmid1 = MAKE_GREY_EASY(mid1.b, mid1.g, mid1.r);
    float nmid2 = MAKE_GREY_EASY(mid2.b, mid2.g, mid2.r);
   
    vec4 bom0 = texture2D(mSample0, vec2(fragCoord.x, fragCoord.y - 1.0)/u_resolution.xy) * vec4(255.0);
    vec4 bom1 = texture2D(mSample0, vec2(fragCoord.x + 1.0, fragCoord.y - 1.0)/u_resolution.xy) * vec4(255.0);
    vec4 bom2 = texture2D(mSample0, vec2(fragCoord.x + 2.0, fragCoord.y - 1.0)/u_resolution.xy) * vec4(255.0);
    
    float nbom0 = MAKE_GREY_EASY(bom0.b, bom0.g, bom0.r);
    float nbom1 = MAKE_GREY_EASY(bom1.b, bom1.g, bom1.r);
    float nbom2 = MAKE_GREY_EASY(bom2.b, bom2.g, bom2.r);
    
    float value1 = AmMAX3(ntop0, ntop1, ntop2);
    float value2 = AmMAX3(nmid0, nmid1, nmid2);
    float value3 = AmMAX3(nbom0, nbom1, nbom2);
    
    value1 = AmMAX3(value1, value2, value3);
    float tmp =  (nmid0 * (16777216.0 / (value1 + 8.0))) / 65536.0;
    value2 = min(tmp, 255.0);
    value3 = ChannelBlend_Overlay(value2,nmid0);

    gl_FragColor = vec4(value3/255.0, value3/255.0, value3/255.0, 1.0);
}
