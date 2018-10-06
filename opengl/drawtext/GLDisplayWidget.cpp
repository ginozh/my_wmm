#include <QTime>
#include <QJsonDocument>
#include <QJsonObject> 
#include <QThread> 
#include <QPushButton>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QDebug>
#include "GLDisplayWidget.h"
#include "GLHiddenWidget.h"
#if 0
#include "Frame.h"
#include "FFmpegWriter.h"
#include "ExporterPrivate.h"
#endif
GLDisplayWidget::GLDisplayWidget(QGLFormat& format, GLHiddenWidget *shareWidget, QWidget *parent,bool bExport)
    : QGLWidget(format, parent, shareWidget)
    //: GLBaseWidget(format, parent, shareWidget) -> setShareGLContext
    //: GLBaseWidget(format, parent)
    , m_format(format)
    , m_shareWidget(shareWidget)
    , m_bExport(bExport)
{
    if(!shareWidget)
    {
        qInfo()<<"GLDisplayWidget::GLDisplayWidget shareWidget is null";
    }
	/////setAutoBufferSwap(false); //storm nook? 
#if 0
    QBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    {
        QGraphicsScene* scene = new QGraphicsScene(this);
        QGraphicsView* graphicsView = new QGraphicsView(scene);
        layout->addWidget(graphicsView);
        int iSceneWidth = 800/2;
        int iSceneHeight = 600/2;
        int iHeightIdx = 0;
        scene->setSceneRect(0,0,iSceneWidth,iSceneHeight); //如果没有这个，可能宽度会变成764
        graphicsView->setSceneRect(0,0,iSceneWidth,iSceneHeight);
        //graphicsView->setGeometry(QRect(0, 0, iFrameWidth, 500*dFactorY));
        //graphicsView->setFixedSize(iFrameWidth, 500*dFactorY);
    }
    setLayout(layout);
#endif
	doneCurrent();
#if 0
    m_pbPreFrame = new QPushButton(this);
    m_pbPreFrame->setText("0");
#endif
}
GLDisplayWidget::~GLDisplayWidget()
{
    qDebug()<<"GLDisplayWidget::~GLDisplayWidget";
}
void GLDisplayWidget::initializeGL()
{
    qDebug()<<"GLDisplayWidget::initializeGL context: "<<context() <<" isValid: "<<isValid();
    initializeGLFunctions();
}
void GLDisplayWidget::resizeGL(int width, int height)
{
    qDebug()<<"GLDisplayWidget::resizeGL width: "<<width<<" height: "<<height<<" context: "<<context() <<" isValid: "<<isValid();
    m_width=width;
    m_height=height;
    QGLWidget::resizeGL(width, height);
}
void GLDisplayWidget::paintGL()
{
    qDebug()<<"GLDisplayWidget::paintGL displayTexture: "<<displayTexture<<" m_width: "
        <<m_width<<" m_height: "<<m_height;
    makeCurrentOut();
    if(m_width && m_height) glViewport(0, 0, m_width, m_height);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    if(displayTexture>0)
    {
        qDebug()<<"GLDisplayWidget::paintGL display";
        m_shareWidget->display();
        ///swapBuffers();  //storm
        ///displayTexture=0;
    }
    doneCurrentOut();
}
#if 0
void GLDisplayWidget::paintGL()
{
    GLuint displayTexture=1;
#if 0
    std::shared_ptr<Frame> tmpframe=frame;
    if(!tmpframe || !m_shareWidget)
    {
        qDebug()<<"GLWidget::paintGL tmpframe is null or m_shareWidget is null";
        //displayTexture=testTexture;
        return;
    }
    else
    {
        displayTexture = m_shareWidget->GetTexture(tmpframe->GetFboIdx());
    }
#endif
    if(displayTexture <=0)
    {
        qDebug()<<"GLWidget::paintGL displayTexture is 0";
        return;
    }
#if 0
    glFinish();
    QPainter painter;
    QRect target(0,0,m_width, m_height);
    painter.beginNativePainting();
    drawTexture(target, displayTexture);
    painter.endNativePainting();
    return;
#endif
    ////qDebug()<<"GLDisplayWidget::paintGL request: "<<(tmpframe?tmpframe->number:-1)<<" displayTexture: "<<displayTexture;
    //QMutexLocker locker(&m_shareWidget->m_mutexRender); 
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    //GLuint fbo=bindTexture(displayTexture);
#if 0
#if 0
    glFinish();
#else
    glFlush();
    char tmp[10];
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)tmp);
#endif
#endif
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
#if 0
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDepthRange (0.0, 1.0);
#endif
    int ret;
    ret=m_shareWidget->baseFragRenderForAllThreadAgain("Basic"
            , NULL, 0, displayTexture
            , 1, 1, 0
            , NULL, false);
    if(ret!=0)
    {
        qInfo()<<"GLDisplayWidget::paintGL error ret: "<<ret;
    }
    else
    {
#if 0
#if 1
        glFinish();
#else
        glFlush();
        char tmp[10];
        glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)tmp);
#endif
#endif
        swapBuffers();
#if 0
        //if(fw)
        if(ep)
        {
            image=std::shared_ptr<QImage>(new QImage(grabFrameBuffer()));
            if(image->format()!=QImage::Format_RGBA8888)
            {
                image = std::shared_ptr<QImage>(new QImage(image->convertToFormat(QImage::Format_RGBA8888)));
            }
            qDebug()<<"GLWidget::paintGL image width: "<<image->width()<<" height: "<<image->height();
        }
#endif
    }
    emit sigGLPainted();
#if 0
    //if(fw)
    if(ep)
    {
        tmpframe.reset();
        frame.reset();  //for min maxshow
        ep->GLPainted();
    }
#endif
#if 0
    tmpframe.reset();
    frame.reset();  //for min maxshow
#endif
#ifdef OUTPUT_WASTE
    int alldt = startTime.msecsTo(QTime::currentTime());
    qInfo()<<"GLDisplayWidget::paintGL alldt: "<<alldt;
#endif

#if 0
    static qint64 icnt=0;
    m_pbPreFrame->setText(QString::number(++icnt));
#endif
}
#endif
#if 0
void GLDisplayWidget::resizeEvent(QResizeEvent *evt)
{
    qDebug()<<"GLDisplayWidget::resizeEvent evt: "<<evt<<" context: "<<context() <<" isValid: "<<isValid();
    QGLWidget::resizeEvent(evt);
}
#endif
void GLDisplayWidget::showEvent(QShowEvent *event)
{
    qDebug()<<"GLDisplayWidget::showEvent evt: "<<event<<" context: "<<context() <<" isValid: "<<isValid();
    QGLWidget::showEvent(event);
}
void GLDisplayWidget::closeEvent(QCloseEvent *evt)
{
    qDebug()<<"GLDisplayWidget::closeEvent evt: "<<evt<<"  context: "<<context() <<" isValid: "<<isValid();
    QGLWidget::closeEvent(evt);
}
void GLDisplayWidget::setShareGLContext()
{
    qDebug()<<"GLDisplayWidget::setShareGLContext m_glContext: "<<m_glContext;
    if(m_glContext)
    {
        qInfo()<<"GLDisplayWidget::setShareGLContext error. allready set. m_glContext: "<<m_glContext;
        return;
    }
	if(!m_shareWidget || !m_shareWidget->context())
    {
        qInfo()<<"GLDisplayWidget::setShareGLContext error. m_shareWidget: "<<m_shareWidget<<" m_shareWidget->context: "<<(m_shareWidget?m_shareWidget->context():NULL);
        return;
    }
    
	//create context from inside thread
	m_glContext=new QGLContext(m_format, this);

	//share context with another QGLWidget
    qDebug()<<"GLDisplayWidget::setShareGLContext m_shareWidget->context: "<<m_shareWidget->context()<<" m_glContext: "<<m_glContext;
#if 0
    if(m_playerprivate)
        m_playerprivate->setGLWidget(this);
#endif

    m_glContext->create(m_shareWidget->context());
	setContext(m_glContext);
}
void GLDisplayWidget::initializeGLResource(int width, int height,bool bForce)
{
    qDebug()<<"GLDisplayWidget::initializeGLResource width: "<<width<<" height: "<<height<<" bForce: "<<bForce;
}
#if 1
void GLDisplayWidget::paintEvent(QPaintEvent *event)
{
    qDebug()<<"GLDisplayWidget::paintEvent";
    QGLWidget::paintEvent(event);
}
#endif
#if 1
void GLDisplayWidget::glInit()
{
    qDebug()<<"GLDisplayWidget::glInit";
    QGLWidget::glInit();
}

void GLDisplayWidget::glDraw()
{
    qDebug()<<"GLDisplayWidget::glDraw";
    QGLWidget::glDraw();
}
#endif
void GLDisplayWidget::setGLSize(int width, int height)
{
    if(!m_shareWidget)
    {
        qInfo()<<"GLDisplayWidget::setGLSize error. m_shareWidget is null";
        return;
    }
    //GLBaseWidget::setGLSize(width, height);
    m_shareWidget->setGLSize(width, height);
}
int GLDisplayWidget::makeCurrentOut()
{
    if(QThread::currentThread() == thread())
    {
        qDebug()<<"GLDisplayWidget::makeCurrentOut currentThread: "<<QThread::currentThread()
            <<" GLDisplayWidget thread: "<<thread();
        makeCurrent();
    }
    else if(m_shareWidget && QThread::currentThread() == m_shareWidget->getContextThread())
    {
        qDebug()<<"GLDisplayWidget::makeCurrentOut currentThread: "<<QThread::currentThread()
            <<" m_shareWidget thread: "<<m_shareWidget->getContextThread();
        m_shareWidget->makeCurrent();
    }
    else
    {
        qInfo()<<"GLDisplayWidget::makeCurrentOut error. no glwidget. currentThread: "
            <<QThread::currentThread()<<" GLDisplayWidget_thread: "<<thread()<<" hiddencontex_thread: "
            <<(m_shareWidget?m_shareWidget->getContextThread():NULL);
        return -1;
    }
#if 0
    if(QThread::currentThread() == context()->thread())
    {
        qDebug()<<"GLDisplayWidget::makeCurrentOut currentThread: "<<QThread::currentThread()
            <<" GLDisplayWidget thread: "<<context()->thread();
        makeCurrent();
    }
    else if(m_shareWidget && QThread::currentThread() == m_shareWidget->context()->thread())
    {
        qDebug()<<"GLDisplayWidget::makeCurrentOut currentThread: "<<QThread::currentThread()
            <<" m_shareWidget thread: "<<m_shareWidget->context()->thread();
        m_shareWidget->makeCurrent();
    }
    else
    {
        qInfo()<<"GLDisplayWidget::makeCurrentOut error. no glwidget. currentThread: "<<QThread::currentThread();
        return -1;
    }
#endif
    return 0;
}
int GLDisplayWidget::doneCurrentOut()
{
    if(QThread::currentThread() == thread())
    {
        qDebug()<<"GLDisplayWidget::doneCurrentOut currentThread: "<<QThread::currentThread()
            <<" glwidget thread: "<<thread();
        doneCurrent();
    }
    else if(QThread::currentThread() == m_shareWidget->getContextThread())
    {
        qDebug()<<"GLDisplayWidget::doneCurrentOut currentThread: "<<QThread::currentThread()
            <<" m_shareWidget thread: "<<m_shareWidget->getContextThread();
        m_shareWidget->doneCurrent();
    }
    else
    {
        qInfo()<<"GLDisplayWidget::doneCurrentOut error. no glwidget. currentThread: "<<QThread::currentThread();
        return -1;
    }
    return 0;
}
std::shared_ptr<QImage> GLDisplayWidget::getImage()
{
    return image;
}
std::shared_ptr<QImage> GLDisplayWidget::getCapture()
{
    std::shared_ptr<QImage> img;
    if(!frame || !m_shareWidget)
    {
        return img;
    }
#if 0
    STFboInfo* fboinfo=m_shareWidget->getFboInfo(frame->GetFboIdx());
    if(fboinfo && fboinfo->bUsed && fboinfo->qfbo)
    {
        makeCurrent();
        //image = std::shared_ptr<QImage>(new QImage(fboinfo->qfbo->toImage()));
        img = std::shared_ptr<QImage>(new QImage(fboinfo->qfbo->toImage().convertToFormat(QImage::Format_RGBA8888)));
        doneCurrent();
    }
#endif
    return img;
}
#if 0
GLuint GLDisplayWidget::bindTexture(GLuint texture)
{
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
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return fbo;
}
#endif
