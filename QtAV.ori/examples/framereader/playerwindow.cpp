#include "playerwindow.h"
#include <QPushButton>
#include <QSlider>
#include <QLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QThread>
#include <QTimer>

using namespace QtAV;

PlayerWindow::PlayerWindow(QWidget *parent) : QWidget(parent)
{
    m_unit = 1000;
    setWindowTitle(QString::fromLatin1("QtAV simple player example"));
    ///m_player = new AVPlayer(this);
    QVBoxLayout *vl = new QVBoxLayout();
    setLayout(vl);
#if 0
    m_vo = new VideoOutput(this);
    if (!m_vo->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
        return;
    }
    m_player->setRenderer(m_vo);
    vl->addWidget(m_vo->widget());
#endif
    m_glwidget = new OpenGLWidget(this);
    vl->addWidget(m_glwidget);
    m_slider = new QSlider();
    m_slider->setOrientation(Qt::Horizontal);
    connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(seekBySlider(int)));
    connect(m_slider, SIGNAL(sliderPressed()), SLOT(seekBySlider()));
#if 0
    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(updateSlider(qint64)));
    connect(m_player, SIGNAL(started()), SLOT(updateSlider()));
    connect(m_player, SIGNAL(notifyIntervalChanged()), SLOT(updateSliderUnit()));
#endif
    vl->addWidget(m_slider);
    QHBoxLayout *hb = new QHBoxLayout();
    vl->addLayout(hb);
    m_startBtn = new QPushButton(tr("Start"));
    m_nextBtn = new QPushButton(tr("GetNext"));
    m_openBtn = new QPushButton(tr("Open"));
    m_playBtn = new QPushButton(tr("Play/Pause"));
    m_stopBtn = new QPushButton(tr("Stop"));
    hb->addWidget(m_startBtn);
    hb->addWidget(m_nextBtn);
    hb->addWidget(m_openBtn);
    hb->addWidget(m_playBtn);
    hb->addWidget(m_stopBtn);
    connect(m_startBtn, &QAbstractButton::clicked, this,
        [=]() { 
#if 1
        ///m_avr.setFile("c:\\qtproject\\Thousand.Planets.mp4");
        m_avr.play("c:\\qtproject\\Thousand.Planets.mp4");
        ///m_player->play(file);
#endif
#if 0
        //framereader ok
        QStringList vdecnames;
        vdecnames << "DXVA";
        ///vdecnames << "FFmpeg";
        r.setVideoDecoders(vdecnames);
        //r.setMedia("c:\\qtproject\\movie\\movie.mp4");
        r.setMedia("c:\\qtproject\\Thousand.Planets.mp4");
#endif
#if 1
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(processSecondThing()));
        //timer->setSingleShot(true);
        timer->start(100);
#endif
        });
    connect(m_nextBtn, &QAbstractButton::clicked, this,
            [=]() { 
#if 1
            VideoFrame frame = m_avr.GetFrame(0); //TODO: if eof
            if(frame.isValid())
                {QImage img=frame.toImage();static int idx=0;++idx;printf("QAbstractButton::clicked img idx: %d  isNull: %d pts: %f\n",idx,img.isNull(),frame.timestamp());fflush(0);if(idx>25 && idx<=60) img.save(QString("images%1.jpg").arg(idx));}// storm

#endif
#if 0
        //framereader ok
            while (r.readMore()) {
                while (r.hasEnoughVideoFrames()) {
                    const VideoFrame f = r.getVideoFrame(); //TODO: if eof
                    if (!f)
                        continue;
                    count++;
                    //r.readMore();
                    const qint64 now = QDateTime::currentMSecsSinceEpoch();
                    printf("decode @%.3f count: %d\n", f.timestamp(), count);fflush(0);
                    m_glwidget->video_frame=f; //mutex
                    m_glwidget->update();
                    //QThread::msleep(20);
                    return;
                }
            }
#endif
            });
    connect(m_openBtn, SIGNAL(clicked()), SLOT(openMedia()));
    connect(m_playBtn, SIGNAL(clicked()), SLOT(playPause()));
    ///connect(m_stopBtn, SIGNAL(clicked()), m_player, SLOT(stop()));
}

void PlayerWindow::openMedia()
{
    QString file = QFileDialog::getOpenFileName(0, tr("Open a video"));
    if (file.isEmpty())
        return;
    ////m_player->play(file);
}

void PlayerWindow::seekBySlider(int value)
{
#if 0
    if (!m_player->isPlaying())
        return;
    m_player->seek(qint64(value*m_unit));
#endif
}

void PlayerWindow::seekBySlider()
{
    seekBySlider(m_slider->value());
}

void PlayerWindow::playPause()
{
#if 0
    if (!m_player->isPlaying()) {
        m_player->play();
        return;
    }
    m_player->pause(!m_player->isPaused());
#endif
}

void PlayerWindow::updateSlider(qint64 value)
{
    ////m_slider->setRange(0, int(m_player->duration()/m_unit));
    m_slider->setValue(int(value/m_unit));
}

void PlayerWindow::updateSlider()
{
    ////updateSlider(m_player->position());
}

void PlayerWindow::updateSliderUnit()
{
    ///m_unit = m_player->notifyInterval();
    updateSlider();
}

void PlayerWindow::processFirstThing()
{
    qDebug()<<"PlayerWindow::processFirstThing";
    m_openBtn->click();
    return;
    QTimer *timer;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processSecondThing()));
    //timer->setSingleShot(true);
    timer->start(500);
    return;
}
void PlayerWindow::processSecondThing()
{
    qDebug()<<"PlayerWindow::processSecondThing";
    m_nextBtn->click();
}
