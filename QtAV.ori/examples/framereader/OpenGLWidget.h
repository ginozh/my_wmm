#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
#include <QOpenGLFramebufferObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QMatrix4x4>
#include <QTime>
#include <QVector>
#include <QPushButton>
#include <QMap>
#include <QImage>
#include <memory>
#include <QVariant>
#include <QMutex>
#include <memory>
#include <QPainter>

#include <QtCore/QByteArray>
#include <QtCore/QSize>
#include <QtCore/QRectF>
#include <QtGui/QColor>
#include <QtAV/AVOutput.h>
#include <QtAV/VideoFrame.h>
using namespace QtAV;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();
protected:
    void initializeGL() override;

public:
    void paintGL() override;
    void resizeGL(int w, int h) override;

public:
    QMutex m_mutexRender;
    int glw=0;
    int glh=0;
    int glvieww=0;
    int glviewh=0;

    VideoFrame video_frame;
};

#endif
