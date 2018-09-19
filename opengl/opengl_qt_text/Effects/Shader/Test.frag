varying vec2 textureCoord;

uniform sampler2D mSample0;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;
uniform float u_first;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;
const float PI = 3.141592653589793;
void test();
void testmirror();
void testrotate();
void testrotate1();
void testrotate2();
void main() 
{
    vec2 uv=textureCoord;
    //if( u_first==1.0)
    //    uv=vec2(uv.x, 1.0 - uv.y);
    gl_FragColor = texture2D(mSample0, uv);
    //gl_FragColor.a = 1.0;//0.5;
    //gl_FragColor = texture2D(mSample0, textureCoord);
    //gl_FragColor = texture2D(mSample0, vec2(textureCoord.s, 1.0-textureCoord.t));
    //test();
    //testmirror();
    //testrotate();
/*
    if( u_first==1.0)
    {
        //testrotate1();
        testrotate2();
    }
*/
}
vec2 rotate_point(float cx,float cy,float angle,vec2 p)
{
  float s = sin(angle);
  float c = cos(angle);

  // translate point back to origin:
  p.x -= cx;
  p.y -= cy;

  // rotate point
  float xnew = p.x * c - p.y * s;
  float ynew = p.x * s + p.y * c;

  // translate point back:
  p.x = xnew + cx;
  p.y = ynew + cy;
  return p;
}
void testrotate()
{
    vec4 color;
    float width = resolution.x;
    float height = resolution.y;

    color=vec4(0,0,0,0);

    float dToImgScale = 1.0;
    float rectToImg_width = width * dToImgScale * 0.5;
    float rectToImg_height = height * dToImgScale;
    float rectToImg_x = (width - rectToImg_width) / 2.;
    float rectToImg_y = (height - rectToImg_height) / 2.;
    if (abs(gl_FragCoord.y - height / 2.) < rectToImg_height / 2. &&
            abs(gl_FragCoord.x - width / 2.) < rectToImg_width / 2.) 
    {
        float ToI = (gl_FragCoord.y - height / 2.) / dToImgScale + height / 2.;
        float ToJ = (gl_FragCoord.x - width / 2.) / (dToImgScale*0.5) + width / 2.;
        //color = texture2D(mSample1, vec2(ToJ / width, ToI / height));
        ////color = texture2D(mSample1, textureCoord);
        vec2 uv = vec2(ToJ / width, ToI / height);
        //vec2 uv = textureCoord.xy;
        float rotateDeta = PI/180.0*(90.0);
        uv=rotate_point(0.5, 0.5, rotateDeta, uv);
        ////gl_FragColor = texture2D(mSample0, uv);
        color = texture2D(mSample0, uv);
    }
    gl_FragColor = color;
}
void testrotate2()
{
    float angle=90.0;
    vec2 uv = textureCoord.xy;
    float rotateDeta = PI/180.0*(angle);
    uv=rotate_point(0.5, 0.5, rotateDeta, uv);
    gl_FragColor = texture2D(mSample0, uv);
}
void testrotate1()
{
    vec4 color;
    float w=960.0;
    float h=720.0;
    float width=u_resolution.x;
    float height=u_resolution.y;
    float angle=90.0;
    float lastw=w, lasth=h;

    color=vec4(0,0,0,0);
    if(angle==90.0)
    {
        float tmp=height*h/w;
        if(tmp<=width)
        {
            lastw=height;
            lasth=tmp;
        }
        else
        {
            //no way
        }
    }
    // (w/2-lw/2, w/2+lw/2) (h/2-lh/2, h/2+lh/2)
    if(gl_FragCoord.x>width/2.0-lastw/2.0 &&
        gl_FragCoord.x<width/2.0+lastw/2.0 &&
        gl_FragCoord.y>height/2.0-lasth/2.0 &&
        gl_FragCoord.y<height/2.0+lasth/2.0)
    {
        float ToI = gl_FragCoord.x*width/lastw + width/2.0 - width*width/(2.0*lastw);
        float ToJ = gl_FragCoord.y*height/lasth + height/2.0 - height*height/(2.0*lasth);
        vec2 uv = vec2(ToJ / width, ToI / height);
        //vec2 uv = textureCoord.xy;
        float rotateDeta = PI/180.0*(angle);
        uv=rotate_point(0.5, 0.5, rotateDeta, uv);
        color = texture2D(mSample0, uv);
    }
    gl_FragColor = color;
}
void testmirror()
{
    vec2 uv = textureCoord.xy;
/*
    //mirror
    if( uv.x > 0.5 ) 
    {
        uv.x = 1.0 - uv.x;
    }
*/
/*
    uv.x = 1.0 - uv.x; //hflip
    uv.y = 1.0 - uv.y;  //vflip
*/
    gl_FragColor = texture2D(mSample0, uv);
/*
    float x = gl_FragCoord.x/resolution.x;
    float y = gl_FragCoord.y/resolution.y;
    float z = gl_FragCoord.z; // Already in range [0,1]
    if(x>=0.5 && y>=0.5)
    {
        gl_FragColor = texture2D(mSample0, uv);
    }
    else
    {
        gl_FragColor = vec4(0, 0, 0, 0);
    }
*/
}
void test()
{
    vec4 color;
    float dFromImgScale = 1. + 10. * progress;
    float FromI = (gl_FragCoord.y - resolution.y / 2.) / dFromImgScale + resolution.y / 2.;
    float FromJ = (gl_FragCoord.x- resolution.x / 2.) / dFromImgScale + resolution.x / 2.;
    color = texture2D(mSample0, vec2(FromJ / resolution.x, FromI / resolution.y));
    gl_FragColor = color;
	// vec4 orig = texture2D(mSample0, textureCoord);
    // gl_FragColor = vec4(orig.rgb,1);
}
void test1()
{
    // 0~1
    float x = gl_FragCoord.x/resolution.x;
    float y = gl_FragCoord.y/resolution.y;
    float z = gl_FragCoord.z; // Already in range [0,1]
    if(x>=0.5 && y>=0.5)
    {
        gl_FragColor = texture2D(mSample0, textureCoord);
    }
    else
    {
        gl_FragColor = vec4(0, 0, 0, 1);
    }
}
