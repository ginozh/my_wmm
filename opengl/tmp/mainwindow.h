#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QLineEdit>
//#include "FFmpegReader.h"
//#include "PlayerPrivate.h"
#include <QAbstractButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include "Slider.h"


class PlayerWidget: public QWidget
{
    Q_OBJECT
public:
    PlayerWidget(QWidget *parent = NULL){}
    ~PlayerWidget(){}
protected:
    void paintEvent(QPaintEvent * /*event*/){    //绘制图片函数
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
private slots:
    void slotGetOneFrame(const QImage& img)
    //void slotGetOneFrame(const QImage* img)
    {    //获得图片的槽
        mImage = img;
        update(); //调用update将执行 paintEvent函数
    }
private:
    QImage mImage; //记录当前的图像
    //const QImage* mImage=NULL; //记录当前的图像
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = NULL);
public slots:
    //void playStateChanged(PlayState playState);
    //void positionChanged(qint64 position);
    //void durationChanged(qint64 duration);
private:
    void Render();
private:
    QString fileName;
    //QImage mImage; //记录当前的图像
    QLineEdit* m_leFileName;
    QLineEdit* m_leTransition;
    QComboBox* m_cbTransition;
    //QLineEdit* m_leMotion;
    QComboBox* m_cbMotion;
    //QLineEdit* m_leFilter;
    QComboBox* m_cbFilter;
    QLineEdit* m_leAVDiffMax;
    QLineEdit* m_leAVDiffMin;
    QLabel* m_qlDisplayTime;
    QCheckBox* m_cbDisableVideo;
    QCheckBox* m_cbDisableAudio;
    QAbstractButton *m_pbLeftRotate;
    QAbstractButton *m_pbPreFrame;
    QAbstractButton *m_pbNextFrame;
    QAbstractButton *m_pbCleanAndDelAllSubt;

    int m_playState;
    QAbstractButton *playButton;
    QAbstractButton *stopButton;
    MYSlider *positionSlider;

    int m_duration=0;
    int m_position=0;

    QLabel* lblImage;
    QImage image1, image2;
    int glw, glh;
    QImage* mask;
    //unsigned char *mask_pixels;
    int64_t curr_effect_number;
    int64_t total_effect_number;
    QString m_effectName, m_effectName2;
public:
    PlayerWidget* playerWidget;
};

#endif
