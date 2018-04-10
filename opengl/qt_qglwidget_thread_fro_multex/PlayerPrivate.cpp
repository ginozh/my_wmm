#include <QImage>
#include <QDebug>
#include "PlayerPrivate.h"
#include "GLWidget.h"
PlayerPrivate::PlayerPrivate(QObject *parent)
    :QThread(parent)
{
}
PlayerPrivate::~PlayerPrivate()
{
}
void PlayerPrivate::setGLWidget(GLWidget* glw)
{
    qDebug()<<"PlayerPrivate::setGLWidget moveToThread";
    if(glwidget)
        return;
    glwidget=glw;
    glwidget->m_shareWidget->context()->moveToThread(this);
    qDebug()<<"PlayerPrivate::setGLWidget moveToThread 2";
}
void PlayerPrivate::run()
{
    int idx=0;
    int glw=1280;
    int glh=720;
    glwidget->m_shareWidget->initialOpengl(glw, glh);
    for (;;) 
    {
        double frameTime = 100;
        if(!m_bstart)
        {
            QThread::msleep(frameTime);
            continue;
        }
        ++idx;

        int idxFbo=-1;
        GLuint texture=-1;
        {
            QImage image;
            QString fileName;
            if(idx%2==0)
            {
                //fileName="c:\\shareproject\\jpg\\img006.jpg";
                //fileName="c:\\shareproject\\jpg\\1.jpg";
                fileName="1.jpg";
            }
            else
            {
                //fileName="c:\\shareproject\\jpg\\img007.jpg";
                //fileName="c:\\shareproject\\jpg\\img006.jpg";
                fileName="1.jpg";
            }
            image.load(fileName);
            if (image.isNull()) {
                qDebug()<<"error fileName: "<<fileName;
            }
            image = image.convertToFormat(QImage::Format_RGBA8888);

            {

            qDebug()<<"PlayerPrivate::run start lock";
            QMutexLocker locker(&glwidget->m_mutexRender); 
            qDebug()<<"PlayerPrivate::run get lock";
            glwidget->m_shareWidget->context()->makeCurrent();
            //texture=glwidget->load2DTexture(image.width(), image.height(), image.bits());
            texture=glwidget->m_shareWidget->load2DTexture(image.width(), image.height(), image.bits());
            if(texture<=0)
            {
                qInfo()<<"load2DTexture error";
            }
#if 1
            idxFbo=glwidget->m_shareWidget->fragRenderForOtherThreadAgain("Basic"
                    , NULL, 0, texture
                    , 1, 1, 0, true, true);
            if(idxFbo<0)
            {
                qInfo()<<"fragRenderForOtherThread error";
            }
#endif
            glwidget->m_shareWidget->context()->doneCurrent();
            qDebug()<<"PlayerPrivate::run end lock";
            }
        }
        if(idxFbo>=0)
        {
            glwidget->m_texture=glwidget->m_shareWidget->GetTexture(idxFbo);
            //glwidget->m_idxFbo=idxFbo;
            glwidget->update();
        }
        else if(texture>0)
        {
#if 0
            glwidget->m_texture=texture;
            glwidget->update();
#endif
        }
        //m_bstart=false;
        QThread::msleep(frameTime);
    }
}
