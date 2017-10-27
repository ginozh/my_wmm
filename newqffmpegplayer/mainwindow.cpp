/**
* Copyright(C) 2009-2012
* @author Jing HUANG   matrixvis.cn
* @file MainWindow.cpp
* @brief
* @date 1/2/2017
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaType>
#include <QPainter>
#include <QPaintEvent>
#include <QFileDialog>
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

    //=======QDesktopWidget=======
    QDesktopWidget *desktop=QApplication::desktop();
    bool shi=desktop->isVirtualDesktop();
    int num=desktop->numScreens();       //显示屏数
    int nump=desktop->primaryScreen();   //主要的屏幕
    int nnn=desktop->screenNumber();     //屏幕数

    if(num > 0)
    {
#if 0
        widget_ext = NULL;
        widget_ext = new QOpenGLVideoRenderer();
        //widget_ext = new QWidgetPlayer;
        //this->setCentralWidget(widget_ext);
#endif
    }

    //=============================

    av_register_all(); //初始化FFMPEG  调用了这个才能正常使用编码器和解码器

    mPlayer = new PlayerPrivate;    //解析音视频类对象
    connect(mPlayer,SIGNAL(sig_GetOneFrame(QImage)),this,SLOT(slotGetOneFrame(QImage)));
    //connect(mPlayer,SIGNAL(sig_TotalTimeChanged(qint64)),this,SLOT(slotTotalTimeChanged(qint64)));
    //connect(mPlayer,SIGNAL(sig_StateChanged(VideoThread::PlayerState)),this,SLOT(slotStateChanged(VideoThread::PlayerState)));

    mTimer = new QTimer;    //定时器-获取当前视频时间
    connect(mTimer,SIGNAL(timeout()),this,SLOT(slotTimerTimeOut()));
    mTimer->setInterval(500);

    //按钮
    connect(ui->pushButton_open,SIGNAL(clicked()),this,SLOT(slotBtnClick()));
    connect(ui->pushButton_play,SIGNAL(clicked()),this,SLOT(slotBtnClick()));
    connect(ui->pushButton_pause,SIGNAL(clicked()),this,SLOT(slotBtnClick()));
    connect(ui->pushButton_stop,SIGNAL(clicked()),this,SLOT(slotBtnClick()));
    connect(ui->pushButton_two,SIGNAL(clicked()),this,SLOT(slotBtnClick()));
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),this,SLOT(slotSliderMoved(int)));
    connect(ui->pushButton_VRMode,SIGNAL(clicked()),this,SLOT(changeVRMode()));
    connect(this, SIGNAL(sig_fullScreen()), this, SLOT(slot_fullScreen()));
    connect(this, SIGNAL(sig_subWindow()), this, SLOT(slot_subWindow()));
    //widget_ext->show();
    //widget_ext = new QOpenGLVideoRenderer();
    //widget_ext->show();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *mouseEvent){

    gIndex = ~gIndex;//每次翻转一次
    if( mouseEvent->buttons() == Qt::LeftButton){ //判断是否左键双击
    if(gIndex & 0x1){
        emit sig_fullScreen(); //发送全屏信号
    }
    else{
        emit sig_subWindow();} //退出全屏信号
   }
}

void MainWindow::slot_fullScreen(){   //全屏

    MainWindow::showFullScreen();
}

void MainWindow::slot_subWindow(){  //正常屏

    MainWindow::showNormal();
}

void MainWindow::paintEvent(QPaintEvent *event){  //绘制
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色

    if (mImage.size().width() <= 0) return;

    //将图像按比例缩放成和窗口一样大小
    QImage img = mImage.scaled(this->size(),Qt::KeepAspectRatio);

    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 2;
    y /= 2;

    painter.drawImage(QPoint(x,y),img); //画出图像

}

void MainWindow::changeVRMode()
{
#if 0
    if(widget_ext != NULL)
    {
        widget_ext->changeVRMode();
    }
#endif
}

void MainWindow::slotGetOneFrame(QImage img){
    mImage = img;
    update(); //调用update将执行 paintEvent函数
#if 0
    if(widget_ext != NULL && widget_ext->isVisible())
        widget_ext->updateFrame(mImage);   //将QImage传递给另一个屏幕
#endif
}

void MainWindow::slotTotalTimeChanged(qint64 uSec){       //视频的总时间
    qint64 Sec = uSec/1000000;

    ui->horizontalSlider->setRange(0,Sec);

    //时分秒，小时未加
//  QString hStr = QString("00%1").arg(Sec/3600);
    QString mStr = QString("00%1").arg(Sec/60);
    QString sStr = QString("00%1").arg(Sec%60);

    QString str = QString("%1:%2").arg(mStr.right(2)).arg(sStr.right(2));
    ui->label_totaltime->setText(str);

}

void MainWindow::slotSliderMoved(int value){      //slider变化
    if (QObject::sender() == ui->horizontalSlider){
#ifdef VIDEO_THREAD
        mPlayer->seek((qint64)value * 1000000);
#endif
    }
}

void MainWindow::slotTimerTimeOut(){      //当前视频的正在播放的时间
    if (QObject::sender() == mTimer){
#ifdef VIDEO_THREAD
        qint64 Sec = mPlayer->getCurrentTime();
        blockSignals(true);
        ui->horizontalSlider->setValue(Sec);
        blockSignals(false);
        //时分秒，只显示了分秒
    //  QString hStr = QString("00%1").arg(Sec/3600);
        QString mStr = QString("00%1").arg(Sec/60);
        QString sStr = QString("00%1").arg(Sec%60);

        QString str = QString("%1:%2").arg(mStr.right(2)).arg(sStr.right(2));
        ui->label_currenttime->setText(str);        //显示时间的label
#endif
    }
}

void MainWindow::slotBtnClick(){         //按钮监听事件
    if (QObject::sender() == ui->pushButton_play){   //播放按钮
        mPlayer->start();
#if 0
        mPlayer->setFileName("c:/Users/Public/Videos/SampleVideos/Wildlife.wmv");
        while(1){
            QImage img = mPlayer->GetFrame(1);
            av_log(NULL, AV_LOG_INFO, "image isnull: %d\n", img.isNull());
            //emit sig_GetOneFrame(img);
            slotGetOneFrame(img);
            //sleep(delay);
        }
#endif
        mTimer->start();
    }
    else if (QObject::sender() == ui->pushButton_pause){  //暂停按钮
#ifdef VIDEO_THREAD
        mPlayer->pause();
#endif
    }
    else if (QObject::sender() == ui->pushButton_two){    //显示另一个屏播放按钮
#if 0
        if(widget_ext != NULL)
        {
            int num=QApplication::desktop()->numScreens();
            if(num > 1)
            {
                QScreen *screen = QGuiApplication::screens()[1]; // specify which screen to use
                widget_ext->move(screen->geometry().x(), screen->geometry().y());
                widget_ext->showFullScreen();
            }
            widget_ext->show();
        }
#endif
    }
    else if (QObject::sender() == ui->pushButton_stop){   //停止播放按钮
#ifdef VIDEO_THREAD
        mPlayer->stop(true);
#endif
    }
    else if (QObject::sender() == ui->pushButton_open){    //打开文件按钮
        QString s=QFileDialog::getOpenFileName(this,tr("open the movie"),"/",tr("mov files(*.wmv *.mov *.flv *.rmvb *.mp4 *.avi);;all files(*)"));
        if (!s.isEmpty()){
            s.replace("/","\\");
#ifdef VIDEO_THREAD
            mPlayer->stop(true);      //如果在播放则先停止
            mPlayer->setFileName(s);
#endif
            mTimer->start();
        }
    }
}

#if 0
void MainWindow::slotStateChanged(VideoThread::PlayState mPlayState){   //播放状态改变时槽，可以加一些变化隐藏等动作

}
#endif

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        close();
        break;
#if 0
    case Qt::Key_1:
        widget_ext->modifyEyeSeparation(0.01);
        break;
    case Qt::Key_2:
        widget_ext->modifyEyeSeparation(-0.01);
        break;
#endif
#ifdef VIDEO_THREAD
    case Qt::Key_Space:
        if(mPlayer->isPlaying()) mPlayer->pause();
        else mPlayer->play();
        break;
#endif
    default:
        break;
    }

    event->accept();
}

void MainWindow::closeEvent(QCloseEvent *event) {
#if 0
    //widget_ext->setVisible(false);
    //QApplication::quitOnLastWindowClosed();
    delete widget_ext;
#endif
    QApplication::quit();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
#ifdef VIDEO_THREAD
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Space)
        {
            if(mPlayer->isPlaying()) mPlayer->pause();
            else mPlayer->play();
        }
#endif

    }
    return QObject::eventFilter(obj, event);
}
