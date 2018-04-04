#include <QTime>
#include <QDebug>
#include "GLHiddenWidget.h"

GLHiddenWidget::GLHiddenWidget(QGLFormat format, QWidget *parent)
    //: QGLWidget(format, parent)
    : GLWidget(format, parent)
{
	setAutoBufferSwap(false);
	doneCurrent();

    idxFrameBuf=0;
    maxFrameBuf=30;
}

GLHiddenWidget::~GLHiddenWidget()
{
    qDebug()<<"GLHiddenWidget::~GLHiddenWidget";
}
#if 0
void GLHiddenWidget::glInit()
{
    qDebug()<<"GLHiddenWidget::glInit";
}

void GLHiddenWidget::glDraw()
{
    qDebug()<<"GLHiddenWidget::glDraw";
}
#endif
void GLHiddenWidget::initializeGLProgram()
{
    qDebug()<<"GLHiddenWidget::initializeGLProgram ParseConfCreateProgram basic overlay...";
    if(ParseConfCreateProgram(":/Effects/Shader/","Basic")!=0
            || ParseConfCreateProgram(":/Effects/Shader/","Overlay")!=0
            //|| ParseConfCreateProgram(":/Effects/Shader/","TiltShift")!=0
            || ParseConfCreateProgram(":/Effects/Shader/","LightRoom")!=0
            || ParseConfCreateProgram(":/Effects/Shader/","Background")!=0 
            || ParseConfCreateProgram(":/Effects/Shader/","FrameMap")!=0
            || ParseConfCreateProgram(":/Effects/Shader/","SaveImage")!=0
      )
    {
        qInfo()<<"MMGlobalContext::initialOpengl error. createProgram wrong. effect: Basic, Overlay...";
    }
}
void GLHiddenWidget::initializeGL()
{
    qDebug()<<"GLHiddenWidget::initializeGL context: "<<context()
         <<" isValid: "<<isValid();
    initializeGLFunctions();

}

void GLHiddenWidget::resizeGL(int width, int height)
{
    qDebug()<<"GLHiddenWidget::resizeGL width: "<<width<<" height: "<<height<<"  context: "<<context()<<" isValid: "<<isValid();
}

void GLHiddenWidget::paintGL()
{
    qDebug()<<"GLHiddenWidget::paintGL context: "<<context()<<" isValid: "<<isValid();
}

void GLHiddenWidget::paintEvent(QPaintEvent *event)
{
    qDebug()<<"GLHiddenWidget::paintEvent context: "<<context()<<" isValid: "<<isValid();
    ////QGLWidget::paintEvent(event);
}
#if 0
void GLHiddenWidget::resizeEvent(QResizeEvent *event)
{
    qDebug()<<"GLHiddenWidget::resizeEvent context: "<<context()<<" isValid: "<<isValid();
    QGLWidget::resizeEvent(event);
}
#endif
void GLHiddenWidget::showEvent(QShowEvent *event)
{
    qDebug()<<"GLHiddenWidget::showEvent context: "<<context()<<" isValid: "<<isValid();
    //QGLWidget::showEvent(event);
    setVisible(false);
}
void GLHiddenWidget::createBindFramebufferTexture()
{
    if(!glw || !glh)
    {
        qInfo()<<"GLHiddenWidget::createBindFramebufferTexture error. glw: "<<glw<<" glh: "<<glh;
        return;
    }
    qDebug()<<"GLHiddenWidget::createBindFramebufferTexture maxFrameBuf: "<<maxFrameBuf<<" glw: "<<glw
        <<" glh: "<<glh<<" maxFrameBuf: "<<maxFrameBuf<<" context: "<<context();
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    for (int ii=0; ii<vFboInfo.size(); ++ii)
    {
        qDebug()<<"GLHiddenWidget::createBindFramebufferTexture gldelete fb: "<<vFboInfo[ii].framebuffer;
        glDeleteFramebuffers(1, &vFboInfo[ii].framebuffer);
        qDebug()<<"GLHiddenWidget::createBindFramebufferTexture gldelete fbtexture: "<<vFboInfo[ii].fbotexture;
        glDeleteTextures(1, &vFboInfo[ii].fbotexture);
        delete vFboInfo[ii].qfbo;
    }
    vFboInfo.clear();
    idxFrameBuf=0;//usedFboCnt=0;
    for (int ii = 0; ii < maxFrameBuf; ++ii) 
    {
        GLuint texture = createAndSetupTexture();
        //fbotextures.push_back(texture);

        //设置纹理大小和图像大小一致
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glw, glh, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        // 创建一个帧缓冲
        // https://www.khronos.org/opengl/wiki/Framebuffer_Object
        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        qDebug()<<"GLHiddenWidget::createBindFramebufferTexture glcreate fbo: "<<fbo;
        //framebuffers.push_back(fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // 绑定纹理到帧缓冲
        //     gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, texture, 0);
        //glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
        //glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, texture, 0);

        vFboInfo.push_back(STFboInfo(ii, fbo, texture, glw, glh));
        STFboInfo& fboinfo=vFboInfo.back();
        fboinfo.qfbo = new QGLFramebufferObject(glw, glh);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        qDebug()<<"GLHiddenWidget::createBindFramebufferTexture insert vFboInfo. idx: "<<ii
            <<" fbo: "<<fbo<<" texture: "<<texture;
    }
#ifdef OUTPUT_WASTE
    int dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<<"GLHiddenWidget::createBindFramebufferTexture waste: "<<dt;
#endif
    //doneCurrent();
}
void GLHiddenWidget::clearFboBuffer()
{
    qDebug()<<"GLHiddenWidget::clearFboBuffer";
    //createBindFramebufferTexture(m_gs->w, m_gs->h);
#if 1
    QTime startTime = QTime::currentTime();
    for (int ii=0; ii<vFboInfo.size(); ++ii)
    {
        if(vFboInfo[ii].bUsed)
        {
            vFboInfo[ii].bUsed=false;
            fragRenderForFbo(&vFboInfo[ii]);
            //fragRenderForFbo(&vFboInfo[ii]);
            //1, copy texture
            //2, frag color(0, 0, 0, 1)
        }
    }
    //usedFboCnt=0;
    idxFrameBuf=0;
    int dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<<"GLHiddenWidget::clearFboBuffer waste: "<<dt;
#endif
}
int GLHiddenWidget::fragRenderForFbo(STFboInfo* finalFboinfo)
{
    if(!finalFboinfo)
    {
        qInfo()<<"GLHiddenWidget::fragRenderForFbo error finalFboinfo is NULL";
        return -1;
    }
    const QString effectname="Background";
    QMap<QString/*effectname*/, STFragmentInfo >& m_mapEffectProgram=MMGlobalContext::instance()->m_mapEffectProgram;
    if(!m_mapEffectProgram.contains(effectname))
    {
        qInfo()<<"GLHiddenWidget::fragRenderForFbo error. no such effect: "<<effectname;
        return -2;
    }
    //makeCurrent();
    STFragmentInfo& fragInfo=m_mapEffectProgram[effectname];

    QTime startTime = QTime::currentTime();
    int dt, dtall;
    int iRtn=0;
    GLuint        program;
    GLint status;
    //GLuint fbo ;
    //VAO
    GLuint        pos_buf=0, tex_coord_buf=0;
    float w=glw, h=glh;
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
    {
        spos=static_spos;
        spossize=sizeof(static_spos);
    }
    bool bound;
    program=fragInfo.program;
    if (!glIsProgram(program)) {
        qInfo()<<"GLHiddenWidget::fragRenderForFbo error program is not program"<< " effectname:"
            <<effectname;
        iRtn=-2;
        goto END_fragRenderForOtherThread;
    }
#if 0
    if (!glIsFramebuffer(finalFboinfo->framebuffer))
    {
        qInfo()<<"GLHiddenWidget::fragRenderForFbo error framebuffer is not fbo effectname:"
            <<effectname<<" framebuffer: "<<finalFboinfo->framebuffer;
        iRtn=-3;
        goto END_fragRenderForOtherThread;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, finalFboinfo->framebuffer);
#else
    bound=finalFboinfo->qfbo->bind();
	if(!bound)
    {
        qInfo()<<"GLHiddenWidget::fragRenderForFbo error. bind fail";
        iRtn=-3;
        goto END_fragRenderForOtherThread;
    }
#endif

    glViewport(0, 0, glw, glh);
    glUseProgram(program);
    glUniform2f(glGetUniformLocation(program, "u_resolution"), glw, glh);
    //VAO
    {
    glGenBuffers(1, &pos_buf);
    qDebug()<<"GLHiddenWidget::fragRenderForFbo glcreate pos_buf: "<<pos_buf;
    glBindBuffer(GL_ARRAY_BUFFER, pos_buf);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(spos), spos, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, spossize, spos, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    // Step4: 指定解析方式  并启用顶点属性
    // 顶点位置属性
    GLint loc = glGetAttribLocation(program, "position");
    qDebug()<<"GLHiddenWidget::fragRenderForFbo position loc: "<<loc;
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    {
    glGenBuffers(1, &tex_coord_buf);
    qDebug()<<"GLHiddenWidget::fragRenderForFbo glcreate tex_coord_buf: "<<tex_coord_buf;
    glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordpos), coordpos, GL_STATIC_DRAW);

    // Step4: 指定解析方式  并启用顶点属性
    // 顶点位置属性
    GLint loc = glGetAttribLocation(program, "a_texCoord");
    qDebug()<<"GLHiddenWidget::fragRenderForFbo a_texCoord loc: "<<loc;
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    qDebug()<< "GLHiddenWidget::fragRenderForFbo start draw";
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
    dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<< "GLHiddenWidget::fragRenderForFbo waste_time frag. time: " << dt
        <<"idxFrameBuf: "<<(finalFboinfo?finalFboinfo->idxFrameBuf:0);
END_fragRenderForOtherThread:
#if 1
    if(pos_buf) {
        qDebug()<<"GLHiddenWidget::fragRenderForFbo gldelete pos_buf: "<<pos_buf;
        glDeleteBuffers(1, &pos_buf);
    }
    if(tex_coord_buf) {
        qDebug()<<"GLHiddenWidget::fragRenderForFbo gldelete tex_coord_buf: "<<tex_coord_buf;
        glDeleteBuffers(1, &tex_coord_buf);
    }
#endif
    //doneCurrent();
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    finalFboinfo->qfbo->bindDefault();
    return (!iRtn && finalFboinfo)?finalFboinfo->idxFrameBuf:iRtn;
}
STFboInfo* GLHiddenWidget::getNewFboInfo()
{
    STFboInfo* fboinfo=NULL;
    int fbosize=vFboInfo.size();
#if 0
    if(usedFboCnt>=fbosize)
    {
        qInfo()<<"GLHiddenWidget::getNewFboInfo error. usedFboCnt: "<<usedFboCnt
            <<" greate vFboInfo.size: "<<fbosize;
        return fboinfo;
    }
#endif
    int iloop=0;
    for(int iloop=0; iloop<fbosize; iloop++)
    {
        if(!vFboInfo[idxFrameBuf%fbosize].bUsed)
        {
            fboinfo=&vFboInfo[idxFrameBuf];
            fboinfo->bUsed=true;
            idxFrameBuf=(idxFrameBuf+1)%fbosize;
            break;
        }
        idxFrameBuf=(idxFrameBuf+1)%fbosize;
    }
    if(fboinfo)
    {
        qDebug()<<"GLHiddenWidget::getNewFboInfo idxFrameBuf: "<<fboinfo->idxFrameBuf
            << "framebuffer: "<<fboinfo->framebuffer
            <<" fbotexture: "<<fboinfo->fbotexture<<" qfbotexture: "<<fboinfo->qfbo->texture()
            <<" fbosize: "<<fbosize;
    }
    else
    {
        qInfo()<<"GLHiddenWidget::getNewFboInfo error. no enough fbo. iloop: "<<iloop
            <<" fbosize: "<<fbosize;
        //expandFramebuffer(glw, glh);
    }
    return fboinfo;
}
STFboInfo* GLHiddenWidget::getFboInfo(int idx)
{
    int fbosize=vFboInfo.size();
    if(idx>=fbosize || idx<0)
    {
        qInfo()<<"GLHiddenWidget::getFboInfo error. idx: "<<idx
            <<" greate vFboInfo.size: "<<fbosize;
        return NULL;
    }
    if(!vFboInfo[idx].bUsed)
    {
        qInfo()<<"GLHiddenWidget::getFboInfo error. the fboinfo not use idx: "<<idx;
    }
    return &vFboInfo[idx];
}
GLuint GLHiddenWidget::GetTexture(int idx)
{
    STFboInfo* fboinfo=getFboInfo(idx);
    if(fboinfo)
    {
        qDebug()<<"GLHiddenWidget::GetTexture idx: "<<idx<<" fbotexture: "<<fboinfo->fbotexture
            <<" qfbotexture: "<<fboinfo->qfbo->texture();
        //return fboinfo->fbotexture;
        return fboinfo->qfbo->texture();
    }
    else
    {
        qInfo()<<"GLHiddenWidget::GetTexture error. no such fboinfo. idx: "<<idx;
    }
    return 0;
}
void GLHiddenWidget::resetFboUsed(int idx)
{
    STFboInfo* fboinfo=getFboInfo(idx);
    if(fboinfo)
    {
        qDebug()<<"GLHiddenWidget::resetFboUsed idx: "<<idx<<" fbotexture: "<<fboinfo->fbotexture;
#if 1
        fboinfo->bUsed=false;
        fragRenderForFbo(fboinfo);
#endif
    }
    else
    {
        qInfo()<<"GLHiddenWidget::resetFboUsed error. no such fboinfo. idx: "<<idx;
    }
}
