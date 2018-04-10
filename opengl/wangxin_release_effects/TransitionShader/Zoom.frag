#ifdef GL_ES
precision highp float;
#endif

// General parameters
float progress = PREFIX(global_time);
vec2 resolution = iResolution;

const float minImgSize = 5.;

vec4 FUNCNAME(vec2 tc) {
  vec4 color;
  vec2 p = tc;
  float i = gl_FragCoord.y;
  float j = gl_FragCoord.x;
  float width = resolution.x;
  float height = resolution.y;
  if (progress >= 1.0)
  {
    color = INPUT2(vec2(p));
  }
  else
  {
    float dFromImgScale = 1. + 10. * progress;
    float FromI = (gl_FragCoord.y - resolution.y / 2.) / dFromImgScale + resolution.y / 2.;
	  float FromJ = (gl_FragCoord.x- resolution.x / 2.) / dFromImgScale + resolution.x / 2.;
    color = INPUT1(vec2(FromJ / resolution.x, FromI / resolution.y));
    
    float dToImgScale = progress;
    float rectToImg_width = resolution.x * dToImgScale;
	  float rectToImg_height = resolution.y * dToImgScale;
	  float rectToImg_x = (resolution.x - rectToImg_width) / 2.;
  	float rectToImg_y = (resolution.y - rectToImg_height) / 2.;
  	if (rectToImg_width > minImgSize)
  	{
  	  if (abs(i - resolution.y / 2.) < rectToImg_height / 2. &&
					abs(j - resolution.x / 2.) < rectToImg_width / 2.) 
				{
				  float ToI = (i - height / 2.) / dToImgScale + height / 2.;
					float ToJ = (j - width / 2.) / dToImgScale + width / 2.;
					color = INPUT2(vec2(ToJ / resolution.x, ToI / resolution.y));
				}
  	}
  }
  return color;
}
