#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include "mainwindow.h"
#include <QFontMetrics>
#include <QFont>
#include <QToolButton>
#include <QMenu>
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

