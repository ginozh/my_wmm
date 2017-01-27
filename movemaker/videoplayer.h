#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H
#include <QMediaPlayer>
#include <QMovie>
#include <QWidget>
#include <QBuffer>
#include <QGraphicsScene>
#include "slider.h"

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
public slots:
    void openFile();
    void play();
private slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
private:
    void initial();
private:
    QMediaPlayer mediaPlayer;
    QGraphicsVideoItem *videoItem;
    QAbstractButton *playButton;
    Slider *positionSlider;
    QGraphicsScene *scene;
    QGraphicsView *graphicsView;
};

#endif
