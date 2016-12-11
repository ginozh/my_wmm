#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include "mainwindow.h"
#include <QFontMetrics>
#include <QFont>
#include <QToolButton>
#include <QMenu>
#include <QFile>
#include <QPushButton>
#include <QStyle>
MainWindow::MainWindow()
{
    setWindowTitle(tr("Character Map"));

    QString fileName=QString(tr("C:/qtproject/ffmpeg-3.0.2/jpg/longlovechina2.wav"));
    //QString fileName=QString(tr("C:/qtproject/ffmpeg-3.0.2/jpg/lovechina1.mp3"));
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

    mediaPlayer.setMedia(QUrl::fromLocalFile("tmp.wav"), &m_playBuffer);
    //mediaPlayer.setMedia(QUrl::fromLocalFile("tmp.wav"), &m_playBuffer);

    //mediaPlayer.setMedia(QUrl::fromLocalFile(QString(tr("C:/qtproject/ffmpeg-3.0.2/jpg/longlovechina2.wav"))));
    //mediaPlayer.play();
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    if(!mediaPlayer.isAudioAvailable())
    {
        //QMessageBox::information(this, "info", QString(tr("file is not available")));
    }
    int duration = mediaPlayer.duration();

    QWidget *tabAnimations = new QWidget;
    QHBoxLayout *hboxAnimations = new QHBoxLayout;
    {
        QPushButton* playButton = new QPushButton;
        hboxAnimations->addWidget(playButton);

        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        connect(playButton, SIGNAL(clicked()), this, SLOT(play()));
    }
    {
        lblDurationTrans = new QLabel(QString(tr("Duration: %1")).arg(duration));
        hboxAnimations->addWidget(lblDurationTrans);
    }
    //test popup
    {
        QToolButton* fontColorToolButton = new QToolButton;
        hboxAnimations->addWidget(fontColorToolButton);

        fontColorToolButton->setPopupMode(QToolButton::DelayedPopup);
        QMenu *colorMenu = new QMenu(this);
        fontColorToolButton->setMenu(colorMenu);
    }
    tabAnimations->setLayout(hboxAnimations);
    setCentralWidget(tabAnimations);
}
void MainWindow::durationChanged(qint64 duration)
{
    QFont font;
    font.setPointSize(8); 
    QFontMetrics fm(font); 
    int fw = fm.width("test");
    int fh = fm.height();
    //QMessageBox::information(this, "info", QString(tr("file is not available")));
    lblDurationTrans->setText(QString(tr("Duration: %1 fh: %2")).arg(duration).arg(QString::number(fh)));
}
//! [6]

void MainWindow::play()
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
