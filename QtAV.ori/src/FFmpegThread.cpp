#include "FFmpegThread.h"
#include "AVThread_p.h"
#include "QtAV/Packet.h"
#include "QtAV/AVClock.h"
#include "QtAV/VideoCapture.h"
#include "QtAV/VideoDecoder.h"
#include "QtAV/VideoRenderer.h"
#include "QtAV/Statistics.h"
#include "QtAV/Filter.h"
#include "QtAV/FilterContext.h"
#include "output/OutputSet.h"
#include "QtAV/private/AVCompat.h"
#include <QtCore/QFileInfo>
#include <QDebug>
///#include "utils/Logger.h"
#include <QThread>
//AVDemuxMyThread
#include <limits>
#include "QtAV/AVClock.h"
#include "QtAV/AVDemuxer.h"
#include "QtAV/AVDecoder.h"
#include "FFmpegThread.h"
#include "AVDemuxMyThread.h"
#include <QtCore/QTime>
#define RESUME_ONCE_ON_SEEK 0
//end AVDemuxMyThread

namespace QtAV {
//AVDemuxMyThread
class AutoSemt {
    QSemaphore *s;
public:
    AutoSemt(QSemaphore* sem) : s(sem) { s->release();}
    ~AutoSemt() {
        if (s->available() > 0)
            s->acquire(s->available());
    }
};

class QueueEmptyCallt : public PacketBuffer::StateChangeCallback
{
public:
    QueueEmptyCallt(AVDemuxMyThread* thread):
        mDemuxThread(thread)
    {}
    virtual void call() {
        if (!mDemuxThread)
            return;
        if (mDemuxThread->isEnd())
            return;
        if (mDemuxThread->atEndOfMedia())
            return;
        mDemuxThread->updateBufferState(); // ensure detect buffering immediately
        AVThread *thread = mDemuxThread->videoThread();
        //qDebug("try wake up video queue");
        if (thread)
            thread->packetQueue()->blockFull(false);
        //qDebug("try wake up audio queue");
        thread = mDemuxThread->audioThread();
        if (thread)
            thread->packetQueue()->blockFull(false);
    }
private:
    AVDemuxMyThread *mDemuxThread;
};
//end AVDemuxMyThread

class FFmpegThreadPrivate : public AVThreadPrivate
{
public:
    FFmpegThreadPrivate():
        AVThreadPrivate()
      , force_fps(0)
      , force_dt(0)
      , capture(0)
      , filter_context(0)
    {
    }
    ~FFmpegThreadPrivate() {
        //not neccesary context is managed by filters.
        if (filter_context) {
            delete filter_context;
            filter_context = 0;
        }
    }

    VideoFrameConverter conv;
    qreal force_fps; // <=0: try to use pts. if no pts in stream(guessed by 5 packets), use |force_fps|
    // not const.
    int force_dt; //unit: ms. force_fps = 1/force_dt.

    double pts; //current decoded pts. for capture. TODO: remove
    VideoCapture *capture;
    VideoFilterContext *filter_context;//TODO: use own smart ptr. QSharedPointer "=" is ugly
    VideoFrame displayed_frame;
};

FFmpegThread::FFmpegThread(QObject *parent) :
    AVThread(*new FFmpegThreadPrivate(), parent)
//AVDemuxMyThread
  , paused(false)
  , user_paused(false)
  , end(false)
  , end_action(MediaEndAction_Default)
  , m_buffering(false)
  , m_buffer(0)
  , demuxer(0)
  , ademuxer(0)
  , audio_thread(0)
  , video_thread(0)
  , clock_type(-1)
//end AVDemuxMyThread
{
//AVDemuxMyThread
    seek_tasks.setCapacity(1);
    seek_tasks.blockFull(false);
//end AVDemuxMyThread
}

//it is called in main thread usually, but is being used in video thread,
VideoCapture* FFmpegThread::setVideoCapture(VideoCapture *cap)
{
    qDebug("setCapture %p", cap);
    DPTR_D(FFmpegThread);
    QMutexLocker locker(&d.mutex);
    VideoCapture *old = d.capture;
    d.capture = cap;
    if (old)
        disconnect(old, SIGNAL(requested()), this, SLOT(addCaptureTask()));
    if (cap)
        connect(cap, SIGNAL(requested()), this, SLOT(addCaptureTask()));
    if (cap->autoSave() ) {
        // statistics is already set by AVPlayer
        cap->setCaptureName(QFileInfo(d.statistics->url).completeBaseName());
    }
    return old;
}

VideoCapture* FFmpegThread::videoCapture() const
{
    return d_func().capture;
}

void FFmpegThread::addCaptureTask()
{
    if (!isRunning())
        return;
    class CaptureTask : public QRunnable {
    public:
        CaptureTask(FFmpegThread *vt) : vthread(vt) {}
        void run() {
            VideoCapture *vc = vthread->videoCapture();
            if (!vc)
                return;
            VideoFrame frame(vthread->displayedFrame());
            //vthread->applyFilters(frame);
            ////vc->setVideoFrame(frame);
            ///vc->start();
        }
    private:
        FFmpegThread *vthread;
    };
    scheduleTask(new CaptureTask(this));
}

void FFmpegThread::clearRenderers()
{
    d_func().outputSet->sendVideoFrame(VideoFrame());
}

VideoFrame FFmpegThread::displayedFrame() const
{
    return d_func().displayed_frame;
}

void FFmpegThread::setFrameRate(qreal value)
{
    DPTR_D(FFmpegThread);
    d.force_fps = value;
    if (d.force_fps != 0.0) {
        d.force_dt = int(1000.0/d.force_fps);
    } else {
        d.force_dt = 0;
    }
}

void FFmpegThread::setBrightness(int val)
{
    setEQ(val, 101, 101);
}

void FFmpegThread::setContrast(int val)
{
    setEQ(101, val, 101);
}

void FFmpegThread::setSaturation(int val)
{
    setEQ(101, 101, val);
}

void FFmpegThread::setEQ(int b, int c, int s)
{
    class EQTask : public QRunnable {
    public:
        EQTask(VideoFrameConverter *c)
            : brightness(0)
            , contrast(0)
            , saturation(0)
            , conv(c)
        {
            //qDebug("EQTask tid=%p", QThread::currentThread());
        }
        void run() {
            conv->setEq(brightness, contrast, saturation);
        }
        int brightness, contrast, saturation;
    private:
        VideoFrameConverter *conv;
    };
    DPTR_D(FFmpegThread);
    EQTask *task = new EQTask(&d.conv);
    task->brightness = b;
    task->contrast = c;
    task->saturation = s;
    if (isRunning()) {
        scheduleTask(task);
    } else {
        task->run();
        delete task;
    }
}

void FFmpegThread::applyFilters(VideoFrame &frame)
{
    DPTR_D(FFmpegThread);
    QMutexLocker locker(&d.mutex);
    Q_UNUSED(locker);
    if (!d.filters.isEmpty()) {
        //sort filters by format. vo->defaultFormat() is the last
        foreach (Filter *filter, d.filters) {
            VideoFilter *vf = static_cast<VideoFilter*>(filter);
            if (!vf->isEnabled())
                continue;
            if (vf->prepareContext(d.filter_context, d.statistics, &frame))
                vf->apply(d.statistics, &frame);
        }
    }
}

// filters on vo will not change video frame, so it's safe to protect frame only in every individual vo
bool FFmpegThread::deliverVideoFrame(VideoFrame &frame)
{
    DPTR_D(FFmpegThread);
    /*
     * TODO: video renderers sorted by preferredPixelFormat() and convert in AVOutputSet.
     * Convert only once for the renderers has the same preferredPixelFormat().
     */
    d.outputSet->lock();
    QList<AVOutput *> outputs = d.outputSet->outputs();
    VideoRenderer *vo = 0;
    if (!outputs.isEmpty())
        vo = static_cast<VideoRenderer*>(outputs.first());
    if (vo && (!vo->isSupported(frame.pixelFormat())
            || (vo->isPreferredPixelFormatForced() && vo->preferredPixelFormat() != frame.pixelFormat())
            )) {
        VideoFormat fmt(frame.format());
        if (fmt.hasPalette() || fmt.isRGB())
            fmt = VideoFormat::Format_RGB32;
        else
            fmt = vo->preferredPixelFormat();
        VideoFrame outFrame(d.conv.convert(frame, fmt));
        if (!outFrame.isValid()) {
            d.outputSet->unlock();
            return false;
        }
        frame = outFrame;
    }
    d.outputSet->sendVideoFrame(frame); //TODO: group by format, convert group by group
    d.outputSet->unlock();

    Q_EMIT frameDelivered();
    return true;
}
#if 0
//TODO: if output is null or dummy, the use duration to wait
void FFmpegThread::run()
{
    DPTR_D(FFmpegThread);
    if (!d.dec || !d.dec->isAvailable() || !d.outputSet)
        return;
    // resetState(); // we can't reset the thread state from here
    if (d.capture->autoSave()) {
        d.capture->setCaptureName(QFileInfo(d.statistics->url).completeBaseName());
    }
    //not neccesary context is managed by filters.
    d.filter_context = VideoFilterContext::create(VideoFilterContext::QtPainter);
    VideoDecoder *dec = static_cast<VideoDecoder*>(d.dec);
    Packet pkt;
    QVariantHash *dec_opt = &d.dec_opt_normal; //TODO: restore old framedrop option after seek
    /*!
     * if we skip some frames(e.g. seek, drop frames to speed up), then then first frame to decode must
     * be a key frame for hardware decoding. otherwise may crash
     */
    bool wait_key_frame = false;
    int nb_dec_slow = 0;
    int nb_dec_fast = 0;

    qint32 seek_count = 0; // wm4 says: 1st seek can not use frame drop for decoder
    // TODO: kNbSlowSkip depends on video fps, ensure slow time <= 2s
    /* kNbSlowSkip: if video frame slow count >= kNbSlowSkip, skip decoding all frames until next keyframe reaches.
     * if slow count > kNbSlowSkip/2, skip rendering every 3 or 6 frames
     */
    const int kNbSlowSkip = 20;
    // kNbSlowFrameDrop: if video frame slow count > kNbSlowFrameDrop, skip decoding nonref frames. only some of ffmpeg based decoders support it.
    const int kNbSlowFrameDrop = 10;
    bool sync_audio = d.clock->clockType() == AVClock::AudioClock;
    bool sync_video = d.clock->clockType() == AVClock::VideoClock; // no frame drop
    const qint64 start_time = QDateTime::currentMSecsSinceEpoch();
    qreal v_a = 0;
    int nb_no_pts = 0;
    //bool wait_audio_drain
    const char* pkt_data = NULL; // workaround for libav9 decode fail but error code >= 0
    qint64 last_deliver_time = 0;
    int sync_id = 0;
    while (!d.stop) {
        processNextTask();
        //TODO: why put it at the end of loop then stepForward() not work?
        //processNextTask tryPause(timeout) and  and continue outter loop
        if (d.render_pts0 < 0) { // no pause when seeking
            if (tryPause()) { //DO NOT continue, or stepForward() will fail

            } else {
                if (isPaused())
                    continue; //timeout. process pending tasks
            }
        }
        if (d.seek_requested) {
            d.seek_requested = false;
            qDebug("request seek video thread");
            pkt = Packet(); // last decode failed and pkt is valid, reset pkt to force take the next packet if seek is requested
            msleep(1);
        } else {
            // d.render_pts0 < 0 means seek finished here
            if (d.clock->syncId() > 0) {
                qDebug("video thread wait to sync end for sync id: %d", d.clock->syncId());
                if (d.render_pts0 < 0 && sync_id > 0) {
                    msleep(10);
                    v_a = 0;
                    continue;
                }
            } else {
                sync_id = 0;
            }
        }
        if(!pkt.isValid() && !pkt.isEOF()) { // can't seek back if eof packet is read
            pkt = d.packets.take(); //wait to dequeue
           // TODO: push pts history here and reorder
        }
        if (pkt.isEOF()) {
            wait_key_frame = false;
            qDebug("video thread gets an eof packet.");
        } else {
            //qDebug() << pkt.position << " pts:" <<pkt.pts;
            //Compare to the clock
            if (!pkt.isValid()) {
                // may be we should check other information. invalid packet can come from
                wait_key_frame = true;
                qDebug("Invalid packet! flush video codec context!!!!!!!!!! video packet queue size: %d", d.packets.size());
                d.dec->flush(); //d.dec instead of dec because d.dec maybe changed in processNextTask() but dec is not
                d.render_pts0 = pkt.pts;
                sync_id = pkt.position;
                if (pkt.pts >= 0)
                    qDebug("video seek: %.3f, id: %d", d.render_pts0, sync_id);
                d.pts_history = ring<qreal>(d.pts_history.capacity());
                v_a = 0;
                continue;
            }
        }
        if (pkt.pts <= 0 && !pkt.isEOF() && pkt.data.size() > 0) {
            nb_no_pts++;
        } else {
            nb_no_pts = 0;
        }
        if (nb_no_pts > 5) {
            qDebug("the stream may have no pts. force fps to: %f/%f", d.force_fps < 0 ? -d.force_fps : 24, d.force_fps);
            d.clock->setClockAuto(false);
            d.clock->setClockType(AVClock::VideoClock);
            if (d.force_fps < 0)
                setFrameRate(-d.force_fps);
            else if (d.force_fps == 0)
                setFrameRate(24);
        }

        if (d.clock->clockType() == AVClock::AudioClock) {
            sync_audio = true;
            sync_video = false;
        } else if (d.clock->clockType() == AVClock::VideoClock) {
            sync_audio = false;
            sync_video = true;
        } else {
            sync_audio = false;
            sync_video = false;
        }
        const qreal dts = pkt.dts; //FIXME: pts and dts
        // TODO: delta ref time
        // if dts is invalid, diff can be very small (<0) and video will be decoded and rendered(display_wait is disabled for now) immediately
        qreal diff = dts > 0 ? dts - d.clock->value() + v_a : v_a;
        if (pkt.isEOF())
            diff = qMin<qreal>(1.0, qMax<qreal>(d.delay, 1.0/d.statistics->video_only.currentDisplayFPS()));
        if (diff < 0 && sync_video)
            diff = 0; // this ensures no frame drop
        if (diff > kSyncThreshold) {
            nb_dec_fast++;
        } else {
            nb_dec_fast /= 2;
        }
        bool seeking = d.render_pts0 >= 0.0;
        if (seeking) {
            nb_dec_slow = 0;
            nb_dec_fast = 0;
        }
        //qDebug("nb_fast/slow: %d/%d. diff: %f, delay: %f, dts: %f, clock: %f", nb_dec_fast, nb_dec_slow, diff, d.delay, dts, clock()->value());
        if (d.delay < -0.5 && d.delay > diff) {
            if (!seeking) {
                // ensure video will not later than 2s
                if (diff < -2 || (nb_dec_slow > kNbSlowSkip && diff < -1.0 && !pkt.hasKeyFrame)) {
                    qDebug("video is too slow. skip decoding until next key frame.");
                    // TODO: when to reset so frame drop flag can reset?
                    nb_dec_slow = 0;
                    wait_key_frame = true;
                    pkt = Packet();
                    v_a = 0;
                    // TODO: use discard flag
                    continue;
                } else {
                    nb_dec_slow++;
                    qDebug("frame slow count: %d. v-a: %.3f", nb_dec_slow, diff);
                }
            }
        } else {
            if (nb_dec_slow >= kNbSlowFrameDrop) {
                qDebug("decrease 1 slow frame: %d", nb_dec_slow);
                nb_dec_slow = qMax(0, nb_dec_slow-1); // nb_dec_slow < kNbSlowFrameDrop will reset decoder frame drop flag
            }
        }
        // can not change d.delay after! we need it to comapre to next loop
        d.delay = diff;
        /*
         *after seeking forward, a packet may be the old, v packet may be
         *the new packet, then the d.delay is very large, omit it.
        */
        if (seeking)
            diff = 0; // TODO: here?
        if (!sync_audio && diff > 0) {
            // wait to dts reaches
            // d.force_fps>0: wait after decoded before deliver
            if (d.force_fps <= 0)// || !qFuzzyCompare(d.clock->speed(), 1.0))
                waitAndCheck(diff*1000UL, dts); // TODO: count decoding and filter time, or decode immediately but wait for display
            diff = 0; // TODO: can not change delay!
        }
        // update here after wait. TODO: use decoded timestamp/guessed next pts?
        d.clock->updateVideoTime(dts); // FIXME: dts or pts?
        bool skip_render = false;
        if (qAbs(diff) < 0.5) {
            if (diff < -kSyncThreshold) { //Speed up. drop frame?
                //continue;
            }
        } else if (!seeking) { //when to drop off?
            qDebug("delay %fs @%.3fs pts:%.3f", diff, d.clock->value(), pkt.pts);
            if (diff < 0) {
                if (nb_dec_slow > kNbSlowSkip) {
                    skip_render = !pkt.hasKeyFrame && (nb_dec_slow %2);
                }
            } else {
                const double s = qMin<qreal>(0.01*(nb_dec_fast>>1), diff);
                qWarning("video too fast!!! sleep %.2f s, nb fast: %d, v_a: %.4f", s, nb_dec_fast, v_a);
                waitAndCheck(s*1000UL, dts);
                diff = 0;
                skip_render = false;
            }
        }
        //audio packet not cleaned up?
        if (diff > 0 && diff < 1.0 && !seeking) {
            // can not change d.delay here! we need it to comapre to next loop
            waitAndCheck(diff*1000UL, dts);
        }
        if (wait_key_frame) {
            if (!pkt.hasKeyFrame) {
                qDebug("waiting for key frame. queue size: %d. pkt.size: %d", d.packets.size(), pkt.data.size());
                pkt = Packet();
                v_a = 0;
                continue;
            }
            wait_key_frame = false;
        }
        QVariantHash *dec_opt_old = dec_opt;
        if (!seeking || pkt.pts - d.render_pts0 >= -0.05) { // MAYBE not seeking. We should not drop the frames near the seek target. FIXME: use packet pts distance instead of -0.05 (20fps)
            if (seeking)
                qDebug("seeking... pkt.pts - d.render_pts0: %.3f", pkt.pts - d.render_pts0);
            if (nb_dec_slow < kNbSlowFrameDrop) {
                if (dec_opt == &d.dec_opt_framedrop) {
                    qDebug("frame drop=>normal. nb_dec_slow: %d", nb_dec_slow);
                    dec_opt = &d.dec_opt_normal;
                }
            } else {
                if (dec_opt == &d.dec_opt_normal) {
                    qDebug("frame drop=>noref. nb_dec_slow: %d too slow", nb_dec_slow);
                    dec_opt = &d.dec_opt_framedrop;
                }
            }
        } else { // seeking
            if (seek_count > 0 && d.drop_frame_seek) {
                if (dec_opt == &d.dec_opt_normal) {
                    qDebug("seeking... pkt.pts - d.render_pts0: %.3f, frame drop=>noref. nb_dec_slow: %d", pkt.pts - d.render_pts0, nb_dec_slow);
                    dec_opt = &d.dec_opt_framedrop;
                }
            } else {
                seek_count = -1;
            }
        }

        // decoder maybe changed in processNextTask(). code above MUST use d.dec but not dec
        if (dec != static_cast<VideoDecoder*>(d.dec)) {
            dec = static_cast<VideoDecoder*>(d.dec);
            if (!pkt.hasKeyFrame) {
                wait_key_frame = true;
                v_a = 0;
                continue;
            }
            qDebug("decoder changed. decoding key frame");
        }
        if (dec_opt != dec_opt_old)
            dec->setOptions(*dec_opt);
        if (!dec->decode(pkt)) {
            d.pts_history.push_back(d.pts_history.back());
            //qWarning("Decode video failed. undecoded: %d/%d", dec->undecodedSize(), pkt.data.size());
            if (pkt.isEOF()) {
                Q_EMIT eofDecoded();
                qDebug("video decode eof done. d.render_pts0: %.3f", d.render_pts0);
                if (d.render_pts0 >= 0) {
                    qDebug("video seek done at eof pts: %.3f. id: %d", d.pts_history.back(), sync_id);
                    d.render_pts0 = -1;
                    d.clock->syncEndOnce(sync_id);
                    Q_EMIT seekFinished(qint64(d.pts_history.back()*1000.0));
                    if (seek_count == -1)
                        seek_count = 1;
                    else if (seek_count > 0)
                        seek_count++;
                }
                if (!pkt.position)
                    break;
            }
            pkt = Packet();
            v_a = 0; //?
            continue;
        }
        // reduce here to ensure to decode the rest data in the next loop
        if (!pkt.isEOF())
            pkt.skip(pkt.data.size() - dec->undecodedSize());
        VideoFrame frame = dec->frame();
        if (!frame.isValid()) {
            qWarning("invalid video frame from decoder. undecoded data size: %d", pkt.data.size());
            if (pkt_data == pkt.data.constData()) //FIXME: for libav9. what about other versions?
                pkt = Packet();
            else
                pkt_data = pkt.data.constData();
            v_a = 0; //?
            continue;
        }
        pkt_data = pkt.data.constData();
        if (frame.timestamp() < 0)
            frame.setTimestamp(pkt.pts); // pkt.pts is wrong. >= real timestamp
        const qreal pts = frame.timestamp();
        d.pts_history.push_back(pts);
        // seek finished because we can ensure no packet before seek decoded when render_pts0 is set
        qDebug("pts0: %f, pts: %f pkt_pts: %f pkt_dts: %f, clock: %d\n", d.render_pts0, pts, pkt.pts, pkt.dts, d.clock->clockType()); {QImage img=frame.toImage();static int idx=0;++idx;printf("FFmpegThread::run img idx: %d  isNull: %d\n",idx,img.isNull());/*if(idx==3 || idx==15)*/ img.save(QString("images%1.jpg").arg(idx));}// storm
        if (d.render_pts0 >= 0.0) {
            if (pts < d.render_pts0) {
                if (!pkt.isEOF())
                    pkt = Packet();
                v_a = 0;
                continue;
            }
            d.render_pts0 = -1;
            qDebug("video seek finished @%f. id: %d", pts, sync_id);
            d.clock->syncEndOnce(sync_id);
            Q_EMIT seekFinished(qint64(pts*1000.0));
            if (seek_count == -1)
                seek_count = 1;
            else if (seek_count > 0)
                seek_count++;
        }
        if (skip_render) {
            qDebug("skip rendering @%.3f", pts);
            pkt = Packet();
            v_a = 0;
            continue;
        }
        Q_ASSERT(d.statistics);
        d.statistics->video.current_time = QTime(0, 0, 0).addMSecs(int(pts * 1000.0)); //TODO: is it expensive?
        applyFilters(frame);

        //while can pause, processNextTask, not call outset.puase which is deperecated
        while (d.outputSet->canPauseThread()) {
            d.outputSet->pauseThread(100);
            //tryPause(100);
            processNextTask();
        }
        //qDebug("force fps: %f dt: %d", d.force_fps, d.force_dt);
        if (d.force_dt > 0) {// && qFuzzyCompare(d.clock->speed(), 1.0)) {
            const qint64 now = QDateTime::currentMSecsSinceEpoch();
            const qint64 delta = qint64(d.force_dt) - (now - last_deliver_time);
            if (frame.timestamp() <= 0) {
                // TODO: what if seek happens during playback?
                const int msecs_started(now + qMax(0LL, delta) - start_time);
                frame.setTimestamp(qreal(msecs_started)/1000.0);
                clock()->updateValue(frame.timestamp()); //external clock?
            }
            if (delta > 0LL) { // limit up bound?
                waitAndCheck((ulong)delta, -1); // wait and not compare pts-clock
            }
        } else if (false) { //FIXME: may block a while when seeking
            const qreal display_wait = pts - clock()->value();
            if (!seeking && display_wait > 0.0) {
                // wait to pts reaches. TODO: count rendering time
                //qDebug("wait %f to display for pts %f-%f", display_wait, pts, clock()->value());
                if (display_wait < 1.0)
                    waitAndCheck(display_wait*1000UL, pts); // TODO: count decoding and filter time
            }
        }
        // no return even if d.stop is true. ensure frame is displayed. otherwise playing an image may be failed to display
        if (!deliverVideoFrame(frame))
            continue;
        //qDebug("clock.diff: %.3f", d.clock->diff());
        if (d.force_dt > 0)
            last_deliver_time = QDateTime::currentMSecsSinceEpoch();
        // TODO: store original frame. now the frame is filtered and maybe converted to renderer perferred format
        d.displayed_frame = frame;
        if (d.clock->clockType() == AVClock::AudioClock) {
            const qreal v_a_ = frame.timestamp() - d.clock->value();
            if (!qFuzzyIsNull(v_a_)) {
                if (v_a_ < -0.1) {
                    if (v_a <= v_a_)
                        v_a += -0.01;
                    else
                        v_a = (v_a_ +v_a)*0.5;
                } else if (v_a_ < -0.002) {
                    v_a += -0.001;
                } else if (v_a_ < 0.002) {
                } else if (v_a_ < 0.1) {
                    v_a += 0.001;
                } else {
                    if (v_a >= v_a_)
                        v_a += 0.01;
                    else
                        v_a = (v_a_ +v_a)*0.5;
                }

                if (v_a < -2 || v_a > 2)
                   v_a /= 2.0;
            }
            //qDebug("v_a:%.4f, v_a_: %.4f", v_a, v_a_);
        }
    }
#if 0
    if (d.stop) {// user stop
        // decode eof?
        qDebug("decoding eof...");

        while (d.dec && d.dec->decode(Packet::createEOF())) {d.dec->flush();}
    }
#endif
    d.packets.clear();
    qDebug("Video thread stops running...");
}
#endif

VideoFrame FFmpegThread::GetFrame(int64_t request_frame)
{
    DPTR_D(FFmpegThread);
    VideoFrame frame;
    if (!d.dec || !d.dec->isAvailable() )
        return frame;
    //not neccesary context is managed by filters.
    VideoDecoder *dec = static_cast<VideoDecoder*>(d.dec);
    Packet pkt;
    const char* pkt_data = NULL; // workaround for libav9 decode fail but error code >= 0
    while (!d.stop) {
        ///processNextTask();
        ////if(!pkt.isValid() && !pkt.isEOF()) 
        { // can't seek back if eof packet is read
            pkt = d.packets.take(); //wait to dequeue
           // TODO: push pts history here and reorder
        }
        if (!dec->decode(pkt)) {
            continue;
        }
        frame = dec->frame();
        if (!frame.isValid()) {
            qWarning("invalid video frame from decoder. undecoded data size: %d", pkt.data.size());
            if (pkt_data == pkt.data.constData()) //FIXME: for libav9. what about other versions?
                pkt = Packet();
            else
                pkt_data = pkt.data.constData();
            continue;
        }
        //qDebug("pts0: %f, pkt_pts: %f pkt_dts: %f, clock: %d\n", d.render_pts0, pkt.pts, pkt.dts, d.clock->clockType()); {QImage img=frame.toImage();static int idx=0;++idx;printf("FFmpegThread::run img idx: %d  isNull: %d d.packets: %d\n",idx,img.isNull(),d.packets.size());fflush(0);if(idx>25 && idx<=60) img.save(QString("images%1.jpg").arg(idx));}// storm
        pkt_data = pkt.data.constData();
        if (frame.timestamp() < 0)
            frame.setTimestamp(pkt.pts); // pkt.pts is wrong. >= real timestamp
        // no return even if d.stop is true. ensure frame is displayed. otherwise playing an image may be failed to display
        //if (!deliverVideoFrame(frame))
        //    continue;
        //QThread::msleep(20);
        break;
    }
    d.packets.clear();
    return frame;
    qDebug("Video thread stops running...");
}
//AVDemuxMyThread
void FFmpegThread::setDemuxer(AVDemuxer *dmx)
{
    demuxer = dmx;
}

void FFmpegThread::setAudioDemuxer(AVDemuxer *demuxer)
{
    //QMutexLocker locker(&buffer_mutex);
    //Q_UNUSED(locker);
    ademuxer = demuxer;
}

void FFmpegThread::setAVThread(AVThread*& pOld, AVThread *pNew)
{
    if (pOld == pNew)
        return;
    if (pOld) {
        if (pOld->isRunning())
            pOld->stop();
        pOld->disconnect(this, SLOT(onAVThreadQuit()));
    }
    pOld = pNew;
    if (!pNew)
        return;
    ////pOld->packetQueue()->setEmptyCallback(new QueueEmptyCallt(this)); //storm
    connect(pOld, SIGNAL(finished()), SLOT(onAVThreadQuit()));
}

void FFmpegThread::setAudioThread(AVThread *thread)
{
    setAVThread(audio_thread, thread);
}

void FFmpegThread::setVideoThread(AVThread *thread)
{
    setAVThread(video_thread, thread);
}

AVThread* FFmpegThread::videoThread()
{
    return video_thread;
}

AVThread* FFmpegThread::audioThread()
{
    return audio_thread;
}

void FFmpegThread::stepBackward()
{
    if (!video_thread)
        return;
    AVThread *t = video_thread;
    const qreal pre_pts = video_thread->previousHistoryPts();
    if (pre_pts == 0.0) {
        qWarning("can not get previous pts");
        return;
    }
    end = false;
    // queue maybe blocked by put()
    if (audio_thread) {
        audio_thread->packetQueue()->clear(); // will put new packets before task run
    }

    class stepBackwardTask : public QRunnable {
    public:
        stepBackwardTask(AVDemuxMyThread *dt, qreal t)
            : demux_thread(dt)
            , pts(t)
        {}
        void run() {
            AVThread *avt = demux_thread->videoThread();
            avt->packetQueue()->clear(); // clear here
            if (pts <= 0) {
                demux_thread->demuxer->seek(qint64(-pts*1000.0) - 500LL);
                QVector<qreal> ts;
                qreal t = -1.0;
                while (t < -pts) {
                    demux_thread->demuxer->readFrame();
                    if (demux_thread->demuxer->stream() != demux_thread->demuxer->videoStream())
                        continue;
                    t = demux_thread->demuxer->packet().pts;
                    ts.push_back(t);
                }
                const qreal t0 = ts.back();
                ts.pop_back();
                const qreal dt = t0 - ts.back();
                pts = ts.back();
                // FIXME: sometimes can not seek to the previous pts, the result pts is always current pts, so let the target pts a little earlier
                pts -= dt/2.0;
            }
            qDebug("step backward: %lld, %f", qint64(pts*1000.0), pts);
            demux_thread->video_thread->setDropFrameOnSeek(false);
            demux_thread->seekInternal(qint64(pts*1000.0), AccurateSeek);
        }
    private:
        AVDemuxMyThread *demux_thread;
        qreal pts;
    };

    pause(true);
    t->packetQueue()->clear(); // will put new packets before task run
    t->packetQueue();
    Packet pkt;
    pkt.pts = pre_pts;
    t->packetQueue()->put(pkt); // clear and put a seek packet to ensure not frames other than previous frame will be decoded and rendered
    video_thread->pause(false);
    ////newSeekRequest(new stepBackwardTask(this, pre_pts));
}

void FFmpegThread::seek(qint64 external_pos, qint64 pos, SeekType type)
{
    class SeekTask : public QRunnable {
    public:
        SeekTask(AVDemuxMyThread *dt, qint64 external_pos, qint64 t, SeekType st)
            : demux_thread(dt)
            , type(st)
            , position(t)
            , external_pos(external_pos)
        {}
        void run() {
            // queue maybe blocked by put()
            if (demux_thread->audio_thread) {
                demux_thread->audio_thread->packetQueue()->clear();
            }
            if (demux_thread->video_thread) {
                demux_thread->video_thread->packetQueue()->clear();
            }
            if (demux_thread->video_thread)
                demux_thread->video_thread->setDropFrameOnSeek(true);
            demux_thread->seekInternal(position, type, external_pos);
        }
    private:
        AVDemuxMyThread *demux_thread;
        SeekType type;
        qint64 position;
        qint64 external_pos;
    };

    end = false;
    // queue maybe blocked by put()	
    // These must be here or seeking while paused will not update the video frame
    if (audio_thread) {
        audio_thread->packetQueue()->clear();
    }
    if (video_thread) {
        video_thread->packetQueue()->clear();
    }
    /// newSeekRequest(new SeekTask(this, external_pos, pos, type)); //storm
}

void FFmpegThread::seekInternal(qint64 pos, SeekType type, qint64 external_pos)
{
    AVThread* av[] = { audio_thread, video_thread};
    qDebug("seek to %s %lld ms (%f%%)", QTime(0, 0, 0).addMSecs(pos).toString().toUtf8().constData(), pos, double(pos - demuxer->startTime())/double(demuxer->duration())*100.0);
    demuxer->setSeekType(type);
    demuxer->seek(pos);
    if (ademuxer) {
        ademuxer->setSeekType(type);
        ademuxer->seek(pos);
    }

    AVThread *watch_thread = 0;
    // TODO: why queue may not empty?
    int sync_id = 0;
    for (size_t i = 0; i < sizeof(av)/sizeof(av[0]); ++i) {
        AVThread *t = av[i];
        if (!t)
            continue;
        if (!sync_id)
            sync_id = t->clock()->syncStart(!!audio_thread + (!!video_thread && !demuxer->hasAttacedPicture()));
        Q_ASSERT(sync_id != 0);
        qDebug("demuxer sync id: %d/%d", sync_id, t->clock()->syncId());
        t->packetQueue()->clear();
        if (external_pos != std::numeric_limits < qint64 >::min() )
            t->clock()->updateExternalClock(qMax(qint64(0), external_pos));
        t->clock()->updateValue(double(pos)/1000.0);
        t->requestSeek();
        // TODO: the first frame (key frame) will not be decoded correctly if flush() is called.
        //PacketBuffer *pb = t->packetQueue();
        //qDebug("%s put seek packet. %d/%d-%.3f, progress: %.3f", t->metaObject()->className(), pb->buffered(), pb->bufferValue(), pb->bufferMax(), pb->bufferProgress());
        t->packetQueue()->setBlocking(false); // aqueue bufferValue can be small (1), we can not put and take
        Packet pkt;
        pkt.pts = qreal(pos)/1000.0;
        pkt.position = sync_id;
        t->packetQueue()->put(pkt);
        t->packetQueue()->setBlocking(true); // blockEmpty was false when eof is read.
        if (isPaused()) { //TODO: deal with pause in AVThread?
            t->pause(false);
            watch_thread = t;
        }
    }
    if (watch_thread) {
        pauseInternal(false);
        Q_EMIT requestClockPause(false); // need direct connection
        // direct connection is fine here
        connect(watch_thread, SIGNAL(seekFinished(qint64)), this, SLOT(seekOnPauseFinished()), Qt::DirectConnection);
    }
}

void FFmpegThread::newSeekRequest(QRunnable *r)
{
    if (seek_tasks.size() >= seek_tasks.capacity()) {
        QRunnable *r = seek_tasks.take();
        if (r && r->autoDelete())
            delete r;
    }
    seek_tasks.put(r);
}

void FFmpegThread::processNextSeekTask()
{
    if (seek_tasks.isEmpty())
        return;
    QRunnable *task = seek_tasks.take();
    if (!task)
        return;
    task->run();
    if (task->autoDelete())
        delete task;
}

void FFmpegThread::pauseInternal(bool value)
{
    paused = value;
}

bool FFmpegThread::isPaused() const
{
    return paused;
}

bool FFmpegThread::isEnd() const
{
    return end;
}

bool FFmpegThread::atEndOfMedia() const
{
    return demuxer->atEnd();
}

PacketBuffer* FFmpegThread::buffer()
{
    return m_buffer;
}

void FFmpegThread::updateBufferState()
{
    if (!m_buffer)
        return;
    if (m_buffering) { // always report progress when buffering
        Q_EMIT bufferProgressChanged(m_buffer->bufferProgress());
    }
    if (m_buffering == m_buffer->isBuffering())
        return;
    m_buffering = m_buffer->isBuffering();
    Q_EMIT mediaStatusChanged(m_buffering ? QtAV::BufferingMedia : QtAV::BufferedMedia);
    // state change to buffering, report progress immediately. otherwise we have to wait to read 1 packet.
    if (m_buffering) {
        Q_EMIT bufferProgressChanged(m_buffer->bufferProgress());
    }
}

//No more data to put. So stop blocking the queue to take the reset elements
void FFmpegThread::stop()
{
    //this will not affect the pause state if we pause the output
    //TODO: why remove blockFull(false) can not play another file?
    AVThread* av[] = { audio_thread, video_thread};
    for (size_t i = 0; i < sizeof(av)/sizeof(av[0]); ++i) {
        AVThread* t = av[i];
        if (!t)
            continue;
        t->packetQueue()->clear();
        t->packetQueue()->blockFull(false); //??
        while (t->isRunning()) {
            qDebug() << "stopping thread " << t;
            t->stop();
            t->wait(500);
        }
    }
    pause(false);
    cond.wakeAll();
    qDebug("all avthread finished. try to exit demux thread<<<<<<");
    end = true;
}

void FFmpegThread::pause(bool p, bool wait)
{
    user_paused = p;
    if (paused == p)
        return;
    paused = p;
    if (!paused)
        cond.wakeAll();
    else {
        if (wait) {
            // block until current loop finished
            buffer_mutex.lock();
            buffer_mutex.unlock();
        }
    }
}

void FFmpegThread::setMediaEndAction(MediaEndAction value)
{
    end_action = value;
}

MediaEndAction FFmpegThread::mediaEndAction() const
{
    return end_action;
}

void FFmpegThread::stepForward()
{
    if (end)
        return;
    // clock type will be wrong if no lock because slot frameDeliveredOnStepForward() is in video thread
    QMutexLocker locker(&next_frame_mutex);
    Q_UNUSED(locker);
    pause(true); // must pause AVDemuxMyThread (set user_paused true)
    AVThread* av[] = {video_thread, audio_thread};
    bool connected = false;
    for (size_t i = 0; i < sizeof(av)/sizeof(av[0]); ++i) {
        AVThread *t = av[i];
        if (!t)
            continue;
        // set clock first
        if (clock_type < 0)
            clock_type = (int)t->clock()->isClockAuto() + 2*(int)t->clock()->clockType();
        t->clock()->setClockType(AVClock::VideoClock);
        t->scheduleFrameDrop(false);
        t->pause(false);
        t->packetQueue()->blockFull(false);
        if (!connected) {
            connect(t, SIGNAL(frameDelivered()), this, SLOT(frameDeliveredOnStepForward()), Qt::DirectConnection);
            connect(t, SIGNAL(eofDecoded()), this, SLOT(eofDecodedOnStepForward()), Qt::DirectConnection);
            connected = true;
        }
    }
    Q_EMIT requestClockPause(false);
    pauseInternal(false);
}

void FFmpegThread::seekOnPauseFinished()
{
    AVThread *thread = video_thread ? video_thread : audio_thread;
    Q_ASSERT(thread);
    disconnect(thread, SIGNAL(seekFinished(qint64)), this, SLOT(seekOnPauseFinished()));
    if (user_paused) {
        pause(true); // restore pause state
        Q_EMIT requestClockPause(true); // need direct connection
    // pause video/audio thread
        if (video_thread)
            video_thread->pause(true);
        if (audio_thread)
            audio_thread->pause(true);
    }
}

void FFmpegThread::frameDeliveredOnStepForward()
{
    AVThread *thread = video_thread ? video_thread : audio_thread;
    Q_ASSERT(thread);
    QMutexLocker locker(&next_frame_mutex);
    Q_UNUSED(locker);
    disconnect(thread, SIGNAL(frameDelivered()), this, SLOT(frameDeliveredOnStepForward()));
    disconnect(thread, SIGNAL(eofDecoded()), this, SLOT(eofDecodedOnStepForward()));
    if (user_paused) {
        pause(true); // restore pause state
        Q_EMIT requestClockPause(true); // need direct connection
    // pause both video and audio thread
        if (video_thread)
            video_thread->pause(true);
        if (audio_thread)
            audio_thread->pause(true);
    }
    if (clock_type >= 0) {
        thread->clock()->setClockAuto(clock_type & 1);
        thread->clock()->setClockType(AVClock::ClockType(clock_type/2));
        clock_type = -1;
        thread->clock()->updateExternalClock((thread->previousHistoryPts() - thread->clock()->initialValue())*1000.0);
    }
    Q_EMIT stepFinished();
}

void FFmpegThread::eofDecodedOnStepForward()
{
    AVThread *thread = video_thread ? video_thread : audio_thread;
    Q_ASSERT(thread);
    QMutexLocker locker(&next_frame_mutex);
    Q_UNUSED(locker);
    disconnect(thread, SIGNAL(frameDelivered()), this, SLOT(frameDeliveredOnStepForward()));
    disconnect(thread, SIGNAL(eofDecoded()), this, SLOT(eofDecodedOnStepForward()));
    pause(false);
    end = true;
    if (clock_type >= 0) {
        thread->clock()->setClockAuto(clock_type & 1);
        thread->clock()->setClockType(AVClock::ClockType(clock_type/2));
        clock_type = -1;
    }
    Q_EMIT stepFinished();
}

void FFmpegThread::onAVThreadQuit()
{
#if 0
    AVThread* av[] = { audio_thread, video_thread};
    for (size_t i = 0; i < sizeof(av)/sizeof(av[0]); ++i) {
        if (!av[i])
            continue;
        if (av[i]->isRunning())
            return;
    }
    end = true; //(!audio_thread || !audio_thread->isRunning()) &&
#endif
}

bool FFmpegThread::waitForStarted(int msec)
{
    if (!sem.tryAcquire(1, msec > 0 ? msec : std::numeric_limits<int>::max()))
        return false;
    sem.release(1); //ensure other waitForStarted() calls continue
    return true;
}
#if 0
void FFmpegThread::run()
{
    m_buffering = false;
    end = false;
    if (audio_thread && !audio_thread->isRunning())
        audio_thread->start(QThread::HighPriority);
#if 0
    if (video_thread && !video_thread->isRunning())
        video_thread->start();
#endif
    int stream = 0;
    Packet pkt;
    pause(false);
    qDebug("get av queue a/v thread = %p %p", audio_thread, video_thread);
    PacketBuffer *aqueue = audio_thread ? audio_thread->packetQueue() : 0;
    PacketBuffer *vqueue = video_thread ? video_thread->packetQueue() : 0;
    // aqueue as a primary buffer: music with/without cover
    AVThread* thread = !video_thread || (audio_thread && demuxer->hasAttacedPicture()) ? audio_thread : video_thread;
    m_buffer = thread->packetQueue();
    const qint64 buf2 = aqueue ? aqueue->bufferValue() : 1; // TODO: may be changed by user. Deal with audio track change
    if (aqueue) {
        aqueue->clear();
        aqueue->setBlocking(true);
    }
    if (vqueue) {
        vqueue->clear();
        vqueue->setBlocking(true);
    }
    connect(thread, SIGNAL(seekFinished(qint64)), this, SIGNAL(seekFinished(qint64)), Qt::DirectConnection);
    seek_tasks.clear();
    int was_end = 0;
    if (ademuxer) {
        ademuxer->seek(0LL);
    }
    qreal last_apts = 0;
    qreal last_vpts = 0;

    AutoSemt as(&sem);
    Q_UNUSED(as);
    while (!end) {
        processNextSeekTask();
        //vthread maybe changed by AVPlayer.setPriority() from no dec case
        vqueue = video_thread ? video_thread->packetQueue() : 0;
        if (demuxer->atEnd()) {
            // if avthread may skip 1st eof packet because of a/v sync
            const int kMaxEof = 1;//if buffer packet, we can use qMax(aqueue->bufferValue(), vqueue->bufferValue()) and not call blockEmpty(false);
            if (aqueue && (!was_end || aqueue->isEmpty())) {
                if (was_end < kMaxEof)
                    aqueue->put(Packet::createEOF());
                const qreal dpts = last_vpts - last_apts;
                if (dpts > 0.1) {
                    Packet fake_apkt;
                    fake_apkt.duration = last_vpts - qMin(thread->clock()->videoTime(), thread->clock()->value()); // FIXME: when clock value < 0?
                    qDebug("audio is too short than video: %.3f, fake_apkt.duration: %.3f", dpts, fake_apkt.duration);
                    last_apts = last_vpts = 0; // if not reset to 0, for example real eof pts, then no fake apkt after seek because dpts < 0
                    aqueue->put(fake_apkt);
                }
                aqueue->blockEmpty(was_end >= kMaxEof); // do not block if buffer is not enough. block again on seek
            }
            if (vqueue && (!was_end || vqueue->isEmpty())) {
                if (was_end < kMaxEof)
                    vqueue->put(Packet::createEOF());
                vqueue->blockEmpty(was_end >= kMaxEof);
            }
            if (m_buffering) {
                m_buffering = false;
                Q_EMIT mediaStatusChanged(QtAV::BufferedMedia);
            }
            was_end = qMin(was_end + 1, kMaxEof);
            bool exit_thread = !user_paused;
            if (aqueue)
                exit_thread &= aqueue->isEmpty();
            if (vqueue)
                exit_thread &= vqueue->isEmpty();
            if (exit_thread) {
                if (!(mediaEndAction() & MediaEndAction_Pause))
                    break;
                pause(true);
                Q_EMIT requestClockPause(true);
                if (aqueue)
                    aqueue->blockEmpty(true);
                if (vqueue)
                    vqueue->blockEmpty(true);
            }
            // wait for a/v thread finished
            msleep(100);
            continue;
        }
        if (demuxer->mediaStatus() == StalledMedia) {
            qDebug("stalled media. exiting demuxing thread");
            break;
        }
        was_end = 0;
        if (tryPause()) {
            continue; //the queue is empty and will block
        }
        updateBufferState();
        if (!demuxer->readFrame()) {
            continue;
        }
        stream = demuxer->stream();
        pkt = demuxer->packet();
        Packet apkt;
        bool audio_has_pic = demuxer->hasAttacedPicture();
        int a_ext = 0;
        if (ademuxer) {
            QMutexLocker locker(&buffer_mutex);
            Q_UNUSED(locker);
            if (ademuxer) {
                a_ext = -1;
                audio_has_pic = ademuxer->hasAttacedPicture();
                // FIXME: buffer full but buffering!!!
                // avoid read external track everytime. aqueue may not block full
                // vqueue will not block if aqueue is not enough
                if (!aqueue->isFull() || aqueue->isBuffering()) {
                    if (ademuxer->readFrame()) {
                        if (ademuxer->stream() == ademuxer->audioStream()) {
                            a_ext = 1;
                            apkt = ademuxer->packet();
                        }
                    }
                    // no continue otherwise. ademuxer finished earlier than demuxer
                }
            }
        }
        //qDebug("vqueue: %d, aqueue: %d/isbuffering %d isfull: %d, buffer: %d/%d", vqueue->size(), aqueue->size(), aqueue->isBuffering(), aqueue->isFull(), aqueue->buffered(), aqueue->bufferValue());

        //QMutexLocker locker(&buffer_mutex); //TODO: seems we do not need to lock
        //Q_UNUSED(locker);
        /*1 is empty but another is enough, then do not block to
          ensure the empty one can put packets immediatly.
          But usually it will not happen, why?
        */
        /* demux thread will be blocked only when 1 queue is full and still put
         * if vqueue is full and aqueue becomes empty, then demux thread
         * will be blocked. so we should wake up another queue when empty(or threshold?).
         * TODO: the video stream and audio stream may be group by group. provide it
         * stream data: aaaaaaavvvvvvvaaaaaaaavvvvvvvvvaaaaaa, it happens
         * stream data: aavavvavvavavavavavavavavvvaavavavava, it's ok
         */
        //TODO: use cache queue, take from cache queue if not empty?
        const bool a_internal = stream == demuxer->audioStream();
        if (a_internal || a_ext > 0) {//apkt.isValid()) 
            if (a_internal && !a_ext) // internal is always read even if external audio used
                apkt = demuxer->packet();
            last_apts = apkt.pts;
            /* if vqueue if not blocked and full, and aqueue is empty, then put to
             * vqueue will block demuex thread
             */
            if (aqueue) {
                if (!audio_thread || !audio_thread->isRunning()) {
                    aqueue->clear();
                    continue;
                }
                // must ensure bufferValue set correctly before continue
                if (m_buffer != aqueue)
                    aqueue->setBufferValue(m_buffer->isBuffering() ? std::numeric_limits<qint64>::max() : buf2);
                // always block full if no vqueue because empty callback may set false
                // attached picture is cover for song, 1 frame
                aqueue->blockFull(!video_thread || !video_thread->isRunning() || !vqueue || audio_has_pic);
                // external audio: a_ext < 0, stream = audio_idx=>put invalid packet
                if (a_ext >= 0)
                    aqueue->put(apkt); //affect video_thread
            }
        }
        // always check video stream if use external audio
        if (stream == demuxer->videoStream()) {
            if (vqueue) {
#if 0
                if (!video_thread || !video_thread->isRunning()) {
                    vqueue->clear();
                    continue;
                }
#endif
                vqueue->blockFull(!audio_thread || !audio_thread->isRunning() || !aqueue || aqueue->isEnough());
                vqueue->put(pkt); //affect audio_thread
                last_vpts = pkt.pts;
            }
        } else if (demuxer->subtitleStreams().contains(stream)) { //subtitle
            Q_EMIT internalSubtitlePacketRead(demuxer->subtitleStreams().indexOf(stream), pkt);
        } else {
            continue;
        }
    }
    m_buffering = false;
    m_buffer = 0;
    while (audio_thread && audio_thread->isRunning()) {
        qDebug("waiting audio thread.......");
        Packet quit_pkt(Packet::createEOF());
        quit_pkt.position = 0;
        aqueue->put(quit_pkt);
        aqueue->blockEmpty(false); //FIXME: why need this
        audio_thread->pause(false);
        audio_thread->wait(500);
    }
    while (video_thread && video_thread->isRunning()) {
        qDebug("waiting video thread.......");
        Packet quit_pkt(Packet::createEOF());
        quit_pkt.position = 0;
        vqueue->put(quit_pkt);
        vqueue->blockEmpty(false);
        video_thread->pause(false);
        video_thread->wait(500);
    }
    thread->disconnect(this, SIGNAL(seekFinished(qint64)));
    qDebug("Demux thread stops running....");
    if (demuxer->atEnd())
        Q_EMIT mediaStatusChanged(QtAV::EndOfMedia);
    else
        Q_EMIT mediaStatusChanged(QtAV::StalledMedia);
}
#endif
bool FFmpegThread::tryPause(unsigned long timeout)
{
    if (!paused)
        return false;
    QMutexLocker lock(&buffer_mutex);
    Q_UNUSED(lock);
    cond.wait(&buffer_mutex, timeout);
    return true;
}
#if 0
void FFmpegThread::run() //storm
{
    m_buffering = false;
    end = false;
    if (video_thread && !video_thread->isRunning())
        video_thread->start();

    int stream = 0;
    Packet pkt;
    pause(false);
    qDebug("get av queue a/v thread = %p %p", audio_thread, video_thread);
    PacketBuffer *aqueue = audio_thread ? audio_thread->packetQueue() : 0;
    PacketBuffer *vqueue = video_thread ? video_thread->packetQueue() : 0;
    // aqueue as a primary buffer: music with/without cover
    AVThread* thread = !video_thread || (audio_thread && demuxer->hasAttacedPicture()) ? audio_thread : video_thread;
    m_buffer = thread->packetQueue();
    if (vqueue) {
        vqueue->clear();
        vqueue->setBlocking(true);
    }
    connect(thread, SIGNAL(seekFinished(qint64)), this, SIGNAL(seekFinished(qint64)), Qt::DirectConnection);
    seek_tasks.clear();
    int was_end = 0;

    while (!end) {
        processNextSeekTask();
        //vthread maybe changed by AVPlayer.setPriority() from no dec case
        vqueue = video_thread ? video_thread->packetQueue() : 0;
        if (demuxer->atEnd()) {
            // if avthread may skip 1st eof packet because of a/v sync
            const int kMaxEof = 1;//if buffer packet, we can use qMax(aqueue->bufferValue(), vqueue->bufferValue()) and not call blockEmpty(false);
            if (vqueue && (!was_end || vqueue->isEmpty())) {
                if (was_end < kMaxEof)
                    vqueue->put(Packet::createEOF());
                vqueue->blockEmpty(was_end >= kMaxEof);
            }
            if (m_buffering) {
                m_buffering = false;
                Q_EMIT mediaStatusChanged(QtAV::BufferedMedia);
            }
            was_end = qMin(was_end + 1, kMaxEof);
            bool exit_thread = !user_paused;
            if (vqueue)
                exit_thread &= vqueue->isEmpty();
            if (exit_thread) {
                if (!(mediaEndAction() & MediaEndAction_Pause))
                    break;
                pause(true);
                Q_EMIT requestClockPause(true);
                if (vqueue)
                    vqueue->blockEmpty(true);
            }
            // wait for a/v thread finished
            msleep(100);
            continue;
        }
        if (demuxer->mediaStatus() == StalledMedia) {
            qDebug("stalled media. exiting demuxing thread");
            break;
        }
        was_end = 0;
        if (tryPause()) {
            continue; //the queue is empty and will block
        }
        updateBufferState();
        if (!demuxer->readFrame()) {
            continue;
        }
        stream = demuxer->stream();
        pkt = demuxer->packet();
        bool audio_has_pic = demuxer->hasAttacedPicture();
        //qDebug("vqueue: %d, aqueue: %d/isbuffering %d isfull: %d, buffer: %d/%d", vqueue->size(), aqueue->size(), aqueue->isBuffering(), aqueue->isFull(), aqueue->buffered(), aqueue->bufferValue());

        // always check video stream if use external audio
        if (stream == demuxer->videoStream()) {
            if (vqueue) {
                if (!video_thread || !video_thread->isRunning()) {
                    vqueue->clear();
                    continue;
                }
                vqueue->blockFull(!audio_thread || !audio_thread->isRunning() || !aqueue || aqueue->isEnough());
                vqueue->put(pkt); //affect audio_thread
            }
        } else if (demuxer->subtitleStreams().contains(stream)) { //subtitle
            Q_EMIT internalSubtitlePacketRead(demuxer->subtitleStreams().indexOf(stream), pkt);
        } else {
            continue;
        }
    }
    m_buffering = false;
    m_buffer = 0;
    while (video_thread && video_thread->isRunning()) {
        qDebug("waiting video thread.......");
        Packet quit_pkt(Packet::createEOF());
        quit_pkt.position = 0;
        vqueue->put(quit_pkt);
        vqueue->blockEmpty(false);
        video_thread->pause(false);
        video_thread->wait(500);
    }
    thread->disconnect(this, SIGNAL(seekFinished(qint64)));
    qDebug("Demux thread stops running....");
    if (demuxer->atEnd())
        Q_EMIT mediaStatusChanged(QtAV::EndOfMedia);
    else
        Q_EMIT mediaStatusChanged(QtAV::StalledMedia);
}
#endif 
//end AVDemuxMyThread
#if 0
void FFmpegThread::run()
{
    DPTR_D(FFmpegThread);
    if (!d.dec || !d.dec->isAvailable() )
        return;
    //not neccesary context is managed by filters.
    VideoDecoder *dec = static_cast<VideoDecoder*>(d.dec);
    Packet pkt;
    const char* pkt_data = NULL; // workaround for libav9 decode fail but error code >= 0
    while (!d.stop) {
        ///processNextTask();
        ////if(!pkt.isValid() && !pkt.isEOF()) 
        { // can't seek back if eof packet is read
            pkt = d.packets.take(); //wait to dequeue
           // TODO: push pts history here and reorder
        }
        if (!dec->decode(pkt)) {
            continue;
        }
        VideoFrame frame = dec->frame();
        if (!frame.isValid()) {
            qWarning("invalid video frame from decoder. undecoded data size: %d", pkt.data.size());
            if (pkt_data == pkt.data.constData()) //FIXME: for libav9. what about other versions?
                pkt = Packet();
            else
                pkt_data = pkt.data.constData();
            continue;
        }
        ///qDebug("pts0: %f, pkt_pts: %f pkt_dts: %f, clock: %d\n", d.render_pts0, pkt.pts, pkt.dts, d.clock->clockType()); {QImage img=frame.toImage();static int idx=0;++idx;if(idx>25 && idx<=60){printf("FFmpegThread::run img idx: %d  isNull: %d pts: %f\n",idx,img.isNull(),frame.timestamp());fflush(0);img.save(QString("selfimages%1.jpg").arg(idx));}}// storm
        pkt_data = pkt.data.constData();
        if (frame.timestamp() < 0)
            frame.setTimestamp(pkt.pts); // pkt.pts is wrong. >= real timestamp
        // no return even if d.stop is true. ensure frame is displayed. otherwise playing an image may be failed to display
        //if (!deliverVideoFrame(frame))
        //    continue;
#if 1
        static int idx=0;++idx;
        while(m_svframe.size()!=0)
        {
            QThread::msleep(20);
        }
        ///if(idx>25 && idx<=60 )
            m_svframe.enqueue(frame);
#endif
        ////QThread::msleep(20);
    }
    d.packets.clear();
    qDebug("Video thread stops running...");
}
#endif
#if 1
void FFmpegThread::run()
{
//AVDemuxMyThread
    m_buffering = false;
    end = false;
    int stream = 0;
    Packet pkt;
    pause(false);
    qDebug("get av queue a/v thread = %p %p", audio_thread, video_thread);
    PacketBuffer *aqueue = 0;
    PacketBuffer *vqueue = packetQueue();

    AVThread* thread = this;
    m_buffer = thread->packetQueue();
    const qint64 buf2 =  1; // TODO: may be changed by user. Deal with audio track change
    if (vqueue) {
        vqueue->clear();
        vqueue->setBlocking(true);
    }
    int was_end = 0;
//end AVDemuxMyThread
    DPTR_D(FFmpegThread);
    if (!d.dec || !d.dec->isAvailable() )
        return;
    //not neccesary context is managed by filters.
    VideoDecoder *dec = static_cast<VideoDecoder*>(d.dec);
    ///Packet pkt;
    const char* pkt_data = NULL; // workaround for libav9 decode fail but error code >= 0
    while (!d.stop) {
#if 0
        ///processNextTask();
        ////if(!pkt.isValid() && !pkt.isEOF()) 
        { // can't seek back if eof packet is read
            pkt = d.packets.take(); //wait to dequeue
           // TODO: push pts history here and reorder
        }
#endif
//AVDemuxMyThread
        if (demuxer->atEnd()) {
            // if avthread may skip 1st eof packet because of a/v sync
            const int kMaxEof = 1;//if buffer packet, we can use qMax(aqueue->bufferValue(), vqueue->bufferValue()) and not call blockEmpty(false);
            if (vqueue && (!was_end || vqueue->isEmpty())) {
                if (was_end < kMaxEof)
                    vqueue->put(Packet::createEOF());
                vqueue->blockEmpty(was_end >= kMaxEof);
            }
            if (m_buffering) {
                m_buffering = false;
                Q_EMIT mediaStatusChanged(QtAV::BufferedMedia);
            }
            was_end = qMin(was_end + 1, kMaxEof);
            bool exit_thread = !user_paused;
            if (vqueue)
                exit_thread &= vqueue->isEmpty();
            if (exit_thread) {
                if (!(mediaEndAction() & MediaEndAction_Pause))
                    break;
                pause(true);
                Q_EMIT requestClockPause(true);
                if (vqueue)
                    vqueue->blockEmpty(true);
            }
            // wait for a/v thread finished
            msleep(100);
            continue;
        }
        if (demuxer->mediaStatus() == StalledMedia) {
            qDebug("stalled media. exiting demuxing thread");
            break;
        }
        was_end = 0;
        if (tryPause()) {
            continue; //the queue is empty and will block
        }
        updateBufferState();
        if (!demuxer->readFrame()) {
            continue;
        }
        stream = demuxer->stream();
        pkt = demuxer->packet();
        if (stream == demuxer->videoStream()) {
#if 0
            if (vqueue) {
                if (!video_thread || !video_thread->isRunning()) {
                    vqueue->clear();
                    continue;
                }
                vqueue->blockFull(!audio_thread || !audio_thread->isRunning() || !aqueue || aqueue->isEnough());
                vqueue->put(pkt); //affect audio_thread
            }
#endif
        } else if (demuxer->subtitleStreams().contains(stream)) { //subtitle
            Q_EMIT internalSubtitlePacketRead(demuxer->subtitleStreams().indexOf(stream), pkt);
        } else {
            continue;
        }
//end AVDemuxMyThread
        if (!dec->decode(pkt)) {
            continue;
        }
        VideoFrame frame = dec->frame();
        if (!frame.isValid()) {
            qWarning("invalid video frame from decoder. undecoded data size: %d", pkt.data.size());
            if (pkt_data == pkt.data.constData()) //FIXME: for libav9. what about other versions?
                pkt = Packet();
            else
                pkt_data = pkt.data.constData();
            continue;
        }
        ///qDebug("pts0: %f, pkt_pts: %f pkt_dts: %f, clock: %d\n", d.render_pts0, pkt.pts, pkt.dts, d.clock->clockType()); {QImage img=frame.toImage();static int idx=0;++idx;if(idx>25 && idx<=60){printf("FFmpegThread::run img idx: %d  isNull: %d pts: %f\n",idx,img.isNull(),frame.timestamp());fflush(0);img.save(QString("selfimages%1.jpg").arg(idx));}}// storm
        pkt_data = pkt.data.constData();
        if (frame.timestamp() < 0)
            frame.setTimestamp(pkt.pts); // pkt.pts is wrong. >= real timestamp
        // no return even if d.stop is true. ensure frame is displayed. otherwise playing an image may be failed to display
        //if (!deliverVideoFrame(frame))
        //    continue;
#if 1
        static int idx=0;++idx;
        while(m_svframe.size()!=0)
        {
            QThread::msleep(200);
        }
        ///if(idx>25 && idx<=60 )
            m_svframe.enqueue(frame);
#endif
        ////QThread::msleep(20);
    }
    d.packets.clear();
    qDebug("Video thread stops running...");
}
#endif

} //namespace QtAV
