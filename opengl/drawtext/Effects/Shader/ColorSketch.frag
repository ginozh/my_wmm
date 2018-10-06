/*{
	"GUID":"2D0FCDC7-956B-469f-8E05-7B36CC2933FF"
}*/

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

float clip_byte(float n)
{
	return clamp(n, 0.0, 1.0);
}

void main()
{
	vec2 tc = textureCoord;
	float offsetX = 1.0 / u_resolution.x;
	float offsetY = 1.0 / u_resolution.y;
	
    vec4 top0 = texture2D(mSample0, vec2(tc.x, tc.y + offsetY));
    vec4 top1 = texture2D(mSample0, vec2(tc.x + offsetX, tc.y + offsetY));
    vec4 top2 = texture2D(mSample0, vec2(tc.x + offsetX*2., tc.y + offsetY));
    
    vec4 mid0 = texture2D(mSample0, vec2(tc.x, tc.y));
    vec4 mid1 = texture2D(mSample0, vec2(tc.x + offsetX, tc.y));
    vec4 mid2 = texture2D(mSample0, vec2(tc.x + offsetX*2., tc.y));
    
    vec4 bom0 = texture2D(mSample0, vec2(tc.x, tc.y - offsetY));
    vec4 bom1 = texture2D(mSample0, vec2(tc.x + offsetX, tc.y - offsetY));
    vec4 bom2 = texture2D(mSample0, vec2(tc.x + offsetX*2., tc.y - offsetY));
    
    top0.b = clip_byte(top0.b - mid1.b);
    top0.g = clip_byte(top0.g - mid1.g);
    top0.r = clip_byte(top0.r - mid1.r);
    top1.b = clip_byte(top1.b - mid1.b);
    top1.g = clip_byte(top1.g - mid1.g);
    top1.r = clip_byte(top1.r - mid1.r);
    top2.b = clip_byte(top2.b - mid1.b);
    top2.g = clip_byte(top2.g - mid1.g);
    top2.r = clip_byte(top2.r - mid1.r);
    mid0.b = clip_byte(mid0.b - mid1.b);
    mid0.g = clip_byte(mid0.g - mid1.g);
    mid0.r = clip_byte(mid0.r - mid1.r);
    mid2.b = clip_byte(mid2.b - mid1.b);
    mid2.g = clip_byte(mid2.g - mid1.g);
    mid2.r = clip_byte(mid2.r - mid1.r);
    bom0.b = clip_byte(bom0.b - mid1.b);
    bom0.g = clip_byte(bom0.g - mid1.g);
    bom0.r = clip_byte(bom0.r - mid1.r);
    bom1.b = clip_byte(bom1.b - mid1.b);
    bom1.g = clip_byte(bom1.g - mid1.g);
    bom1.r = clip_byte(bom1.r - mid1.r);
    bom2.b = clip_byte(bom2.b - mid1.b);
    bom2.g = clip_byte(bom2.g - mid1.g);
    bom2.r = clip_byte(bom2.r - mid1.r);
	
	float b0, g0, r0, b1, g1, r1, b2, g2, r2, b, g, r;
	
	b0 = clip_byte(clip_byte(top0.b - top1.b) + top1.b - top2.b) + top2.b;
    g0 = clip_byte(clip_byte(top0.g - top1.g) + top1.g - top2.g) + top2.g;
    r0 = clip_byte(clip_byte(top0.r - top1.r) + top1.r - top2.r) + top2.r;
    
    b1 = clip_byte(mid0.b - mid2.b) + mid2.b;
    g1 = clip_byte(mid0.g - mid2.g) + mid2.g;
    r1 = clip_byte(mid0.r - mid2.r) + mid2.r;
    
    b2 = clip_byte(clip_byte(bom0.b - bom1.b) + bom1.b - bom2.b) + bom2.b;
    g2 = clip_byte(clip_byte(bom0.g - bom1.g) + bom1.g - bom2.g) + bom2.g;
    r2 = clip_byte(clip_byte(bom0.r - bom1.r) + bom1.r - bom2.r) + bom2.r;
    
    b = clip_byte(clip_byte(b0 - b1) + b1 - b2) + b2;
    g = clip_byte(clip_byte(g0 - g1) + g1 - g2) + g2;
    r = clip_byte(clip_byte(r0 - r1) + r1 - r2) + r2;
	
	float y1 = b * 255.0;
	float x1 = sqrt(mid1.b * 255.) + 1.0;
	float z = 1.4 * x1 / (2. * x1 + y1);
	b = mid1.b + (1.0 - mid1.b) * z;
	
	y1 = g  * 255.0;
	x1 = sqrt(mid1.g * 255.) + 1.0;
	z = 1.4 * x1 / (2. * x1 + y1);
	g = mid1.g + (1. - mid1.g) * z;
	
	y1 = r  * 255.0;
	x1 = sqrt(mid1.r * 255.) + 1.0;
	z = 1.4 * x1 / (2. * x1 + y1);
	r = mid1.r + (1. - mid1.r) * z;
	
    gl_FragColor = vec4(r, g, b, texture2D(mSample0, tc).a);
}
