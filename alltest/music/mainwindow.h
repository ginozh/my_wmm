#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QBuffer>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE
//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    //void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    //bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;
private slots:
    void durationChanged(qint64 duration);
    void play();
private:
    QLabel *lblDurationTrans;
    QMediaPlayer mediaPlayer;
    QByteArray m_playData;
    QBuffer m_playBuffer;
};
//! [0]

#endif
