/*{
	"GUID":"D3FFB279-0581-4c3c-8CAC-CB2E1D148614"
}*/
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

vec4 screen(vec4 base, vec4 blend)
{
    vec4 white = vec4(1.0);
    vec4 result = white - ((white - blend) * (white - base));
    return result;
}

vec4 DoOverlay(vec4 base, vec4 blend)
{
    vec4 result;
    vec4 lumCoeff = vec4(0.0721, 0.7154, 0.2125, 0.0);
    vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
    
    float luminance = dot(base, lumCoeff);
    if (luminance < 0.45)
        result = 2.0 * blend * base;
    else if (luminance > 0.55)
        result = white - 2.0 * (white - blend) * (white - base);
    else
    {
        vec4 result1 = 2.0 * blend * base;
        vec4 result2 = white - 2.0 * (white - blend) * (white - base);
        result = mix(result1, result2, (luminance - 0.45) * 10.0);
    }
    return result;
}

vec4 RGBA2GRAY(vec4 inRGBA)
{
    float gray = dot(inRGBA.rgb, vec3(0.299, 0.587, 0.114));
    vec4 outGray = vec4(vec3(gray, gray, gray), inRGBA.a);
    return outGray;
}

void main() 
{
	vec2 tc = textureCoord;
    vec4 inBGRA = texture2D(mSample0, tc);
    vec4 color = inBGRA;
    vec4 gray = RGBA2GRAY(inBGRA);
    color = screen(color, gray);
    vec3 white = vec3(1.0, 1.0, 1.0);
    float lim = 0.0;
    vec3 cmy = white - color.rgb;
    vec3 cmy1 = vec3(0.0,0.0,0.0);
    vec3 cmy2 = vec3(0.0,0.0,0.0);
    vec3 cmy3 = vec3(0.0,0.0,0.0);
    float vmax = max(max(color.r, color.g), color.b);
    float vmin = min(min(color.r, color.g), color.b);
    float vmid = max(min(color.r, color.g), min(max(color.r, color.g), color.b));
    
    lim = vmax - vmid;
    if(vmax == color.r)
    {
        cmy1.r = -cmy.r * lim;
        cmy1.g = -cmy.g * lim * 0.5;
        cmy1.b = -cmy.b * lim * 0.25;
    }
    else if(vmax == color.b)
    {
        cmy1.r = -cmy.r * lim * 0.6;
        cmy1.g = -cmy.g * lim * 0.25;
        cmy1.b = cmy.b * lim * 0.25;
    }
    else if(vmax == color.g)
    {
        cmy1.r = -cmy.r * lim * 0.8;
        cmy1.g = -cmy.g * lim * 0.1;
        cmy1.b = cmy.b * lim;
    }

    lim = vmid - vmin;
    if (vmin == color.b)
    {
        cmy2.r = -cmy.r * lim * 0.75;
        cmy2.g = cmy.g * lim * 0.08;
        cmy2.b = cmy.b * lim * 0.08;
    }
    else if(vmin == color.g)
    {
        cmy2.r = -cmy.r * lim;
        cmy2.g = -cmy.g * lim;
        cmy2.b = -cmy.b * lim;
    }
    else
    {
        cmy2.g = -cmy.g * lim;
    }
    
    lim = 1.0 - abs(vmax - 0.5) - abs(vmin - 0.5);
    cmy3.r = -cmy.r * lim * 0.25;
    cmy3.g = -cmy.g * lim * 0.25;
    cmy3.b = -cmy.b * lim * 0.25;
    
    cmy = clamp(cmy + cmy1 + cmy2 + cmy3, 0.0, 1.0);
    color.rgb = white - cmy;
    vec4 temp = screen(vec4(0.3725, 0.2392, 0.2157, inBGRA.a), gray);
    color = DoOverlay(color, temp);
    color.a = inBGRA.a;
    
    gl_FragColor = color;
}


