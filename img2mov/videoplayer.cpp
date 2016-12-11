#include "videoplayer.h"
#include "elementsedit.h"

#include <QtWidgets>
#include <QVideoSurfaceFormat>
#include <QGraphicsVideoItem>
#include <QDebug>

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
    int iFrameWidth = 650*dFactorX;
    //setMinimumWidth(400);
    //setMaximumWidth(600);
    setFixedWidth(iFrameWidth);
    //setFixedSize(600, 500);

    QBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    {
        scene = new GraphicsScene(this);
        graphicsView = new QGraphicsView(scene);
        layout->addWidget(graphicsView);

        //graphicsView->setSceneRect(0,0,500,480);
        QSize qSize=GlobalContext::instance()->m_iScaledSize;
        int iSceneWidth = qSize.width();
        int iSceneHeight = qSize.height();
        int iHeightIdx = 0;
        scene->setSceneRect(0,0,iSceneWidth,iSceneHeight); //如果没有这个，可能宽度会变成764
        graphicsView->setSceneRect(0,0,iSceneWidth,iSceneHeight);
        //graphicsView->setGeometry(QRect(0, 0, iFrameWidth, 500*dFactorY));
        graphicsView->setFixedSize(iFrameWidth, 500*dFactorY);

        videoItem = new QGraphicsVideoItem;
        videoItem->setSize(QSizeF(iSceneWidth, iSceneHeight));
        scene->addItem(videoItem);
    }
    {
        m_qlDisplayTime = new QLabel;
        layout->addWidget(m_qlDisplayTime);
        m_qlDisplayTime->setFixedHeight(40*dFactorY);

        m_qlDisplayTime->setAlignment(Qt::AlignRight|Qt::AlignTop);
        m_qlDisplayTime->setText("00:00.00/00:00.00");

#if 0
        QBoxLayout *timeLayout = new QHBoxLayout;
        layout->addLayout(timeLayout);
        timeLayout->setMargin(0);
        {
        }
        {
        }
        {
        }
        timeLayout->addWidget(playButton);
        timeLayout->addWidget(positionSlider);
#endif
    }
    {
        QBoxLayout *controlLayout = new QHBoxLayout;
        layout->addLayout(controlLayout);
        controlLayout->setAlignment(Qt::AlignTop);
        //controlLayout->setFixedSize(600, 50);
        controlLayout->setGeometry(QRect(0, 550*dFactorY, iFrameWidth, 20*dFactorY));
        controlLayout->setMargin(0);
        {
//#define OPEN_FILE
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
#if 1
    {
#if 0
        QVBoxLayout *nullLayout = new QVBoxLayout;
        layout->addLayout(nullLayout);
#endif
        QWidget* nullWidget = new QWidget;
        nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        layout->addWidget(nullWidget);
    }
#endif

    setLayout(layout);

    mediaPlayer.setVideoOutput(videoItem);
    connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(mediaStateChanged(QMediaPlayer::State)));
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));

    initial();
}

VideoPlayer::~VideoPlayer()
{
}

#if 1
void VideoPlayer::openFile()
{
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
    //QMessageBox::information(this, "info", QString(tr("open fileName: %1")).arg(fileName));

    if (!fileName.isEmpty()) {
        //QString vfileName("C:/QtProjects/qtmovie/first.avi");
        //QMessageBox::information(this, "info", QString(tr("open fileName: %1")).arg(vfileName));
        //mediaPlayer.setMedia(QUrl::fromLocalFile(vfileName));
        mediaPlayer.setMedia(QUrl::fromLocalFile(fileName));

#if 0
        QMediaContent mc = mediaPlayer.media();
        //QMediaResourceList mrl = mc.resources();
        QMediaResource mr = mc.canonicalResource();
        qDebug() << "VideoPlayer::openFile. codec: " << mr.videoCodec()<<" mimeType: "<<mr.mimeType()
            <<" dataSize: "<< mr.dataSize()<<" sampleRate: "<<mr.sampleRate()<<" channelCount: "
            <<mr.channelCount();
#endif

        playButton->setEnabled(true);
    }
}
#endif
//void VideoPlayer::readyVideo(const QString& fileName)
void VideoPlayer::readyVideo(const QString& fileName, const QByteArray& buffer, int position)
{
    //QMessageBox::information(this, "info", QString(tr("position: %1")).arg(position));
    qDebug()<<"VideoPlayer::readyVideo fileName: "<<fileName;
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

        mediaPlayer.stop();
        mediaPlayer.setMedia(QUrl::fromLocalFile(fileName), &m_playBuffer);
        mediaPlayer.setNotifyInterval(40); //1秒25帧,即40毫秒通知一次

        playButton->setEnabled(true);
        mediaPlayer.pause(); // for display image
        mediaPlayer.setPosition(position); 
        //positionSlider->setValue(position);
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

void VideoPlayer::controlMusicState(int position/*=-1*/)
{
    qDebug()<<"VideoPlayer::controlMusicState";
    GlobalContext* m_globalContext = GlobalContext::instance();
    if(!m_globalContext || !m_globalContext->m_elementsEdit || !m_globalContext->m_elementsEdit->m_musicMediaPlayer.isAudioAvailable() ) 
    {
        return;
    }
    QMediaPlayer::State state = mediaPlayer.state();
    switch(state) {
        case QMediaPlayer::PlayingState:
            m_globalContext->m_elementsEdit->m_musicMediaPlayer.play();
            break;
        case QMediaPlayer::StoppedState:
            //m_globalContext->m_elementsEdit->m_musicMediaPlayer.stop();
            //break;
        case QMediaPlayer::PausedState:
            m_globalContext->m_elementsEdit->m_musicMediaPlayer.pause();
            break;
    }
#if 1
    if(position!=-1 && m_globalContext->m_elementsEdit->m_musicMediaPlayer.isAudioAvailable()) 
    {
        qDebug()<<"VideoPlayer::controlMusicState. music. position: "<<position<<" state: "<<state;
        m_globalContext->m_elementsEdit->m_musicMediaPlayer.setPosition(position); 
    }
    else
    {
        qDebug()<<"VideoPlayer::controlMusicState. no music. position: "<<position<<" state: "<<state;
    }
#endif
}
void VideoPlayer::mediaStateChanged(QMediaPlayer::State state)
{
    qDebug()<<"VideoPlayer::mediaStateChanged state: "<<state;
    GlobalContext* m_globalContext = GlobalContext::instance();
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
    controlMusicState();
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
#if 0
    QMediaContent mc = mediaPlayer.media();
    QMediaResourceList mrl = mc.resources();
    QMediaResource mr = mrl[0];//mc.canonicalResource();
    qDebug() << "VideoPlayer::openFile. codec: " << mr.videoCodec()<<" mimeType: "<<mr.mimeType()
        <<" dataSize: "<< mr.dataSize()<<" sampleRate: "<<mr.sampleRate()<<" channelCount: "
        <<mr.channelCount();
#endif
}

void VideoPlayer::setPosition(int position)
{
    qDebug()<<"VideoPlayer::setPosition. position: "<<position;
    mediaPlayer.setPosition(position); //notice 不会立即生效
    controlMusicState(position);
}
void VideoPlayer::updatedVideoTimeText(int position, int duration)
{
    QString qsPosition = QDateTime(QDate::currentDate()).addMSecs(position).toString("mm:ss.zzz"); 
    qsPosition.chop(1);
    QString qsDuration = QDateTime(QDate::currentDate()).addMSecs(duration).toString("mm:ss.zzz"); 
    qsDuration.chop(1);
    //qDebug()<<"position: "<<position<<" duration: "<<duration<<"qsPosition: "<<qsPosition<<" qsDuration: "<<qsDuration;
    m_qlDisplayTime->setText(QString(tr("%1/%2")).arg(qsPosition).arg(qsDuration));
}


void VideoPlayer::rotateVideo(int angle)
{
    //rotate around the center of video element
    qreal x = videoItem->boundingRect().width() / 2.0;
    qreal y = videoItem->boundingRect().height() / 2.0;
    videoItem->setTransform(QTransform().translate(x, y).rotate(angle).translate(-x, -y));
}
void VideoPlayer::initial()
{
    //1, 
    playButton->setEnabled(false);

    //2, black screen //uncomlete
    mediaPlayer.stop();
    mediaPlayer.setMedia(NULL);

    m_qlDisplayTime->setText("00:00.00/00:00.00");
}
/*
   QPalette pl = iEdit->palette();

   pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));

   iEdit->setPalette(pl);
   悬浮QLineEdit ,设置样式，完成输入后，将结果显示在画布上

	void Widget::mouseReleaseEvent(QMouseEvent *event)
{
        m_dragging = 0;
}
//mousemove改为：
void Widget::mouseMoveEvent(QMouseEvent *event)
{
        if (m_dragging==1)
        {
                a=event->pos().x();
                b=event->pos().y();
                update();
        }
}
widgets/graphicsview/diagramscene c:\QtProjects\QtExamples\widgets\graphicsview\diagramscene\debug\
widgets/graphicsview/chip
widgets/tools/undo

tips:
    widgets/widgets/charactermap

画板:
    widgets/widgets/scribble
 * */

