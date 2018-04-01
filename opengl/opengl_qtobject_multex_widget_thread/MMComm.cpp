#include <QDebug>
#include "MMComm.h"

MMGlobalContext* MMGlobalContext::m_pInstance = NULL;
MMGlobalContext* MMGlobalContext::instance()  
{  
    if(m_pInstance == NULL)  //判断是否第一次调用  
        m_pInstance = new MMGlobalContext();  
    return m_pInstance;  
}  
MMGlobalContext::MMGlobalContext()
{
    initializeOpenGLFunctions();
}

void MMGlobalContext::initialOpengl(int width, int height,bool bForce)
{
    qDebug()<<"MMGlobalContext::initialOpengl width: "<<width<<" height: "<<height;
    if(glw==width && glw==height)
    {
        return;
    }

    glw=width;
    glh=height;

}
void MMGlobalContext::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "vertex.frag"))
    {
        qInfo()<<"error vertex";
    }

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "Basic.frag"))
    {
        qInfo()<<"error Basic.frag";
    }

    // Link shader pipeline
    if (!program.link())
    {
        qInfo()<<"error link";
    }

    if (!program1977.addShaderFromSourceFile(QOpenGLShader::Vertex, "vertex.frag"))
    {
        qInfo()<<"error vertex";
    }
    if (!program1977.addShaderFromSourceFile(QOpenGLShader::Fragment, "1977.frag"))
    {
        qInfo()<<"error 1977.frag";
    }
    if (!program1977.link())
    {
        qInfo()<<"error link";
    }
}
void MMGlobalContext::initTextures()
{
    // Load cube.png image
    //texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());
    texture = new QOpenGLTexture(QImage("cube.png"));

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}

int MMGlobalContext::fragRenderForOtherThreadAgain()
{
    //VAO
    GLuint        pos_buf=0, tex_coord_buf=0;
    float w=glw, h=glh;
    bool oneFrameFirstgl=true;

    const float coordpos[12] = {
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
    float static_spos[12] = { 
        0.0f, h,
        w, h,
        0.0f, 0.0f,
        0.0f, 0.0f,
        w, h,
        w, 0.0f
    };
    float* spos;
    int spossize;
    /*if(ori_spos && ori_spossize>0)
    {
        spos=ori_spos;
        spossize=ori_spossize;
    }
    else*/
    {
        spos=static_spos;
        spossize=sizeof(static_spos);
    }
    if (!program.bind())
    {
        qInfo()<<"error program.bind";
    }
    program.setUniformValue("u_first", oneFrameFirstgl?1.0f:0.0);
    program.setUniformValue("u_resolution", w, h);

    //VAO
    {
    glGenBuffers(1, &pos_buf);
    qDebug()<<"MMGlobalContext::fragRenderForOtherThreadAgain glcreate pos_buf: "<<pos_buf;
    glBindBuffer(GL_ARRAY_BUFFER, pos_buf);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(spos), spos, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, spossize, spos, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    // Step4: 指定解析方式  并启用顶点属性
    // 顶点位置属性
    GLint loc = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    {
    glGenBuffers(1, &tex_coord_buf);
    qDebug()<<"MMGlobalContext::fragRenderForOtherThreadAgain glcreate tex_coord_buf: "<<tex_coord_buf;
    glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordpos), coordpos, GL_STATIC_DRAW);

    // Step4: 指定解析方式  并启用顶点属性
    // 顶点位置属性
    GLint loc = glGetAttribLocation(program, "a_texCoord");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    program.release();
    return 0;
}
