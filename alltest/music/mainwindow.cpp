#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include "mainwindow.h"
MainWindow::MainWindow()
{
    setWindowTitle(tr("Character Map"));

    mediaPlayer.setMedia(QUrl::fromLocalFile(QString(tr("C:/qtproject/ffmpeg-3.0.2/jpg/longlovechina2.wav"))));
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
        lblDurationTrans = new QLabel(QString(tr("Duration: %1")).arg(duration));
        hboxAnimations->addWidget(lblDurationTrans);
    }
    tabAnimations->setLayout(hboxAnimations);
    setCentralWidget(tabAnimations);
}
void MainWindow::durationChanged(qint64 duration)
{
    //QMessageBox::information(this, "info", QString(tr("file is not available")));
    lblDurationTrans->setText(QString(tr("Duration: %1")).arg(duration));
}
//! [6]

