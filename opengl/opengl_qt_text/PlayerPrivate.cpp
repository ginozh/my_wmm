#include <QTime>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include "PlayerPrivate.h"
#include "MMComm.h"
#include "GLHiddenWidget.h"
#include "GLDisplayWidget.h"
PlayerPrivate::PlayerPrivate(QObject *parent)
    :QThread(parent)
{
    abort=false;
}

PlayerPrivate::~PlayerPrivate()
{
    //stopPlayback();
}

void PlayerPrivate::setGLWidget(GLDisplayWidget* glw)
{
    qDebug()<<"PlayerPrivate::setGLWidget moveToThread";
    if(!glw || !glw->m_shareWidget)
    {
        qInfo()<<"PlayerPrivate::setGLWidget glwidget: "<<glw<<" glhiddenwidget: "<<(glw?glw->m_shareWidget:NULL);
        return;
    }
    m_glwidget=glw;
    m_glhiddenwidget=glw->m_shareWidget;
    m_glhiddenwidget->setGLDisplayWidget(glw);
    m_glhiddenwidget->context()->moveToThread(this);
    m_glhiddenwidget->setContextThread(this);
    m_glwidget->setShareGLContext();
    qDebug()<<"PlayerPrivate::setGLWidget moveToThread context: "<<m_glhiddenwidget->context();
}
void PlayerPrivate::run()
{
    {
        do{
            m_glhiddenwidget->makeCurrent();
#ifdef OUTPUT_DELAY
            qDebug()<<"PlayerPrivate::run playerWidget_currentContext: "
                <<m_glwidget->context()->currentContext()
                <<" m_hiddenWidget_currentContext: "<<m_glhiddenwidget->context()->currentContext();
#endif
            if(m_glwidget->context() && m_glwidget->context()->currentContext() 
                    && m_glhiddenwidget->context() && m_glhiddenwidget->context()->currentContext())
            {
                m_glhiddenwidget->doneCurrent();
                break;
            }
#if 0
            if(pauseStatue==1)
            {
                conditionAsynPause.wakeOne();
                qDebug()<<"PlayerPrivate::run waiteRealPaused conditionAsynPause wakeOne";
            }
#endif
            QThread::msleep(20);
            m_glhiddenwidget->doneCurrent();
        }while(true);
        m_glhiddenwidget->makeCurrent();
        GLHiddenWidget::getGPUInfo();
        m_glhiddenwidget->doneCurrent();

        m_glhiddenwidget->initializeGLProgram();
        m_glhiddenwidget->createBindFramebufferTexture();
        ////updateBlackFrame(); //update gldisplay && create t->blackframe
    }
    for (;!abort;) 
    {
        {
            m_glhiddenwidget->initialForBackThread();
        }
        if(ready>0)
        {
            m_glwidget->displayTexture=1;
            m_glwidget->update();
            ready=0;
            QThread::msleep(100);
        }
        QThread::msleep(20);
    }
    {
        //MMGlobalContext::instance()->hiddenwidget->context()->moveToThread(qApp->thread());
        m_glwidget->frame.reset();
        m_glhiddenwidget->context()->moveToThread(qApp->thread());
        m_glhiddenwidget->setContextThread(qApp->thread());
    }
}
void PlayerPrivate::startPlayback()
{
    ////stopPlayback();
    abort = false;
#if 0
    if(audioPlayback)
        audioPlayback->abort=false;
    if(videoPlayback)
        videoPlayback->abort=false;
#endif
    if(!isRunning())
    {
        start();
    }
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
