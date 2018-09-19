#ifdef GL_ES
precision highp float;
#endif

// General parameters
uniform sampler2D mSample0;
uniform sampler2D mSample1;

varying vec2 textureCoord;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;

const float minImgSize = 5.;

void main() 
{
	vec2 tc = textureCoord;
  vec4 color;
  vec2 p = tc;
  float i = gl_FragCoord.y;
  float j = gl_FragCoord.x;
  float width = resolution.x;
  float height = resolution.y;
  if (progress >= 1.0)
  {
    color = texture2D(mSample1, vec2(p));
  }
  else
  {
    // (w/2-w*s/2, w/2+w*s/2) (h/2-h*s/2, h/2+h*s/2) => (0, w) (0, h)
    float dFromImgScale = 1. + 10. * progress;
    float FromI = (gl_FragCoord.y - resolution.y / 2.) / dFromImgScale + resolution.y / 2.;
	  float FromJ = (gl_FragCoord.x- resolution.x / 2.) / dFromImgScale + resolution.x / 2.;
    color = texture2D(mSample0, vec2(FromJ / resolution.x, FromI / resolution.y));
    
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
					color = texture2D(mSample1, vec2(ToJ / resolution.x, ToI / resolution.y));
				}
  	}
  }
  gl_FragColor = color;
}
