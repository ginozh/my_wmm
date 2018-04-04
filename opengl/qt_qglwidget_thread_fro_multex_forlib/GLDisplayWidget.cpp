#include <QTime>
#include <QJsonDocument>
#include <QJsonObject> 
#include <QThread> 
#include <QPushButton>
#include <QDebug>
#include "GLDisplayWidget.h"
#include "GLHiddenWidget.h"

GLDisplayWidget::GLDisplayWidget(QGLFormat format, GLHiddenWidget *shareWidget, QWidget *parent)
    //: QGLWidget(format, parent, shareWidget)
    //: GLWidget(format, parent, shareWidget) -> setShareGLContext
    : GLWidget(format, parent)
    , m_format(format)
    , m_shareWidget(shareWidget)
{
    qDebug()<<"GLDisplayWidget::GLDisplayWidget";
	setAutoBufferSwap(false);

	doneCurrent();
    m_pbPreFrame = new QPushButton(this);
    m_pbPreFrame->setText("0");
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
    qDebug()<<"GLDisplayWidget::paintGL context: "<<context() <<" isValid: "<<isValid();
    qDebug()<<"GLWidget::paintGL start lock";
    QMutexLocker locker(&m_mutexRender); 
    qDebug()<<"GLWidget::paintGL get lock";
    //QGLWidget::paintGL();
    if(m_idxFbo<0 && m_texture<=0)
    {
        qInfo()<<"GLWidget::paintGL error. m_idxFbo: "<<m_idxFbo<<" m_texture: "<<m_texture;
        //QGLWidget::paintGL();
        return;
    }
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDepthRange (0.0, 1.0);
    int idxFbo=-1;
    //fbo to glwidget
    {
        GLuint displayTexture;
        if(m_texture>0)
        {
            displayTexture=m_texture;
        }
        else
        {
            ////m_shareWidget->getFboInfo(m_idxFbo)->qfbo->toImage();
            //m_shareWidget->getFboInfo(m_idxFbo)->qfbo->handle();
#if 1
            glFinish();
#else
            glFlush();
            char tmp[10];
            glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)tmp);
#endif
            displayTexture=m_shareWidget->GetTexture(m_idxFbo);
        }
        if(displayTexture<=0)
        {
            qInfo()<<"GLWidget::paintGL error. displayTexture: "<<displayTexture;
        }
#if 0
        {
            QImage image;
            QString fileName;
            fileName="1.jpg";
            image.load(fileName);
            if (image.isNull()) {
                qDebug()<<"error fileName: "<<fileName;
            }
            image = image.convertToFormat(QImage::Format_RGBA8888);
            displayTexture=load2DTexture(image.width(), image.height(), image.bits());
            if(displayTexture<=0)
            {
                qInfo()<<"load2DTexture error";
            }
        }
#endif

    qDebug()<<"GLWidget::paintGL get2 lock";
        idxFbo=fragRenderForOtherThreadAgain("Basic"
                , NULL, 0, displayTexture
                , 1, 1, 0
                , NULL, false, m_width, m_height);
        if(idxFbo!=0)
        {
            qInfo()<<"GLWidget::paintGL error ret: "<<idxFbo;
        }
        else
        {
            //glFlush();
            swapBuffers();
            //QThread::msleep(10);
        }
    }
#ifdef OUTPUT_WASTE
    int alldt = startTime.msecsTo(QTime::currentTime());
    qDebug()<<"GLWidget::paintGL alldt: "<<alldt;
#endif
    if(m_texture>0)
    {
        //deleteTexture(m_texture);
    }
    else
    {
        //m_shareWidget->
        //displayTexture=m_shareWidget->GetTexture(m_idxFbo);
    }
    m_idxFbo=-1;
    m_texture=-1;
    qDebug()<<"GLWidget::paintGL end lock";
    static qint64 icnt=0;
    m_pbPreFrame->setText(QString::number(++icnt));
}
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
    qDebug()<<"GLDisplayWidget::initializeGLResource width: "<<width<<" height: "<<height;
}
#if 0
void GLDisplayWidget::paintEvent(QPaintEvent *)
{
    qDebug()<<"GLDisplayWidget::paintEvent";
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
    GLWidget::setGLSize(width, height);
    m_shareWidget->setGLSize(width, height);
}
