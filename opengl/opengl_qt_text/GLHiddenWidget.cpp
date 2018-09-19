#include <QTime>
#include <QDebug>
#include "GLHiddenWidget.h"

#include "GLHiddenWidgetGL.cpp"
#include "GLHiddenWidgetText.cpp"

//#define USE_ORI_FBO
GLHiddenWidget::GLHiddenWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent)
    //: GLBaseWidget(format, parent)
{
	setAutoBufferSwap(false);
	doneCurrent();

    idxFrameBuf=0;
    maxFrameBuf=40;


    for(int idx=0; idx<MAX_TEXTURES_CNT; idx++)
    {
        arrTexturesVar[idx].clear();
        arrTexturesVar[idx].append(QString("mSample%1").arg(idx));
    }
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
    makeCurrent();
    qDebug()<<"GLHiddenWidget::initializeGLProgram ParseConfCreateProgram basic overlay... currentContext():"<<context()->currentContext();
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
    blackTexture();
    doneCurrent();
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

void GLHiddenWidget::paintEvent(QPaintEvent * /*event*/)
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
void GLHiddenWidget::showEvent(QShowEvent * /*event*/)
{
    qDebug()<<"GLHiddenWidget::showEvent context: "<<context()<<" isValid: "<<isValid();
    //QGLWidget::showEvent(event);
    setVisible(false);
}
void GLHiddenWidget::createBindFramebufferTexture()
{
    if(!glw || !glh)
    {
        //qInfo()<<"GLHiddenWidget::createBindFramebufferTexture error. glw: "<<glw<<" glh: "<<glh;
        //qDebug()<<"GLHiddenWidget::createBindFramebufferTexture glw: "<<glw<<" glh: "<<glh;
        return;
    }
#if 1
    if(m_bCreateFbo)
    {
        return;
    }
    makeCurrent();
    m_bCreateFbo=true;
#endif
    qDebug()<<"GLHiddenWidget::createBindFramebufferTexture maxFrameBuf: "<<maxFrameBuf<<" glw: "<<glw
        <<" glh: "<<glh<<" maxFrameBuf: "<<maxFrameBuf<<" context: "<<context();
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    for (int ii=0; ii<vFboInfo.size(); ++ii)
    {
#ifdef USE_ORI_FBO
        qDebug()<<"GLHiddenWidget::createBindFramebufferTexture gldelete fb: "<<vFboInfo[ii]->framebuffer;
        glDeleteFramebuffers(1, &vFboInfo[ii]->framebuffer);
        qDebug()<<"GLHiddenWidget::createBindFramebufferTexture gldelete fbtexture: "<<vFboInfo[ii]->fbotexture;
        glDeleteTextures(1, &vFboInfo[ii]->fbotexture);
#else
        qDebug()<<"GLHiddenWidget::createBindFramebufferTexture gldelete fbtexture: "
            <<vFboInfo[ii]->qfbo->texture();
        delete vFboInfo[ii]->qfbo;
        vFboInfo[ii]->qfbo=NULL;
#endif
    }
    vFboInfo.clear();
    idxFrameBuf=0;//usedFboCnt=0;
    for (int ii = 0; ii < maxFrameBuf; ++ii) 
    {
#ifdef USE_ORI_FBO
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
        vFboInfo.push_back(new STFboInfo(ii, fbo, texture, glw, glh));
        qDebug()<<"GLHiddenWidget::createBindFramebufferTexture insert vFboInfo. idx: "<<ii
            <<" fbo: "<<fbo<<" texture: "<<texture;
#else
        vFboInfo.push_back(new STFboInfo(ii, glw, glh));
        STFboInfo* fboinfo=vFboInfo.back();
        //fboinfo.qfbo = new QGLFramebufferObject(glw, glh);
        qDebug()<<"GLHiddenWidget::createBindFramebufferTexture insert vFboInfo. idx: "<<ii
            <<" texture: "<<fboinfo->qfbo->texture();
#endif
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
#ifdef OUTPUT_WASTE
    int dt = startTime.msecsTo(QTime::currentTime());
    qInfo()<<"GLHiddenWidget::createBindFramebufferTexture waste: "<<dt;
#endif
    doneCurrent();
}
#if 0
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
#endif
STFboInfo* GLHiddenWidget::getNewFboInfo()
{
    int fbosize=vFboInfo.size();
    STFboInfo* fboinfo=getRealNewFboInfo();
    if(!fboinfo)
    {
        qInfo()<<"GLHiddenWidget::getNewFboInfo warning. no enough fbo. expand. fbosize: "<<vFboInfo.size();
        expandFramebuffer(glw, glh);
        fboinfo=getRealNewFboInfo();
    }
    if(fboinfo)
    {
        qDebug()<<"GLHiddenWidget::getNewFboInfo idxFrameBuf: "<<fboinfo->idxFrameBuf
            << "framebuffer: "<<fboinfo->framebuffer
            <<" fbotexture: "<<fboinfo->fbotexture<<" qfbotexture: "<<(fboinfo->qfbo?fboinfo->qfbo->texture():0)
            <<" fbosize: "<<vFboInfo.size();
    }
    else
    {
        qInfo()<<"GLHiddenWidget::getNewFboInfo error. no enough fbo. fbosize: "<<vFboInfo.size();
    }
    return fboinfo;
}
STFboInfo* GLHiddenWidget::getRealNewFboInfo()
{
    STFboInfo* fboinfo=NULL;
    int fbosize=vFboInfo.size();
    int iloop=0;
    for(int iloop=0; iloop<fbosize; iloop++)
    {
        if(!vFboInfo[idxFrameBuf%fbosize]->bUsed)
        {
            fboinfo=vFboInfo[idxFrameBuf];
            fboinfo->bUsed=true;
            idxFrameBuf=(idxFrameBuf+1)%fbosize;
            if(!fboinfo->bClean)
            {
                fboinfo->bClean=true;
                qDebug()<<"GLHiddenWidget::getRealNewFboInfo real clean fbo idx: "<<iloop<<" fbotexture: "<<fboinfo->fbotexture;

                baseFragRenderForAllThreadAgain("Basic"
                        , NULL, 0, blackTexture()
                        , 1, 1, 0
                        , fboinfo, false);
            }
            break;
        }
        idxFrameBuf=(idxFrameBuf+1)%fbosize;
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
    if(!vFboInfo[idx]->bUsed)
    {
        qInfo()<<"GLHiddenWidget::getFboInfo error. the fboinfo not use idx: "<<idx;
    }
    return vFboInfo[idx];
}
GLuint GLHiddenWidget::GetTexture(int idx)
{
    STFboInfo* fboinfo=getFboInfo(idx);
    if(fboinfo)
    {
        qDebug()<<"GLHiddenWidget::GetTexture idx: "<<idx<<" fbotexture: "<<fboinfo->fbotexture
            <<" qfbotexture: "<<(fboinfo->qfbo?fboinfo->qfbo->texture():0);
#ifdef USE_ORI_FBO
        return fboinfo->fbotexture;
#else
        return fboinfo->qfbo->texture();
#endif
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
        fboinfo->bClean=false;
        ////fragRenderForFbo(fboinfo); //延迟清除->使用时清除
#endif
    }
    else
    {
        qInfo()<<"GLHiddenWidget::resetFboUsed error. no such fboinfo. idx: "<<idx;
    }
}
int GLHiddenWidget::fragRenderForOtherThreadAgain(const QString& effectname
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2
        , bool useFbo, bool oneFrameFirstgl/*=false*/)
{
    qDebug()<<"GLHiddenWidget::fragRenderForOtherThreadAgain effectname: "<<effectname
        <<" texture: "<<texture<<" texture2: "<<texture2
        <<" useFbo: "<<useFbo<<" oneFrameFirstgl: "<<oneFrameFirstgl
        <<" globaltime: "<<globaltime<<" totaltime: "<<totaltime<<" glw: "<<glw<<" glh: "<<glh;
    STFboInfo* finalFboinfo=NULL;
    if(useFbo)
    {
        finalFboinfo = getNewFboInfo();
        if(!finalFboinfo)
        {
            qInfo()<<"GLHiddenWidget::fragRenderForOtherThreadAgain error. finalFboinfo is null";
            return -1;
        }
    }
    int ret=baseFragRenderForAllThreadAgain(effectname
            , ori_spos , ori_spossize, texture
            , globaltime, totaltime, texture2
            , finalFboinfo, oneFrameFirstgl );
    if(ret<0 && finalFboinfo)
    {
        resetFboUsed(finalFboinfo->idxFrameBuf);
    }
    return ret;
    ///QMutexLocker locker(&m_mutexRender); 
}
int GLHiddenWidget::fragRenderForOtherThreadAgain(const QString& effectname
        , const STFragmentInfo& fragInfo
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2
        , bool useFbo, bool oneFrameFirstgl/*=false*/)
{
    qDebug()<<"GLHiddenWidget::fragRenderForOtherThreadAgain effectname: "<<effectname
        <<" texture: "<<texture<<" texture2: "<<texture2
        <<" useFbo: "<<useFbo<<" oneFrameFirstgl: "<<oneFrameFirstgl
        <<" globaltime: "<<globaltime<<" totaltime: "<<totaltime<<" glw: "<<glw<<" glh: "<<glh;
    STFboInfo* finalFboinfo=NULL;
    if(useFbo)
    {
        finalFboinfo = getNewFboInfo();
        if(!finalFboinfo)
        {
            qInfo()<<"GLHiddenWidget::fragRenderForOtherThreadAgain error. finalFboinfo is null";
            return -1;
        }
    }
    //QMutexLocker locker(&m_mutexRender); 
    int ret= baseFragRenderForAllThreadAgain(effectname, fragInfo
            , ori_spos, ori_spossize, texture, globaltime, totaltime, texture2
            , finalFboinfo, oneFrameFirstgl );
    if(ret<0 && finalFboinfo)
    {
        resetFboUsed(finalFboinfo->idxFrameBuf);
    }
    return ret;
}
void GLHiddenWidget::setGLSize(int width, int height)
{
    qDebug()<<"GLHiddenWidget::setGLSize width: "<<width<<" height: "<<height;
    if(!width || !height)
    {
        qInfo()<<"GLHiddenWidget::setGLSize error. width: "<<width<<" height: "<<height;
        return;
    }
    if(glw==width && glh==height)
    {
        qInfo()<<"GLHiddenWidget::setGLSize error. width: "<<width<<" height: "<<height<<" glw: "
            <<glw<<" glh: "<<glh;
        return;
    }
    if(glw>0 && glh>0)
    {
        qDebug()<<"GLHiddenWidget::setGLSize . already set. glw: "<<glw<<" glh: "<<glh;
        m_bCreateFbo=false;
    }
    glw=width;
    glh=height;
}
void GLHiddenWidget::initialForBackThread()
{
    createBindFramebufferTexture();
}
GLuint GLHiddenWidget::blackTexture()
{
    if(!m_blackTexture)
    {
        if(!m_blackImage)
        {
            m_blackImage = std::shared_ptr<QImage>(new QImage(m_dst_w, m_dst_h, QImage::Format_RGBA8888));
            m_blackImage->fill(QColor(0, 0, 0, 0));
        }
        m_blackTexture=load2DTextureInOut(m_dst_w, m_dst_h, m_blackImage->bits());
    }
    return m_blackTexture;
}
void GLHiddenWidget::expandFramebuffer(int glw, int glh)
{
    maxFrameBuf = 2*maxFrameBuf;
    qDebug()<<"GLHiddenWidget::expandFramebuffer glw: "<<glw<<" glh: "<<glh<<" maxFrameBuf: "<<maxFrameBuf;
    for (int ii=vFboInfo.size(); ii<maxFrameBuf; ++ii)
    {
#ifdef USE_ORI_FBO
        GLuint texture = createAndSetupTexture();
        //fbotextures.push_back(texture);

        //设置纹理大小和图像大小一致
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glw, glh, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        // 创建一个帧缓冲
        // https://www.khronos.org/opengl/wiki/Framebuffer_Object
        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        qDebug()<<"GLHiddenWidget::expandFramebuffer glcreate fbo: "<<fbo;
        //framebuffers.push_back(fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // 绑定纹理到帧缓冲
        //     gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, texture, 0);
        //glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
        //glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, texture, 0);
        vFboInfo.push_back(new STFboInfo(ii, fbo, texture, glw, glh));
        qDebug()<<"GLHiddenWidget::expandFramebuffer insert vFboInfo. idx: "<<ii
            <<" fbo: "<<fbo<<" texture: "<<texture;
#else
        vFboInfo.push_back(new STFboInfo(ii, glw, glh));
        STFboInfo* fboinfo=vFboInfo.back();
        //fboinfo.qfbo = new QGLFramebufferObject(glw, glh);
        qDebug()<<"GLHiddenWidget::expandFramebuffer insert vFboInfo. idx: "<<ii
            <<" texture: "<<fboinfo->qfbo->texture();
#endif
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}
