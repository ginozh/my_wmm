#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/**
* Copyright(C) 2009-2012
* @author Jing HUANG   matrixvis.cn
* @file MainWindow.h
* @brief  ui interface
* @date 1/2/2017
*/

#include <QMainWindow>
#include <QImage>
#include <QPaintEvent>
#include <QTimer>
#include <videothread.h>
//#include "QWidgetPlayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


protected:
    void paintEvent(QPaintEvent *event);    //绘制图片函数
    void mouseDoubleClickEvent(QMouseEvent *);  //鼠标双击事件
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
private:
    Ui::MainWindow *ui;

    VideoThread *mPlayer; //解析视音频类对象

    QImage mImage; //记录当前的图像

    QTimer *mTimer; //定时器-获取当前视频时间

    unsigned char gIndex;  //全屏or not标志

    //QWidgetPlayer* widget_ext;
    //QOpenGLVideoRenderer * widget_ext;   //第二个屏幕

private slots:
    void slotGetOneFrame(QImage img);    //获得图片的槽

    void slotTotalTimeChanged(qint64 uSec);   //视频总时间变化的槽

    void slotSliderMoved(int value);     //播放slider变化的槽

    void slotTimerTimeOut();    //计时槽

    void slotBtnClick();       //按钮槽

    void slot_fullScreen();     //全屏槽
    void slot_subWindow();      //正常屏槽

    void slotStateChanged(VideoThread::PlayState mPlayState);   //播放状态改变槽
    void changeVRMode();

signals:
    void sig_fullScreen();   //全屏信号
    void sig_subWindow();    //正常屏信号

};

#endif // MAINWINDOW_H
