#ifndef MUSICS_H
#define MUSICS_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMediaPlayer>
#include <QTableWidget>
#include "slider.h"

class Musics : public QWidget
{
    Q_OBJECT

public:
    Musics(QWidget *parent=NULL);
    ~Musics();
private slots:
    void openFile();
    void play();
    void stop();
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void addMyMusic();
    void musicDurationChanged(qint64 duration);
private:
    QLabel* m_lblSelectedMusic;
    QPushButton* playButton;
    QPushButton* stopButton;
    Slider *positionSlider;
    QMediaPlayer m_musicMediaPlayer;
    QTableWidget* musicTable;
    QTableWidget* myMusicTable;

    QString m_qsMusicFullFilename;
    bool m_isFirstMusic;
};

#endif // MUSICS_H
