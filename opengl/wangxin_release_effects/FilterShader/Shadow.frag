/*{
	"GUID":"D4A1FDF7-FD14-490e-8D29-177AEC4DED39"
}*/

#define MAKE_GREY_EASY(b, g, r) (((b) + (g) + (g) + (r)) / 4.0)
#define AmMAX(a, b) ((a) < (b) ? (b) : (a))
#define AmMAX3(a, b, c) AmMAX(AmMAX(a, b), c)
#define ChannelBlend_Overlay(B,L) ((B) < 128.0) ? ((B) * (L) / 128.0 ): (255.0 - (((255.0 - (B)) * (255.0 - (L))) / 128.0)) 

vec4 FUNCNAME(vec2 tc) 
{
	vec3 fragCoord = gl_FragCoord.xyz;
	
    vec4 top0 = INPUT(vec2(fragCoord.x, fragCoord.y + 1.0)/iResolution.xy) * vec4(255.0);
    vec4 top1 = INPUT(vec2(fragCoord.x + 1.0, fragCoord.y + 1.0)/iResolution.xy) * vec4(255.0);
    vec4 top2 = INPUT(vec2(fragCoord.x + 2.0, fragCoord.y + 1.0)/iResolution.xy) * vec4(255.0);
    
    float ntop0 = MAKE_GREY_EASY(top0.b, top0.g, top0.r);
    float ntop1 = MAKE_GREY_EASY(top1.b, top1.g, top1.r);
    float ntop2 = MAKE_GREY_EASY(top2.b, top2.g, top2.r);
    
    vec4 mid0 = INPUT(vec2(fragCoord.x, fragCoord.y)/iResolution.xy) * vec4(255.0);
    vec4 mid1 = INPUT(vec2(fragCoord.x + 1.0, fragCoord.y)/iResolution.xy) * vec4(255.0);
    vec4 mid2 = INPUT(vec2(fragCoord.x + 2.0, fragCoord.y)/iResolution.xy) * vec4(255.0);
    
    float nmid0 = MAKE_GREY_EASY(mid0.b, mid0.g, mid0.r);
    float nmid1 = MAKE_GREY_EASY(mid1.b, mid1.g, mid1.r);
    float nmid2 = MAKE_GREY_EASY(mid2.b, mid2.g, mid2.r);
   
    vec4 bom0 = INPUT(vec2(fragCoord.x, fragCoord.y - 1.0)/iResolution.xy) * vec4(255.0);
    vec4 bom1 = INPUT(vec2(fragCoord.x + 1.0, fragCoord.y - 1.0)/iResolution.xy) * vec4(255.0);
    vec4 bom2 = INPUT(vec2(fragCoord.x + 2.0, fragCoord.y - 1.0)/iResolution.xy) * vec4(255.0);
    
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

    return vec4(value3/255.0, value3/255.0, value3/255.0, 1.0);
}
