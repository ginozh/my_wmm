#ifndef PLAYERPRIVATE_H
#define PLAYERPRIVATE_H

#include <QThread>
#include <QImage>
#include "FFmpegReader.h"
#include "Timeline.h"
#include "Clip.h"

class PlayerPrivate : public QThread
{
    Q_OBJECT
public:
    explicit PlayerPrivate(){}
    ~PlayerPrivate(){}
protected:
    void run(){
#if 1
        t = new Timeline();

        FFmpegReader* r = NULL;
        Clip* c1 = NULL;
        Effect* e = NULL; //: filter,motion,video/pic attribute
        //Transition* ts = NULL; //: mask, fragment, ffmpeg

        r = new FFmpegReader();
        r->setFileName("c:/shareproject/jpg/IMG_1924.MP4");
        c1 = new Clip(r);
        t->AddClip(c1);

        r = new FFmpegReader();
        r->setFileName("c:/Users/Public/Videos/SampleVideos/Wildlife.wmv");
        //r->setFileName("c:/shareproject/jpg/IMG_1924.MP4");
        c1 = new Clip(r);
        t->AddClip(c1);

        av_log(NULL, AV_LOG_INFO, "PlayerPrivate run r: %d\n", r);
        while(1){
            QImage img = t->GetFrame(1);
            av_log(NULL, AV_LOG_INFO, "image isnull: %d\n", img.isNull());
            if(img.isNull()){
                QThread::msleep(100);
                break;
            }
            else{
                emit sig_GetOneFrame(img);
                //QThread::msleep(20);
            }
        }
#else
#ifdef SUPPORT_THREAD
        ////r->start();
#endif
        FFmpegReader* r = NULL;
        r = new FFmpegReader();
        r->setFileName("c:/shareproject/jpg/IMG_1924.MP4");
        av_log(NULL, AV_LOG_INFO, "PlayerPrivate run r: %d\n", r);
        while(1){
            QImage img = r->GetFrame(1);
            av_log(NULL, AV_LOG_INFO, "image isnull: %d\n", img.isNull());
            if(img.isNull()){
                break;
            }
            else{
                emit sig_GetOneFrame(img);
            }
            //QThread::msleep(20);
            //sleep(delay);
        }
        r = new FFmpegReader();
        r->setFileName("c:/Users/Public/Videos/SampleVideos/Wildlife.wmv");
#ifdef SUPPORT_THREAD
        ////r->start();
#endif
        av_log(NULL, AV_LOG_INFO, "PlayerPrivate run r: %d\n", r);
        while(1){
            QImage img = r->GetFrame(1);
            av_log(NULL, AV_LOG_INFO, "image isnull: %d\n", img.isNull());
            if(img.isNull()){
                break;
            }
            else{
                emit sig_GetOneFrame(img);
            }
            //QThread::msleep(20);
            //sleep(delay);
        }
#endif
    }
signals:
    void sig_GetOneFrame(QImage);   //每获取到一帧图像 就发送此信号
private:
    Timeline* t = NULL;
};
#endif
