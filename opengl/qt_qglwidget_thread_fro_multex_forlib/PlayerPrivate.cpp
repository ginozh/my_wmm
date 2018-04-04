#include <QImage>
#include <QCoreApplication>
#include <QDebug>
#include "PlayerPrivate.h"
#include "GLDisplayWidget.h"
#include "GLHiddenWidget.h"
PlayerPrivate::PlayerPrivate(QObject *parent)
    :QThread(parent)
{
}
PlayerPrivate::~PlayerPrivate()
{
}
void PlayerPrivate::setGLWidget(GLDisplayWidget* glw)
{
    qDebug()<<"PlayerPrivate::setGLWidget moveToThread";
    if(glwidget)
        return;
    glwidget=glw;
    glwidget->m_shareWidget->context()->moveToThread(this);
    qDebug()<<"PlayerPrivate::setGLWidget moveToThread context: "<<glwidget->m_shareWidget->context();
}
void PlayerPrivate::run()
{
    int idx=0;
    if(!glwidget || !glwidget->m_shareWidget)
    {
        qInfo()<<"PlayerPrivate::run error. glwidget: "<<glwidget<<" glwidget->m_shareWidget: "
            <<(glwidget?glwidget->m_shareWidget:NULL);
        return;
    }
    //glwidget->m_shareWidget->initialOpengl(glw, glh);
    glwidget->m_shareWidget->context()->makeCurrent();
    glwidget->m_shareWidget->initializeGLProgram();
    glwidget->m_shareWidget->createBindFramebufferTexture();
    glwidget->m_shareWidget->context()->doneCurrent();
    QImage image;
    {
        QString fileName;
        //if(idx%2==0)
        {
            //fileName="c:\\shareproject\\jpg\\img006.jpg";
            //fileName="c:\\shareproject\\jpg\\1.jpg";
            fileName="1.jpg";
        }
#if 0
        else
        {
            //fileName="c:\\shareproject\\jpg\\img007.jpg";
            fileName="c:\\shareproject\\jpg\\1.jpg";
            //fileName="1.jpg";
        }
#endif
        image.load(fileName);
        if (image.isNull()) {
            qDebug()<<"error fileName: "<<fileName;
        }
        image = image.convertToFormat(QImage::Format_RGBA8888);
    }
    int idxFbo=-1;
    GLuint texture=-1;
    for (;!abort;) 
    {
        double frameTime = 10;
        if(!m_bstart)
        {
            QThread::msleep(frameTime);
            continue;
        }
        ++idx;
#if 1
        {

            {

            qDebug()<<"PlayerPrivate::run start lock idx: "<<idx;
            QMutexLocker locker(&glwidget->m_mutexRender); 
            qDebug()<<"PlayerPrivate::run get lock";
            glwidget->m_shareWidget->context()->makeCurrent();
            if(texture>0)
            {
                qDebug()<<"PlayerPrivate::run delete texture: "<<texture;
                glwidget->m_shareWidget->deleteTexture(texture);
            }
#if 1
            if (idxFbo>=0)
            {
                qDebug()<<"PlayerPrivate::run delete idxFbo: "<<idxFbo;
                glwidget->m_shareWidget->resetFboUsed(idxFbo);
            }
#endif
            //texture=glwidget->load2DTexture(image.width(), image.height(), image.bits());
            texture=glwidget->m_shareWidget->load2DTexture(image.width(), image.height(), image.bits());
            if(texture<=0)
            {
                qInfo()<<"load2DTexture error";
            }
#if 1
            //if(idxFbo<0)
            {
            STFboInfo* finalFboinfo = glwidget->m_shareWidget->getNewFboInfo();
            if(!finalFboinfo)
            {
                qInfo()<<"PlayerPrivate::run error. finalFboinfo is null";
                break;
            }
            idxFbo=glwidget->m_shareWidget->fragRenderForOtherThreadAgain("Basic"
                    , NULL, 0, texture
                    , 1, 1, 0, finalFboinfo, true);
            if(idxFbo<0)
            {
                qInfo()<<"fragRenderForOtherThread error";
            }
            }
#endif
            glwidget->m_shareWidget->context()->doneCurrent();
            qDebug()<<"PlayerPrivate::run end lock";
            }
        }
        if(idxFbo>=0)
        {
            //glwidget->m_texture=glwidget->m_shareWidget->GetTexture(idxFbo);
            //glFlush();
            glwidget->m_idxFbo=idxFbo;
            glwidget->update();
        }
        else if(texture>0)
        {
#if 1
            glwidget->m_texture=texture;
            glwidget->update();
#endif
        }
        //m_bstart=false;
        QThread::msleep(frameTime);
#endif
    }
    glwidget->m_shareWidget->context()->moveToThread(qApp->thread());
}
void PlayerPrivate::stopPlayback()
{
    if(isRunning())
    {
        qDebug()<<"PlayerPrivate::stopPlayback quit myself";
        abort = true;
        quit();
    }
    wait();
    qDebug()<<"PlayerPrivate::stopPlayback quit success";
}
