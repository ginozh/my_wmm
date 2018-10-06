/*{
	"GUID":"A5394086-2AE7-4df3-B85B-0D69E4594338"
}*/

uniform sampler2D mSample0;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;

const vec2 MidTop = vec2(0.50000,0.88333);
const vec2 LeftBottom = vec2(0.15625, 0.08333);
const vec2 RightBottom = vec2(0.84375,0.08333);
const float  PI = 3.1415927;

float Degree2Arc(float d)
{
 	return d * PI / 180.;
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
	vec2 uv = gl_FragCoord.xy;
    float i = gl_FragCoord.y;
    float j = gl_FragCoord.x;
    float tan30 = tan(30. * PI / 180.);
    vec2 aMidTop = vec2(MidTop.x*u_resolution.x, MidTop.y*u_resolution.y);
	vec2 aLeftBottom = vec2(LeftBottom.x*u_resolution.x, LeftBottom.y*u_resolution.y);
	vec2 aRightBottom = vec2(RightBottom.x*u_resolution.x, RightBottom.y*u_resolution.y);
	aLeftBottom.x = aMidTop.x - (aMidTop.y - aLeftBottom.y) * tan30;
	aRightBottom.x = aMidTop.x + (aMidTop.y - aLeftBottom.y) * tan30;
    
	float angleTopMid = atan(float(i - aMidTop.y), float(j - aMidTop.x));
			float angleBottomLeft = atan(float(i - aLeftBottom.y), float(j - aLeftBottom.x));
			float angleBottomRight = atan(float(i - aRightBottom.y), float(j - aRightBottom.x));
			//Mid Triangle
			if (i <= aMidTop.y  && 
				angleBottomLeft > Degree2Arc(60.) && angleBottomLeft <= Degree2Arc(120.))	//Left Triangle
			{
				float nDistance = sqrt(float(i - aMidTop.y) * (i - aMidTop.y) + (j - aMidTop.x) * (j - aMidTop.x));
				uv.x = aMidTop.x + nDistance * cos(angleTopMid + 60. * PI / 180.);
				uv.y = aMidTop.y + nDistance * sin(angleTopMid + 60. * PI / 180.);
			}
			else if (i <= aMidTop.y &&
				angleBottomRight > Degree2Arc(60.) && angleBottomRight <= Degree2Arc(120.))	//Right Triangle
			{
				float nDistance = sqrt(float(i - aMidTop.y) * (i - aMidTop.y) + (j - aMidTop.x) * (j - aMidTop.x));
				uv.x = aMidTop.x + nDistance * cos(angleTopMid - 60. * PI / 180.);
				uv.y = aMidTop.y + nDistance * sin(angleTopMid - 60. * PI / 180.);
			}
			else if (angleTopMid >= Degree2Arc(0.) && angleTopMid < Degree2Arc(60.))
			{
				float nDistance = sqrt(float(i - aMidTop.y) * (i - aMidTop.y) + (j - aMidTop.x) * (j - aMidTop.x));
				uv.x = aMidTop.x + nDistance * cos(angleTopMid - 120. * PI / 180.);
				uv.y = aMidTop.y + nDistance * sin(angleTopMid - 120. * PI / 180.);
			}
			else if (angleTopMid >= Degree2Arc(60.) && angleTopMid < Degree2Arc(120.))
			{
				float nDistance = sqrt(float(i - aMidTop.y) * (i - aMidTop.y) + (j - aMidTop.x) * (j - aMidTop.x));
				uv.x = aMidTop.x + nDistance * cos(angleTopMid + 180. * PI / 180.);
				uv.y = aMidTop.y + nDistance * sin(angleTopMid + 180. * PI / 180.);
			}
			else if (angleTopMid >= Degree2Arc(120.) && angleTopMid < Degree2Arc(180.))
			{
				float nDistance = sqrt(float(i - aMidTop.y) * (i - aMidTop.y) + (j - aMidTop.x) * (j - aMidTop.x));
				uv.x = aMidTop.x + nDistance * cos(angleTopMid + 120. * PI / 180.);
				uv.y = aMidTop.y + nDistance * sin(angleTopMid + 120. * PI / 180.);
			}

			else if (angleBottomRight >= Degree2Arc(0.) && angleBottomRight < Degree2Arc(60.))
			{
				float nDistance = sqrt(float(i - aRightBottom.y) * (i - aRightBottom.y) + (j - aRightBottom.x) * (j - aRightBottom.x));
				uv.x = aRightBottom.x + nDistance * cos(angleBottomRight + 120. * PI / 180.);
				uv.y = aRightBottom.y + nDistance * sin(angleBottomRight + 120. * PI / 180.);
			}
			else if (angleBottomRight >= Degree2Arc(-60.) && angleBottomRight < Degree2Arc(0.))
			{
				float nDistance = sqrt(float(i - aRightBottom.y) * (i - aRightBottom.y) + (j - aRightBottom.x) * (j - aRightBottom.x));
				uv.x = aRightBottom.x + nDistance * cos(-angleBottomRight + 120. * PI / 180.);
				uv.y = aRightBottom.y + nDistance * sin(-angleBottomRight + 120. * PI / 180.);
				
			}
			else if (angleBottomRight >= Degree2Arc(-120.) && angleBottomRight < Degree2Arc(-60.))
			{
				float nDistance = sqrt(float(i - aRightBottom.y) * (i - aRightBottom.y) + (j - aRightBottom.x) * (j - aRightBottom.x));
				uv.x = aLeftBottom.x + nDistance * cos(angleBottomRight + 120. * PI / 180.);	//aLeftBottom to make it looks like mirror
				uv.y = aLeftBottom.y + nDistance * sin(angleBottomRight + 120. * PI / 180.);
			}
			else if (angleBottomRight >= Degree2Arc(-180.) && angleBottomRight < Degree2Arc(-120.) &&
					angleBottomLeft >= Degree2Arc(-60.) && angleBottomLeft < Degree2Arc(0.)
				)
			{
                uv.x = uv.x*i;
                uv.y = 2. * aRightBottom.y - i;
			}
			else if (angleBottomLeft >= Degree2Arc(-120.) && angleBottomLeft < Degree2Arc(-60.)
				)
			{
				float nDistance = sqrt(float(i - aLeftBottom.y) * (i - aLeftBottom.y) + (j - aLeftBottom.x) * (j - aLeftBottom.x));
				uv.x = aRightBottom.x + nDistance * cos(angleBottomLeft - 120. * PI / 180.);
				uv.y = aRightBottom.y + nDistance * sin(angleBottomLeft - 120. * PI / 180.);
			}
			else if (angleBottomLeft > Degree2Arc(-180.) && angleBottomLeft < Degree2Arc(-120.))
			{
				float nDistance = sqrt(float(i - aLeftBottom.y) * (i - aLeftBottom.y) + (j - aLeftBottom.x) * (j - aLeftBottom.x));
				uv.x = aLeftBottom.x + nDistance * cos(-angleBottomLeft - 120. * PI / 180.);
				uv.y = aLeftBottom.y + nDistance * sin(-angleBottomLeft - 120. * PI / 180.);
				
			}
			else if (angleBottomLeft > Degree2Arc(120.) && angleBottomLeft <= Degree2Arc(181.) )
			{
				float nDistance = sqrt(float(i - aLeftBottom.y) * (i - aLeftBottom.y) + (j - aLeftBottom.x) * (j - aLeftBottom.x));
				uv.x = aLeftBottom.x + nDistance * cos(angleBottomLeft - 120. * PI / 180.);
				uv.y = aLeftBottom.y + nDistance * sin(angleBottomLeft - 120. * PI / 180.);
				
			}
			uv = uv / u_resolution.xy;
    uv=correctCoord(uv, tc);
	gl_FragColor = texture2D(mSample0, uv);
}
