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
    glwidget=glw;
    qDebug()<<"PlayerPrivate::setGLWidget moveToThread";
    glwidget->m_shareWidget->context()->moveToThread(this);
    qDebug()<<"PlayerPrivate::setGLWidget moveToThread 2";
}
void PlayerPrivate::run()
{
    for (;;) 
    {
        double frameTime = 1000;
        int idxFbo=-1;
        {
            QImage image1, image2;
            {
                QImage& image=image1;
                QString fileName="c:\\shareproject\\jpg\\img006.jpg";
                image.load(fileName);
                if (image.isNull()) {
                    qDebug()<<"error";
                }
                image = image.convertToFormat(QImage::Format_RGBA8888);
            }
            GLuint texture=0;
            glwidget->m_shareWidget->context()->makeCurrent();
            texture=glwidget->load2DTexture(image1.width(), image1.height(), image1.bits());
            idxFbo=glwidget->fragRenderForOtherThreadAgain("Basic"
                    , NULL, 0, texture
                    , 1, 1, 0, true, true);
            glwidget->m_shareWidget->context()->doneCurrent();
            if(idxFbo<0)
            {
                qInfo()<<"fragRenderForOtherThread error";
            }
        }
        if(idxFbo>=0)
        {
            glwidget->m_idxFbo=idxFbo;
            glwidget->update();
        }
        QThread::msleep(frameTime);
    }
}
