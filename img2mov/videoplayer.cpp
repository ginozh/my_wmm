#include "videoplayer.h"

#include <QtWidgets>
#include <QVideoSurfaceFormat>
#include <QGraphicsVideoItem>

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
    , mediaPlayer(0, QMediaPlayer::VideoSurface)
    , videoItem(0)
    , playButton(0)
    , positionSlider(0)
{
    setMaximumWidth(600);

    videoItem = new QGraphicsVideoItem;
    videoItem->setSize(QSizeF(500, 480));

    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *graphicsView = new QGraphicsView(scene);

    scene->addItem(videoItem);
#if 0
    QSlider *rotateSlider = new QSlider(Qt::Horizontal);
    rotateSlider->setRange(-180,  180);
    rotateSlider->setValue(0);

    connect(rotateSlider, SIGNAL(valueChanged(int)),
            this, SLOT(rotateVideo(int)));
#endif
#if 1
    QAbstractButton *openButton = new QPushButton(tr("Open..."));
    connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
#endif
    playButton = new QPushButton;
    playButton->setEnabled(false); //uncomplete
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(playButton, SIGNAL(clicked()),
            this, SLOT(play()));

    //positionSlider = new QSlider(Qt::Horizontal);
    positionSlider = new Slider(Qt::Horizontal);
    positionSlider->setRange(0, 0);

    connect(positionSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(setPosition(int)));

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(graphicsView);
    //layout->addWidget(rotateSlider);
    layout->addLayout(controlLayout);

    setLayout(layout);

    mediaPlayer.setVideoOutput(videoItem);
    connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(mediaStateChanged(QMediaPlayer::State)));
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
}

VideoPlayer::~VideoPlayer()
{
}

#if 1
void VideoPlayer::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
    //QMessageBox::information(this, "info", QString(tr("open fileName: %1")).arg(fileName));

    if (!fileName.isEmpty()) {
        //QString vfileName("C:/QtProjects/qtmovie/first.avi");
        //QMessageBox::information(this, "info", QString(tr("open fileName: %1")).arg(vfileName));
        //mediaPlayer.setMedia(QUrl::fromLocalFile(vfileName));
        mediaPlayer.setMedia(QUrl::fromLocalFile(fileName));

        playButton->setEnabled(true);
    }
}
#endif
//void VideoPlayer::playVideo(const QString& fileName)
void VideoPlayer::playVideo(const QString& fileName, const QByteArray& buffer)
{
    //QMessageBox::information(this, "info", QString(tr("set fileName: %1")).arg(fileName));
    if (!fileName.isEmpty()) {
#if 0
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly)) {
            QMessageBox::warning(this, tr("Codecs"),
                                 tr("Cannot read file %1:\n%2")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }
        m_playData = file.readAll();
        m_playBuffer.setBuffer(&m_playData);
        m_playBuffer.open(QIODevice::ReadOnly);
        file.close();
#endif
        m_playData = buffer;
        m_playBuffer.setBuffer(&m_playData);
        m_playBuffer.open(QIODevice::ReadOnly);

        mediaPlayer.setMedia(QUrl::fromLocalFile(fileName), &m_playBuffer);

        playButton->setEnabled(true);
        mediaPlayer.setPosition(0); 
        mediaPlayer.pause(); // for display image
    }
    else
        QMessageBox::information(this, "info", QString(tr("fileName is null")));
}
void VideoPlayer::play()
{
    switch(mediaPlayer.state()) {
    case QMediaPlayer::PlayingState:
        mediaPlayer.pause();
        break;
    default:
        mediaPlayer.play();
        break;
    }
}

void VideoPlayer::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    case QMediaPlayer::StoppedState:
        //mediaPlayer.pause(); // for display image  //uncomplete
    default:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void VideoPlayer::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
    switch(mediaPlayer.state()) {
    case QMediaPlayer::StoppedState:
        //mediaPlayer.pause(); // for display image  //uncomplete
        break;
    default:
        break;
    }
}

void VideoPlayer::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
}

void VideoPlayer::setPosition(int position)
{
    mediaPlayer.setPosition(position);
}


void VideoPlayer::rotateVideo(int angle)
{
    //rotate around the center of video element
    qreal x = videoItem->boundingRect().width() / 2.0;
    qreal y = videoItem->boundingRect().height() / 2.0;
    videoItem->setTransform(QTransform().translate(x, y).rotate(angle).translate(-x, -y));
}
