/*{
	"GUID":"43E461FF-BEB3-4ef1-974A-93812CB8D210",
	"INPUTS":[
		{
			"TYPE":"image",
			"FILE":"oldtime.png"
		}
	]
}*/

uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

vec2 offset[25];

//Do gray level adjust
float DoGrayLevel(vec3 inRGB, float lowLevel, float highLevel)
{
    float range = highLevel - lowLevel;
    float gray = dot(inRGB, vec3(0.299, 0.587, 0.114));
    
    gray = max(gray, lowLevel);
    gray = min(gray, highLevel);
    
    gray = (gray - lowLevel) / range;
    
    return gray;
}

void initOffset(float st)
{
    float xInc = st / u_resolution.x;
    float yInc = st / u_resolution.y;
    int loc = 0;
    float startPxlX = -(3.0 / 2.) * xInc;
    float startPxlY = -(3.0 / 2.) * yInc;
    float curPxLY = 0.;
    for(int j = 0; j < 3; j++)
    {
        curPxLY = startPxlY + float(j) * yInc;
        for(int i = 0; i < 3; i++)
        {
            offset[j*3+i].x=startPxlX + float(i) * xInc;
            offset[j*3+i].y = curPxLY;
            loc+=1;
        }
    }
}

void main()
{
	vec2 tc = textureCoord;
	vec2 uv = tc;
    vec4 gaussian;
    vec4 sample[9];
    initOffset(2.);
    for (int i = 0; i < 9; i++)
    {
        // Sample a grid around and including our texel
        sample[i] = texture2D(mSample0, uv + offset[i]);
    }

    // Gaussian weighting:
    // 1  4  7  4 1
    // 4 16 26 16 4
    // 7 26 41 26 7 / 273 (i.e. divide by total of weightings)
    // 4 16 26 16 4
    // 1  4  7  4 1

    gaussian = ( (16.0  * (sample[0] + sample[2]  + sample[6] + sample[8])) +
                 (26.0  * (sample[1] + sample[3]  + sample[5] + sample[7])) +
                 (41.0 * sample[4]) ) / (4.0 * 16.0 + 4.0 * 26.0 + 41.0);

    vec4 original = sample[4];

    
    //Level adjust
    float levelAdjust = DoGrayLevel(original.rgb, 50.0/255.0, 200.0/255.0);

    //Blending
    gaussian = (gaussian + levelAdjust) / 2.0;
    
    gaussian.a = original.a;
    
    vec4 border = texture2D(mSample1, uv);
	gl_FragColor = mix(gaussian, border, border.a) ;
}
