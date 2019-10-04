#ifndef QTAV_FFMPEGTHREAD_H
#define QTAV_FFMPEGTHREAD_H

#include "AVThread.h"
#include "VideoFrame.h"
#include <QtCore/QSize>
#include <QtCore/QQueue>
//AVDemuxMyThread
#include <QtCore/QMutex>
#include <QtCore/QSemaphore>
#include <QtCore/QThread>
#include <QtCore/QRunnable>
#include "PacketBuffer.h"
//end AVDemuxMyThread

namespace QtAV {
//AVDemuxMyThread
class AVDemuxer;
class AVThread;
//end AVDemuxMyThread

class VideoCapture;
class FFmpegThreadPrivate;
class FFmpegThread : public AVThread
{
    Q_OBJECT
    DPTR_DECLARE_PRIVATE(FFmpegThread)
public:
    explicit FFmpegThread(QObject *parent = 0);
    VideoCapture *setVideoCapture(VideoCapture* cap); //ensure thread safe
    VideoCapture *videoCapture() const;
    VideoFrame displayedFrame() const;
    void setFrameRate(qreal value);
    //virtual bool event(QEvent *event);
    void setBrightness(int val);
    void setContrast(int val);
    void setSaturation(int val);
    void setEQ(int b, int c, int s);

public Q_SLOTS:
    void addCaptureTask();
    void clearRenderers();

protected:
    void applyFilters(VideoFrame& frame);
    // deliver video frame to video renderers. frame may be converted to a suitable format for renderer
    bool deliverVideoFrame(VideoFrame &frame);
    virtual void run();
    // wait for value msec. every usleep is a small time, then process next task and get new delay
public:
    VideoFrame GetFrame(int64_t request_frame);
    QQueue<VideoFrame> m_svframe;
//AVDemuxMyThread
public:
    void setDemuxer(AVDemuxer *dmx);
    void setAudioDemuxer(AVDemuxer *demuxer); //not thread safe
    void setAudioThread(AVThread *thread);
    AVThread* audioThread();
    void setVideoThread(AVThread *thread);
    AVThread* videoThread();
    void stepForward(); // show next video frame and pause
    void stepBackward();
    void seek(qint64 external_pos, qint64 pos, SeekType type); //ms
    //AVDemuxer* demuxer
    bool isPaused() const;
    bool isEnd() const;
    bool atEndOfMedia() const;
    PacketBuffer* buffer();
    void updateBufferState();
    void stop(); //TODO: remove it?
    void pause(bool p, bool wait = false);

    MediaEndAction mediaEndAction() const;
    void setMediaEndAction(MediaEndAction value);
    bool waitForStarted(int msec = -1);
Q_SIGNALS:
    void requestClockPause(bool value);
    void mediaStatusChanged(QtAV::MediaStatus);
    void bufferProgressChanged(qreal);
    void seekFinished(qint64 timestamp);
    void stepFinished();
    void internalSubtitlePacketRead(int index, const QtAV::Packet& packet);
private slots:
    void seekOnPauseFinished();
    void frameDeliveredOnStepForward();
    void eofDecodedOnStepForward();
    void onAVThreadQuit();
protected:
    bool tryPause(unsigned long timeout = 100);

private:
    void setAVThread(AVThread *&pOld, AVThread* pNew);
    void newSeekRequest(QRunnable *r);
    void processNextSeekTask();
    void seekInternal(qint64 pos, SeekType type, qint64 external_pos = std::numeric_limits < qint64 >::min()); //must call in AVDemuxMyThread
    void pauseInternal(bool value);

    bool paused;
    bool user_paused;
    volatile bool end;
    MediaEndAction end_action;
    bool m_buffering;
    PacketBuffer *m_buffer;
    AVDemuxer *demuxer;
    AVDemuxer *ademuxer;
    AVThread *audio_thread, *video_thread;
    int audio_stream, video_stream;
    QMutex buffer_mutex;
    QWaitCondition cond;
    BlockingQueue<QRunnable*> seek_tasks;

    QSemaphore sem;
    QMutex next_frame_mutex;
    int clock_type; // change happens in different threads(direct connection)

//end AVDemuxMyThread
};


} //namespace QtAV

#endif
