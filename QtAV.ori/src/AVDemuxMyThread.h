#ifndef QAV_DEMUXMYTHREAD_H
#define QAV_DEMUXMYTHREAD_H

#include <QtCore/QMutex>
#include <QtCore/QSemaphore>
#include <QtCore/QThread>
#include <QtCore/QRunnable>
#include "PacketBuffer.h"

namespace QtAV {

class AVDemuxer;
class AVThread;
class AVDemuxMyThread : public QThread
{
    Q_OBJECT
public:
    explicit AVDemuxMyThread(QObject *parent = 0);
    explicit AVDemuxMyThread(AVDemuxer *dmx, QObject *parent = 0);
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
    virtual void run();
    /*
     * If the pause state is true setted by pause(true), then block the thread and wait for pause state changed, i.e. pause(false)
     * and return true. Otherwise, return false immediatly.
     */
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
    friend class SeekTask;
    friend class stepBackwardTask;
    friend class FFmpegThread;
};

} //namespace QtAV

#endif
