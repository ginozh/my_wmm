const float blurSize = 1.0/512.0;
const float intensity = 0.35;
vec4 FUNCNAME(vec2 tc) 
{
    vec4 sum = vec4(0);
    vec2 texcoord = tc;

    sum += INPUT(vec2(texcoord.x, texcoord.y)) * 0.20;

    sum += INPUT(vec2(texcoord.x + blurSize, texcoord.y)) * 0.11;
    sum += INPUT(vec2(texcoord.x - blurSize, texcoord.y)) * 0.11;
    sum += INPUT(vec2(texcoord.x, texcoord.y + blurSize)) * 0.11;
    sum += INPUT(vec2(texcoord.x, texcoord.y - blurSize)) * 0.11;

    sum += INPUT(vec2(texcoord.x + blurSize, texcoord.y + blurSize)) * 0.07;
    sum += INPUT(vec2(texcoord.x + blurSize, texcoord.y - blurSize)) * 0.07;
    sum += INPUT(vec2(texcoord.x - blurSize, texcoord.y + blurSize)) * 0.07;
    sum += INPUT(vec2(texcoord.x - blurSize, texcoord.y - blurSize)) * 0.07;

    sum += INPUT(vec2(texcoord.x + 2.0*blurSize, texcoord.y)) * 0.02;
    sum += INPUT(vec2(texcoord.x - 2.0*blurSize, texcoord.y)) * 0.02;
    sum += INPUT(vec2(texcoord.x, texcoord.y + 2.0*blurSize)) * 0.02;
    sum += INPUT(vec2(texcoord.x, texcoord.y - 2.0*blurSize)) * 0.02;

    //increase blur with intensity!
    //fragColor = sum*intensity + INPUT(texcoord); 
    return sum * PREFIX(fade) + INPUT(texcoord);
}

