#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMediaPlayer>
#include <QMovie>
#include <QWidget>
#include "slider.h"
#include "videoscene.h"
#include <QBuffer>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QGraphicsVideoItem;
class QLabel;
QT_END_NAMESPACE

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent);
    ~VideoPlayer();

    QSize sizeHint() const { return QSize(800, 600); }
    QMediaPlayer* MediaPlayer(){return &mediaPlayer;}
    GraphicsScene *Scene(){return scene;}
    void initial();

public slots:
    void openFile();
    void play();
    void readyVideo(const QString& fileName, const QByteArray& buffer, int position);
signals:
    //void selectedImageSignal(QWidget*);
private slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void updatedVideoTimeText(int position, int duration);
    void rotateVideo(int angle);
private:
    void controlMusicState(int position=-1);

private:
    QMediaPlayer mediaPlayer;
    QGraphicsVideoItem *videoItem;
    QAbstractButton *playButton;
    //QSlider *positionSlider;
    Slider *positionSlider;
    QByteArray m_playData;
    QBuffer m_playBuffer;
    GraphicsScene *scene;
    QGraphicsView *graphicsView;

    QLabel* m_qlDisplayTime;
};

#endif

