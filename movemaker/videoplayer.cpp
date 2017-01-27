#include <QtWidgets>
#include <QVideoSurfaceFormat>
#include <QGraphicsVideoItem>
#include <QDebug>
#include "videoplayer.h"
#include "comm.h"

VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
    , mediaPlayer(0, QMediaPlayer::VideoSurface)
    , videoItem(0)
    , playButton(0)
    , positionSlider(0)
{
    GlobalContext* globalContext = GlobalContext::instance();
    double dFactorX = globalContext->m_dFactorX;
    double dFactorY = globalContext->m_dFactorY;
    int iFrameWidth = 1000*dFactorX;
    setFixedWidth(iFrameWidth);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    {
        scene = new QGraphicsScene(this);
        graphicsView = new QGraphicsView(scene);
        layout->addWidget(graphicsView);
        QSize qSize=GlobalContext::instance()->m_iScaledSize;
        int iSceneWidth = qSize.width();
        int iSceneHeight = qSize.height();
        int iHeightIdx = 0;
        scene->setSceneRect(0,0,iSceneWidth,iSceneHeight); //如果没有这个，可能宽度会变成764
        graphicsView->setSceneRect(0,0,iSceneWidth,iSceneHeight);
        //graphicsView->setGeometry(QRect(0, 0, iFrameWidth, 500*dFactorY));
        // graphicsView->setFixedSize(iFrameWidth, 500*dFactorY);
        graphicsView->setFixedWidth(iFrameWidth);

        videoItem = new QGraphicsVideoItem;
        videoItem->setSize(QSizeF(iSceneWidth, iSceneHeight));
        qDebug() << " iSceneWidth: " << iSceneWidth << " iSceneHeight: " << iSceneHeight;
        scene->addItem(videoItem);
    }
    {
        QBoxLayout *controlLayout = new QHBoxLayout;
        layout->addLayout(controlLayout);
        controlLayout->setAlignment(Qt::AlignTop);
        //controlLayout->setFixedSize(600, 50);
        controlLayout->setGeometry(QRect(0, 550*dFactorY, iFrameWidth, 20*dFactorY));
        controlLayout->setMargin(0);
        {
#define OPEN_FILE
#ifdef OPEN_FILE
            QAbstractButton *openButton = new QPushButton(tr("Open..."));
            controlLayout->addWidget(openButton);

            connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
#endif
        }
        {
            playButton = new QPushButton;
            controlLayout->addWidget(playButton);

            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            connect(playButton, SIGNAL(clicked()), this, SLOT(play()));
        }
        {
            positionSlider = new Slider(Qt::Horizontal);
            controlLayout->addWidget(positionSlider);

            positionSlider->setRange(0, 0);
            connect(positionSlider, SIGNAL(sliderMoved(int)),
                    this, SLOT(setPosition(int)));
        }
    }
#if 0
    {
        QWidget* nullWidget = new QWidget;
        nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(nullWidget);
    }
#endif

    setLayout(layout);
    mediaPlayer.setVideoOutput(videoItem);
    mediaPlayer.setNotifyInterval(40); //1秒25帧,即40毫秒通知一次

    connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(mediaStateChanged(QMediaPlayer::State)));
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));

    initial();
}

VideoPlayer::~VideoPlayer()
{
}
void VideoPlayer::initial()
{
    //1, 
    playButton->setEnabled(false);

    //2, black screen //uncomlete
    mediaPlayer.stop();
    mediaPlayer.setMedia(NULL);

}
void VideoPlayer::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));

    if (!fileName.isEmpty()) {
        mediaPlayer.setMedia(QUrl::fromLocalFile(fileName));
        playButton->setEnabled(true);
    }
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
    qDebug()<<"VideoPlayer::mediaStateChanged state: "<<state;
    switch(state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    case QMediaPlayer::StoppedState:
        //mediaPlayer.pause(); // for display image  //uncomplete
    case QMediaPlayer::PausedState:
    default:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
    // controlMusicState();
}
void VideoPlayer::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
    switch(mediaPlayer.state()) {
    case QMediaPlayer::StoppedState:
        mediaPlayer.pause(); // for display image  //uncomplete
        break;
    default:
        break;
    }
}
void VideoPlayer::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
    qDebug()<<"VideoPlayer::durationChanged duration: "<<duration<<" isVideoAvailable: "<<mediaPlayer.isVideoAvailable();
}
void VideoPlayer::setPosition(int position)
{
    qDebug()<<"VideoPlayer::setPosition. position: "<<position;
    mediaPlayer.setPosition(position); //notice 不会立即生效
    // controlMusicState(position);
}
