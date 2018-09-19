varying vec2 textureCoord;

uniform sampler2D mSample0;
uniform vec2 u_resolution;
uniform float u_global_time;
uniform float u_total_time;
uniform float u_pos_x;
uniform float u_pos_y;
uniform float u_width;
uniform float u_height;
uniform int u_id;

float progress = u_global_time / u_total_time;
vec2 resolution = u_resolution;
const float PI = 3.141592653589793;
float dToImgScale = 1.5;

void panupleft();
void panup();
void panupright();
void pandownleft();
void pandown();
void pandownright();
void panright();
void panleft();
void zoom(int bZoomIn);

void main() 
{
    if(u_id==50)
        panupleft();
    else if(u_id==51)
        panup();
    else if(u_id==52)
        panupright();
    else if(u_id==53)
        pandownleft();
    else if(u_id==54)
        pandown();
    else if(u_id==55)
        pandownright();
    else if(u_id==57)
        panright();
    else if(u_id==61)
        panleft();
    else if(u_id==63)
        zoom(1);//zoomin
    else if(u_id==71)
        zoom(0);//zoomout
}

//( -(h*s-h)*(1-progress), h*s*progress+h*(1-progress))
// (-(w*s-w)*(1-progress), w*s*progress+w*(1-progress))
void panupright()
{
    float w = resolution.x;
    float h = resolution.y;
    vec2 uv = textureCoord;

    if(u_width>0.0 && u_height>0.0)
    {
        //if(gl_FragCoord.x >= (1.0-progress)*(w-u_pos_x-u_width)+progress*w
        //        && gl_FragCoord.y >= (1.0-progress)*u_pos_y+progress*h)
        if(gl_FragCoord.y >= progress*h || gl_FragCoord.x >= progress*w)
        {
            gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        }
        else
        {
            uv.x = uv.x +(1.0-progress)*(u_pos_x+u_width)/w;
            uv.y = uv.y +(1.0-progress)*(h-u_pos_y)/h;
            gl_FragColor = texture2D(mSample0, uv);
        }
    }
    else
    {
        float rectToImg_w = w * dToImgScale;
        float rectToImg_h = h * dToImgScale;

        //progress=1.0; //

        //float ToI = gl_FragCoord.x / dToImgScale + w - w / dToImgScale;
        float ToI = (gl_FragCoord.x + (rectToImg_w - w)*(1.0-progress)) / dToImgScale;
        float ToJ = (gl_FragCoord.y + (rectToImg_h - h)*(1.0-progress)) / dToImgScale;
        gl_FragColor = texture2D(mSample0, vec2(ToI / w, ToJ / h));
    }
}
//( -(h*s-h)*(1-progress),h*s*progress+h*(1-progress))
// (-(w*s-w)*p, w+(w*s-w)*(1-p)) 
void panupleft()
{
    float w = resolution.x;
    float h = resolution.y;
    vec2 uv = textureCoord;

    if(u_width>0.0 && u_height>0.0)
    {
        //if(gl_FragCoord.x <= (1.0-progress)*(w-u_pos_x)
        //        && gl_FragCoord.y >= (1.0-progress)*u_pos_y+progress*h)
        if(gl_FragCoord.y >= progress*h || gl_FragCoord.x <= (1.0-progress)*w)
        {
            gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        }
        else
        {
            uv.x = uv.x - (1.0-progress)*(w-u_pos_x)/w;
            uv.y = uv.y +(1.0-progress)*(h-u_pos_y)/h;
            gl_FragColor = texture2D(mSample0, uv);
        }
    }
    else
    {
        float rectToImg_w = w * dToImgScale;
        float rectToImg_h = h * dToImgScale;

        //progress=1.0; //

        float ToI = gl_FragCoord.x/dToImgScale - w*progress/dToImgScale + w*progress;
        float ToJ = (gl_FragCoord.y + (rectToImg_h - h)*(1.0-progress)) / dToImgScale;
        gl_FragColor = texture2D(mSample0, vec2(ToI / w, ToJ / h));
    }
}

//( -(h*s-h)*progress, h+(h*s-h)*(1-progress))
// (-(w*s-w)*p, w+(w*s-w)*(1-p)) 
void pandownleft()
{
    float w = resolution.x;
    float h = resolution.y;
    vec2 uv = textureCoord;

    if(u_width>0.0 && u_height>0.0)
    {
        //if(gl_FragCoord.x <= (1.0-progress)*(w-u_pos_x)
        //        && gl_FragCoord.y <= (1.0-progress)*(h-u_pos_x-u_height))
        if(gl_FragCoord.y <= (1.0-progress)*h || gl_FragCoord.x <= (1.0-progress)*w)
        {
            gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        }
        else
        {
            uv.x = uv.x - (1.0-progress)*(w-u_pos_x)/w;
            uv.y = uv.y -(1.0-progress)*(u_height+u_pos_y)/h;
            gl_FragColor = texture2D(mSample0, uv);
        }
    }
    else
    {
        float rectToImg_w = w * dToImgScale;
        float rectToImg_h = h * dToImgScale;

        //progress=1.0; //

        float ToI = gl_FragCoord.x/dToImgScale - w*progress/dToImgScale + w*progress;
        float ToJ = gl_FragCoord.y / dToImgScale + h * progress - h * progress / dToImgScale;
        gl_FragColor = texture2D(mSample0, vec2(ToI / w, ToJ / h));
    }
}
//(-(w*s/2-w/2), w*s/2+w/2) ( -(h*s-h)*progress, h+(h*s-h)*(1-progress))
void pandown()
{
    float w = resolution.x;
    float h = resolution.y;
    vec2 uv = textureCoord;

    if(u_width>0.0 && u_height>0.0)
    {
        //if(gl_FragCoord.y <= (1.0-progress)*(h-u_pos_y-u_height))
        if(gl_FragCoord.y <= (1.0-progress)*h)
        {
            gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        }
        else
        {
            uv.y = uv.y -(1.0-progress)*(u_height+u_pos_y)/h;
            gl_FragColor = texture2D(mSample0, uv);
        }
    }
    else
    {
        float rectToImg_w = w * dToImgScale;
        float rectToImg_h = h * dToImgScale;

        float ToI = gl_FragCoord.x / dToImgScale + w / 2.0 - w / (2.0*dToImgScale);
        float ToJ = gl_FragCoord.y / dToImgScale + h * progress - h * progress / dToImgScale;
        gl_FragColor = texture2D(mSample0, vec2(ToI / w, ToJ / h));

    }
}
// ( -(h*s-h)*progress, h+(h*s-h)*(1-progress))
// (-(w*s-w)(1-p), w+(w*s-w)*p)
void pandownright()
{
    float w = resolution.x;
    float h = resolution.y;
    vec2 uv = textureCoord;

    if(u_width>0.0 && u_height>0.0)
    {
        //if(gl_FragCoord.x >= (1.0-progress)*(w-u_pos_x-u_width)+progress*w
        //        && gl_FragCoord.y <= (1.0-progress)*(h-u_pos_x-u_height))
        if(gl_FragCoord.y <= (1.0-progress)*h || gl_FragCoord.x >= progress*w)
        {
            gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        }
        else
        {
            uv.x = uv.x +(1.0-progress)*(u_pos_x+u_width)/w;
            uv.y = uv.y -(1.0-progress)*(u_height+u_pos_y)/h;
            gl_FragColor = texture2D(mSample0, uv);
        }
    }
    else
    {
        float rectToImg_w = w * dToImgScale;
        float rectToImg_h = h * dToImgScale;

        float ToI = gl_FragCoord.x/dToImgScale - w/dToImgScale + w - w*progress + w*progress/dToImgScale;
        float ToJ = gl_FragCoord.y / dToImgScale + h * progress - h * progress / dToImgScale;
        gl_FragColor = texture2D(mSample0, vec2(ToI / w, ToJ / h));
    }
}
// (-(w*s-w)(1-p), w+(w*s-w)*p) (-(h*s/2-h/2), h*s/2+h/2)
void panright()
{
    float w = resolution.x;
    float h = resolution.y;
    vec2 uv = textureCoord;

    if(u_width>0.0 && u_height>0.0)
    {
        if(gl_FragCoord.x >= (1.0-progress)*(w-u_pos_x-u_width)+progress*w)
        {
            gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        }
        else
        {
            uv.x = uv.x +(1.0-progress)*(u_pos_x+u_width)/w;
            gl_FragColor = texture2D(mSample0, uv);
        }
    }
    else
    {
        float rectToImg_w = w * dToImgScale;
        float rectToImg_h = h * dToImgScale;

        float ToI = gl_FragCoord.x/dToImgScale - w/dToImgScale + w - w*progress + w*progress/dToImgScale;
        float ToJ = gl_FragCoord.y/dToImgScale + h/2.0 - h/(2.0*dToImgScale);
        gl_FragColor = texture2D(mSample0, vec2(ToI / w, ToJ / h));
    }
}

// (-(w*s-w)*p, w+(w*s-w)*(1-p)) (-(h*s/2-h/2), h*s/2+h/2)
void panleft()
{
    float w = resolution.x;
    float h = resolution.y;
    vec2 uv = textureCoord;

    if(u_width>0.0 && u_height>0.0)
    {
        if(gl_FragCoord.x <= (1.0-progress)*(w-u_pos_x))
        {
            gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        }
        else
        {
            uv.x = uv.x - (1.0-progress)*(w-u_pos_x)/w;
            gl_FragColor = texture2D(mSample0, uv);
        }
    }
    else
    {
        float rectToImg_w = w * dToImgScale;
        float rectToImg_h = h * dToImgScale;

        float ToI = gl_FragCoord.x/dToImgScale - w*progress/dToImgScale + w*progress;
        float ToJ = gl_FragCoord.y/dToImgScale + h/2.0 - h/(2.0*dToImgScale);
        gl_FragColor = texture2D(mSample0, vec2(ToI / w, ToJ / h));
    }
}
// ( -(h*s-h)*(1-progress), h*s-(h*s-h)*(1-progress)) => gl_FragCoord.y
void panup()
{
    float w = resolution.x;
    float h = resolution.y;
    vec2 uv = textureCoord;

    if(u_width>0.0 && u_height>0.0)
    {
        if(gl_FragCoord.y >= (1.0-progress)*u_pos_y+progress*h)
        {
            gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        }
        else
        {
            //uv.y = uv.y + 0.5;
            uv.y = uv.y +(1.0-progress)*(h-u_pos_y)/h;
            gl_FragColor = texture2D(mSample0, uv);
        }
    }
    else
    {
        float rectToImg_w = w * dToImgScale;
        float rectToImg_h = h * dToImgScale;

        float ToI = gl_FragCoord.x / dToImgScale + w / 2.0 - w / (2.0*dToImgScale);
        float ToJ = (gl_FragCoord.y + (rectToImg_h - h)*(1.0-progress)) / dToImgScale;
        gl_FragColor = texture2D(mSample0, vec2(ToI / w, ToJ / h));
    }
}

void zoom(int bZoomIn)
{
    vec4 color;
    float w = resolution.x;
    float h = resolution.y;

    color=vec4(0,0,0,0);

    //float dToImgScale = 0.5;
    //float dToImgScale = progress / 2.0 + 0.5;
    float dToImgScale;
    if(bZoomIn == 1)
    {
        if(u_width>0.0 && u_height>0.0)
        {
            dToImgScale = progress / 2.0 + 0.5;
        }
        else
        {
            dToImgScale = progress / 2.0 + 1.0;
        }
    }
    else
    {
        dToImgScale = (1.0-progress) / 2.0 + 1.0;
    }
    float rectToImg_w = w * dToImgScale;
    float rectToImg_h = h * dToImgScale;
    float rectToImg_x = (w - rectToImg_w) / 2.;
    float rectToImg_y = (h - rectToImg_h) / 2.;
    if (abs(gl_FragCoord.y - h / 2.) < rectToImg_h / 2. &&
            abs(gl_FragCoord.x - w / 2.) < rectToImg_w / 2.) 
    {
        float ToI = (gl_FragCoord.x - w / 2.) / (dToImgScale) + w / 2.;
        float ToJ = (gl_FragCoord.y - h / 2.) / dToImgScale + h / 2.;
        color = texture2D(mSample0, vec2(ToI / w, ToJ / h));
    }
    gl_FragColor = color;
}
