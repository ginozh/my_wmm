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
    setMinimumWidth(400);
    setMaximumWidth(600);

    //scene = new GraphicsScene(NULL, this);
    scene = new GraphicsScene(this);
    graphicsView = new QGraphicsView(scene);
    //graphicsView->setSceneRect(0,0,500,480);
    scene->setSceneRect(0,0,512,384); //如果没有这个，可能宽度会变成764
    graphicsView->setSceneRect(0,0,512,384);

    videoItem = new QGraphicsVideoItem;
    videoItem->setSize(QSizeF(512, 384));
    scene->addItem(videoItem);
#if 0
    videoItem = new QGraphicsVideoItem;
    videoItem->setSize(QSizeF(500, 480));

    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *graphicsView = new QGraphicsView(scene);

    scene->addItem(videoItem);

    //widgets/graphicsview/diagramscene
    QFont font;
    font.setPointSize(font.pointSize() * 2);
    font.setBold(true);
    QString sceneText = "Qt Everywhere!";
    QGraphicsTextItem *text = scene->addText(sceneText, font);
    text->setDefaultTextColor(Qt::black);
    text->setPos(50, 300);
#endif
    /*
    一、准备工作
tab: font and text animation
    二、步骤
    1、使编辑框处于编辑状态：1.1 点击图片，显示文本编辑框；再点击文本编辑框
        or  1.2 双击图片下面的文字label
    2、拖动。鼠标停留在文本框四周时，鼠标显示可拖动标识

    三、开发计划
    1, 
     */
#if 0
    QSlider *rotateSlider = new QSlider(Qt::Horizontal);
    rotateSlider->setRange(-180,  180);
    rotateSlider->setValue(0);

    connect(rotateSlider, SIGNAL(valueChanged(int)),
            this, SLOT(rotateVideo(int)));
#endif

//#define OPEN_FILE
#ifdef OPEN_FILE
    QAbstractButton *openButton = new QPushButton(tr("Open..."));
    connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
#endif
    playButton = new QPushButton;
    playButton->setEnabled(false); //uncomplete
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(playButton, SIGNAL(clicked()), this, SLOT(play()));

    //positionSlider = new QSlider(Qt::Horizontal);
    positionSlider = new Slider(Qt::Horizontal);
    positionSlider->setRange(0, 0);

    connect(positionSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(setPosition(int)));

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
#ifdef OPEN_FILE
    controlLayout->addWidget(openButton);
#endif
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
//void VideoPlayer::readyVideo(const QString& fileName)
void VideoPlayer::readyVideo(const QString& fileName, const QByteArray& buffer, int position)
{
    //QMessageBox::information(this, "info", QString(tr("position: %1")).arg(position));
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
        mediaPlayer.pause(); // for display image  //uncomplete
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

