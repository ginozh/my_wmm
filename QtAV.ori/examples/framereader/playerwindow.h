#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QWidget>
//#include <QtAV>
#include "OpenGLWidget.h"

#include <QtAV/FrameReader.h>
#include <AVReader.h>

#include <QOpenGLFramebufferObject> //storm
#include <QOpenGLContext> 
#include <QThreadStorage> 
#include <QImage> 
#include <QMatrix4x4> 
#include <QSurface> 
#include <QScreen> 
#include "QtAV/VideoShader.h"
#include "QtAV/GeometryRenderer.h"
#include "QtAV/OpenGLVideo.h"
#include "opengl/ShaderManager.h"
#include <sys/time.h> //storm

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE
class PlayerWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerWindow(QWidget *parent = 0);
public Q_SLOTS:
    void openMedia();
    void seekBySlider(int value);
    void seekBySlider();
    void playPause();
private Q_SLOTS:
    void updateSlider(qint64 value);
    void updateSlider();
    void updateSliderUnit();
    void processFirstThing();
    void processSecondThing();

private:
    //QtAV::VideoOutput *m_vo;
    //QtAV::AVPlayer *m_player;
    OpenGLWidget *m_glwidget;
    QPushButton *m_startBtn;
    QPushButton *m_nextBtn;
    FrameReader r;
    AVReader m_avr;
    int count = 0;

    QSlider *m_slider;
    QPushButton *m_openBtn;
    QPushButton *m_playBtn;
    QPushButton *m_stopBtn;
    int m_unit;
};

#endif // PLAYERWINDOW_H
