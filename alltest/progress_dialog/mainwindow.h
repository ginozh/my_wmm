#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QProgressDialog>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class ProgressDialog : public QProgressDialog {
    Q_OBJECT

public:
    explicit ProgressDialog(const QString& qsFileName, QWidget *parent = Q_NULLPTR);

public slots:
    void saveMoiveProgress(qint64 completed, qint64 total);
};
//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    //void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    //bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;
signals:
    void saveMoiveProgress(qint64 completed, qint64 total);
    void saveMoiveFinish();
private slots:
    void testProgress();
private:
    QLabel *lblDurationTrans;
    QMediaPlayer mediaPlayer;
};
//! [0]

#endif
