#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QApplication>
#include <QAbstractButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QVector>
#include <QPainter>
#include <memory>
#include "GLDisplayWidget.h"
#include "GLHiddenWidget.h"

enum EN_BUTTON_STATUS{
    STATUS_ADD=1,
    STATUS_RM,
    STATUS_EXPORTING,
    STATUS_EXPORT_PAUSE,
};
class PlayerWidget: public GLDisplayWidget
{
    Q_OBJECT
public:
    //PlayerWidget(QWidget *parent = NULL):GLWidget(parent){}
    PlayerWidget(QGLFormat& format, GLHiddenWidget *shareWidget, QWidget *parent = NULL, bool bExport=false):GLDisplayWidget(format, shareWidget, parent, bExport){}
    //PlayerWidget(int w, int h, QWidget *parent = NULL):GLWidget(w,h,parent){}
    ~PlayerWidget(){}
protected:
    void initializeGL() Q_DECL_OVERRIDE
    {
        GLDisplayWidget::initializeGL();
    }
    void paintGL() Q_DECL_OVERRIDE
    {
        GLDisplayWidget::paintGL();
    }
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE
    {
        GLDisplayWidget::showEvent(event);
    }
};
class ExportPlayerWidget: public QWidget
{
    Q_OBJECT
public:
    ExportPlayerWidget(QWidget *parent = NULL){}
    ~ExportPlayerWidget(){}
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
    void processFirstThing();
    void processSecondThing();
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    void initialOpengl();
private:
    PlayerWidget* playerWidget=NULL;
    GLHiddenWidget* m_hiddenWidget=NULL;
    int iw=800;//640;//1280;
    int ih=500;//360;//720;
    int iw_export=640;//1920;//320;//1280;
    int ih_export=360;//1080;//180;//720;
    int id=1;
    bool firstShow=true;

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
    QLabel* m_qlmediaDisplayTime;
    QCheckBox* m_cbDisableVideo;
    QCheckBox* m_cbDisableAudio;
    QAbstractButton *m_pbLeftRotate;
    QAbstractButton *m_pbPreFrame;
    QAbstractButton *m_pbNextFrame;
    QAbstractButton *m_pbCleanAndDelAllSubt;

    int m_playState;
    int m_mediaplayState;
    int m_exportState;
    QAbstractButton *playButton;
    QAbstractButton *stopButton;
    ////MYSlider *positionSlider;
    QAbstractButton *mediaplayButton;
    QAbstractButton *mediastopButton;
    ////MYSlider *mediapositionSlider;
    ////MYSlider *exportPositionSlider=NULL;

    int m_duration=0;
    int m_position=0;
    int m_mediaduration=0;
    int m_mediaposition=0;
    int m_exportduration=0;
    int m_exportposition=0;
    QLineEdit* m_leMediaFile;

    std::shared_ptr<QApplication> previewApp;
    QAbstractButton *m_pbResizeGL;
    QAbstractButton *m_pbBeginEnd;
    QLineEdit* m_leTestMemory;
    //Timeline
    QLineEdit* m_leTimelineNum1;
    QLineEdit* m_leTimelineDen1;
    QAbstractButton *m_pbTimelineSetfps1;

    QComboBox* m_cbTimelineEffect1;
    int m_iTimelineEffectID1=0;
    QLineEdit* m_leTimelinePos1;
    QLineEdit* m_leTimelineDur1;
    QLineEdit* m_leTimelineType1;
    QComboBox* m_cbTimelineType1;
    QAbstractButton *m_pbTimelineAddRm1;
    int m_iTimelineStatus1=STATUS_RM;

    QComboBox* m_cbTimelineEffect2;
    int m_iTimelineEffectID2=0;
    QLineEdit* m_leTimelinePos2;
    QLineEdit* m_leTimelineDur2;
    QLineEdit* m_leTimelineType2;
    QAbstractButton *m_pbTimelineAddRm2;
    int m_iTimelineStatus2=STATUS_RM;
    int m_iTimelineID2;

    QLineEdit* m_leGoFrame;
    QAbstractButton *m_pbGoFrame;

    QLineEdit* m_lemediaGoFrame;
    QAbstractButton *m_pbmediaGoFrame;

    //export
    QAbstractButton *m_pbExportPauseResume;
    QAbstractButton *m_pbExportCancel;
    int m_iExportStatus=STATUS_EXPORTING;

    //clips
    //clip1
    ////Clip* m_clip1=NULL;
    ////QtImageReader* m_reader1=NULL;
    QLineEdit* m_leClipFile1;
    QLineEdit* m_leClipPos1;
    QLineEdit* m_leClipEnd1;
    QLineEdit* m_leClipLayer1;
    QVector<int> m_viClipRotateAction1;
    QComboBox* m_cbClipTransparece1;
    QComboBox* m_cbClipChromaTransparece1;
    //QLineEdit* m_cbClipChromaR1;
    QComboBox* m_cbClipPicEffect1;
    int m_iClipPicEffectID1=0;
    QLineEdit* m_leClipOverlayW1;
    QLineEdit* m_leClipOverlayH1;
    QLineEdit* m_leClipOverlayPosX1;
    QLineEdit* m_leClipOverlayPosY1;
    QComboBox* m_cbClipEffect1;
    int m_iClipEffectID1=0;
    QAbstractButton *m_pbClipAddRm1;
    int m_iClipStatus1=STATUS_RM;
    //clip2
    ////Clip* m_clip2=NULL;
    ////QtImageReader* m_reader2=NULL;
    QLineEdit* m_leClipFile2;
    QLineEdit* m_leClipPos2;
    QLineEdit* m_leClipEnd2;
    QLineEdit* m_leClipLayer2;
    QVector<int> m_viClipRotateAction2;
    QComboBox* m_cbClipTransparece2;
    QComboBox* m_cbClipChromaTransparece2;
    QComboBox* m_cbClipChromaSimilarity2;
    QComboBox* m_cbClipPicEffect2;
    int m_iClipPicEffectID2=0;
    QLineEdit* m_leClipOverlayW2;
    QLineEdit* m_leClipOverlayH2;
    QLineEdit* m_leClipOverlayPosX2;
    QLineEdit* m_leClipOverlayPosY2;
    QComboBox* m_cbClipEffect2;
    int m_iClipEffectID2=0;
    QAbstractButton *m_pbClipAddRm2;
    int m_iClipStatus2=STATUS_RM;
    //clip5 subtitle
    ////Clip* m_clip5=NULL;
    ////SubtitleReader* m_reader5=NULL;
    QLineEdit* m_leClipFile5;
    QLineEdit* m_leClipPos5;
    QLineEdit* m_leClipEnd5;
    QLineEdit* m_leClipLayer5;
    QVector<int> m_viClipRotateAction5;
    QComboBox* m_cbClipPicEffect5;
    int m_iClipPicEffectID5=0;
    QLineEdit* m_leClipOverlayW5;
    QLineEdit* m_leClipOverlayH5;
    QLineEdit* m_leClipOverlayPosX5;
    QLineEdit* m_leClipOverlayPosY5;
    QComboBox* m_cbClipEffect5;
    int m_iClipEffectID5=0;
    QAbstractButton *m_pbClipAddRm5;
    int m_iClipStatus5=STATUS_RM;
    //clip8
    ////Clip* m_clip8=NULL;
    ////FFmpegReader* m_reader8=NULL;
    QLineEdit* m_leClipFile8;
    QLineEdit* m_leClipPos8;
    QLineEdit* m_leClipStart8;
    QLineEdit* m_leClipEnd8;
    QLineEdit* m_leClipLayer8;
    QVector<int> m_viClipRotateAction8;
    QComboBox* m_cbClipTransparece8;
    QComboBox* m_cbClipChromaTransparece8;
    QComboBox* m_cbClipChromaSimilarity8;
    QComboBox* m_cbClipPicEffect8;
    int m_iClipPicEffectID8=0;
    QLineEdit* m_leClipOverlayW8;
    QLineEdit* m_leClipOverlayH8;
    QLineEdit* m_leClipOverlayPosX8;
    QLineEdit* m_leClipOverlayPosY8;
    QComboBox* m_cbClipEffect8;
    QComboBox* m_cbClipVolume8;
    QLineEdit* m_leFadeIn8;
    QLineEdit* m_leFadeOut8;
    QComboBox* m_cbClipSpeed8;
    int m_iClipEffectID8=0;
    QAbstractButton *m_pbClipAddRm8;
    QCheckBox* m_cbClipDisableVideo8;
    QCheckBox* m_cbClipDisableAudio8;
    int m_iClipStatus8=STATUS_RM;
    //clip9
    ////Clip* m_clip9=NULL;
    ////FFmpegReader* m_reader9=NULL;
    QLineEdit* m_leClipFile9;
    QLineEdit* m_leClipPos9;
    QLineEdit* m_leClipEnd9;
    QLineEdit* m_leClipLayer9;
    QVector<int> m_viClipRotateAction9;
    QComboBox* m_cbClipTransparece9;
    QComboBox* m_cbClipChromaTransparece9;
    QComboBox* m_cbClipChromaSimilarity9;
    QComboBox* m_cbClipTest9;
    QComboBox* m_cbClipPicEffect9;
    int m_iClipPicEffectID9=0;
    QLineEdit* m_leClipOverlayW9;
    QLineEdit* m_leClipOverlayH9;
    QLineEdit* m_leClipOverlayPosX9;
    QLineEdit* m_leClipOverlayPosY9;
    QComboBox* m_cbClipEffect9;
    QComboBox* m_cbClipVolume9;
    int m_iClipEffectID9=0;
    QAbstractButton *m_pbClipAddRm9;
    QCheckBox* m_cbClipDisableVideo9;
    QCheckBox* m_cbClipDisableAudio9;
    int m_iClipStatus9=STATUS_RM;

    ////FFmpegReader* m_mediareader=NULL;
    QAbstractButton* m_pbmedia;

    QAbstractButton* m_pbExport;
};

#endif
