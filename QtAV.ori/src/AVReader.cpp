#include "AVReader.h"
#include <QDebug>
///#include "filter/FilterManager.h"
///#include "output/OutputSet.h"
#include "QtAV/AudioDecoder.h"
#include "QtAV/AudioFormat.h"
#include "QtAV/AudioResampler.h"
//#include "QtAV/MediaIO.h"
//#include "QtAV/VideoCapture.h"
#include "QtAV/private/AVCompat.h"
#if AV_MODULE_CHECK(LIBAVFORMAT, 55, 18, 0, 39, 100)
extern "C" {
#include <libavutil/display.h>
}
#endif

//avplayer
#include <limits>

#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include <QtCore/QDir>
#include <QtCore/QIODevice>
#include <QtCore/QThreadPool>
#include <QtCore/QTimer>
#include "QtAV/AVDemuxer.h"
#include "QtAV/Packet.h"
#include "QtAV/AudioDecoder.h"
//#include "QtAV/MediaIO.h"
///#include "QtAV/VideoRenderer.h"
#include "QtAV/AVClock.h"
//#include "QtAV/VideoCapture.h"
//#include "QtAV/VideoCapture.h"
///#include "filter/FilterManager.h"
///#include "output/OutputSet.h"
#include "AudioThread.h"
#include "FFmpegThread.h"
#include "AVDemuxMyThread.h"
#include "QtAV/private/AVCompat.h"
#include "utils/internal.h"
extern "C" {
#include <libavutil/mathematics.h>
}

#define EOF_ISSUE_SOLVED 0

namespace QtAV {
//avplayer
#if 0
namespace {
static const struct RegisterMetaTypes {
    inline RegisterMetaTypes() {
        qRegisterMetaType<QtAV::AVReader::SState>(); // required by invoke() parameters
    }
} _registerMetaTypes;
} //namespace
#endif
static const qint64 kSeekMS = 10000;

Q_GLOBAL_STATIC(QThreadPool, loaderThreadPool)

/// Supported input protocols. A static string list
const QStringList& AVReader::supportedProtocols()
{
    return AVDemuxer::supportedProtocols();
}
//end avplayer


int computeNotifyPrecision(qint64 duration, qreal fps)
{
    if (duration <= 0 || duration > 60*1000) // no duration or 10min
        return 500;
    if (duration > 20*1000)
        return 250;
    int dt = 500;
    if (fps > 1) {
        dt = qMin(250, int(qreal(dt*2)/fps));
    } else {
        dt = duration / 80; //<= 250
    }
    return qMax(20, dt);
}

static bool correct_audio_channels(AVCodecContext *ctx) {return false;
}

AVReader::AVReader(QObject *parent) :
    QObject(parent)
    // avplayerprivate
    , auto_load(false)
    , async_load(false)
    , bloaded(false)
    , relative_time_mode(true)
    , media_start_pts(0)
    , media_end(kInvalidPosition)
    , reset_state(true)
    , start_position(0)
    , stop_position(kInvalidPosition)
    , start_position_norm(0)
    , stop_position_norm(kInvalidPosition)
    , repeat_max(0)
    , repeat_current(-1)
    , timer_id(-1)
    , audio_track(0)
    , video_track(0)
    , subtitle_track(0)
    , buffer_mode(BufferPackets)
    , buffer_value(-1)
    , read_thread(0)
    , clock(new AVClock(AVClock::AudioClock))
    //, vo(0)
    ///, ao(new AudioOutput())
    , adec(0)
    , vdec(0)
    , athread(0)
    , vthread(0)
    //, vcapture(0)
    , speed(1.0)
    //, vos(0)
    //, aos(0)
    , fbrightness(0)
    , fcontrast(0)
    , fsaturation(0)
    , seeking(false)
    , seek_type(AccurateSeek)
    , interrupt_timeout(30000)
    , force_fps(0)
    , notify_interval(-500)
    , status(NoMedia)
    , vstate(AVReader::StoppedState)
    , end_action(MediaEndAction_Default)
{

    // avplayerprivate
    demuxer.setInterruptTimeout(interrupt_timeout);
    /*
     * reset_state = true;
     * must be the same value at the end of stop(), and must be different from value in
     * stopFromDemuxerThread()(which is false), so the initial value must be true
     */

    vc_ids
//#if QTAV_HAVE(DXVA)
            << VideoDecoderId_DXVA;
//#endif //QTAV_HAVE(DXVA)
#if QTAV_HAVE(VAAPI)
            //<< VideoDecoderId_VAAPI
#endif //QTAV_HAVE(VAAPI)
#if QTAV_HAVE(CEDARV)
            //<< VideoDecoderId_Cedarv
#endif //QTAV_HAVE(CEDARV)
            //<< VideoDecoderId_FFmpeg;
    //avplayer
#if 0
    read_thread = new AVDemuxMyThread(this);
    read_thread->setDemuxer(&demuxer);
#endif
#if 0
    vos = new OutputSet(NULL); //storm
    aos = new OutputSet(NULL);
    connect(this, SIGNAL(started()), this, SLOT(onStarted()));
    /*
     * call stop() before the window(vo) closed to stop the waitcondition
     * If close the vo widget, the the vo may destroy before waking up.
     */
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuitApp()));
    //clock->setClockType(AVClock::ExternalClock);
    connect(&demuxer, SIGNAL(started()), masterClock(), SLOT(start()));
    connect(&demuxer, SIGNAL(error(QtAV::AVError)), this, SIGNAL(error(QtAV::AVError)));
    connect(&demuxer, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)), this, SLOT(updateMediaStatus(QtAV::MediaStatus)), Qt::DirectConnection);
    connect(&demuxer, SIGNAL(loaded()), this, SIGNAL(loaded()));
    connect(&demuxer, SIGNAL(seekableChanged()), this, SIGNAL(seekableChanged()));
    //direct connection can not sure slot order?
    connect(read_thread, SIGNAL(finished()), this, SLOT(stopFromDemuxerThread()), Qt::DirectConnection);
    connect(read_thread, SIGNAL(requestClockPause(bool)), masterClock(), SLOT(pause(bool)), Qt::DirectConnection);
    connect(read_thread, SIGNAL(mediaStatusChanged(QtAV::MediaStatus)), this, SLOT(updateMediaStatus(QtAV::MediaStatus)));
    connect(read_thread, SIGNAL(bufferProgressChanged(qreal)), this, SIGNAL(bufferProgressChanged(qreal)));
    connect(read_thread, SIGNAL(seekFinished(qint64)), this, SLOT(onSeekFinished(qint64)), Qt::DirectConnection);
    connect(read_thread, SIGNAL(internalSubtitlePacketRead(int, QtAV::Packet)), this, SIGNAL(internalSubtitlePacketRead(int, QtAV::Packet)), Qt::DirectConnection);
    //end avplayer
#endif
    ///vcapture = new VideoCapture(this);
}

AVReader::~AVReader()
{
#if 0
    // avplayerprivate
    if (ao) {
        delete ao;
        ao = 0;
    }
#endif
    if (adec) {
        delete adec;
        adec = 0;
    }
    if (vdec) {
        delete vdec;
        vdec = 0;
    }
#if 0
    if (vos) {
        vos->clearOutputs();
        delete vos;
        vos = 0;
    }
    if (aos) {
        aos->clearOutputs();
        delete aos;
        aos = 0;
    }
#endif
#if 0
    if (vcapture) {
        delete vcapture;
        vcapture = 0;
    }
#endif
    if (clock) {
        delete clock;
        clock = 0;
    }
    if (read_thread) {
        delete read_thread;
        read_thread = 0;
    }
}
#if 0
void AVReader::setFile(const QString &path)
{
    m_path=path;
    load();
}
#endif
#if 0
bool AVReader::load()
{
    demuxer.unload();
    demuxer.setMedia(m_path);
    if (!demuxer.load()) {
        qDebug()<<"AVReader::load error";
        printf("AVReader::load error\n");fflush(0);
        return false;
    }
    printf("AVReader::load duration: %ld\n", demuxer.duration());fflush(0);
    qDebug()<<"AVReader::load: "<<demuxer.duration();
    return true;
}
#endif
VideoFrame AVReader::GetFrame(int64_t request_frame)
{
    VideoFrame f;//=vthread->GetFrame(request_frame);
    if(vthread->m_svframe.size()>0)
        f = vthread->m_svframe.dequeue();
    return f;
}

//avplayerprivate
bool AVReader::checkSourceChange()
{return true;
}

void AVReader::updateNotifyInterval()
{return;
}

void AVReader::applyFrameRate()
{return;
}

void AVReader::initStatistics()
{
    initBaseStatistics();
    initAudioStatistics(demuxer.audioStream());
    initVideoStatistics(demuxer.videoStream());
    //initSubtitleStatistics(demuxer.subtitleStream());
}

//TODO: av_guess_frame_rate in latest ffmpeg
void AVReader::initBaseStatistics()
{
    statistics.reset();
    statistics.url = current_source.type() == QVariant::String ? current_source.toString() : QString();
    statistics.start_time = QTime(0, 0, 0).addMSecs(int(demuxer.startTime()));
    statistics.duration = QTime(0, 0, 0).addMSecs((int)demuxer.duration());
    AVFormatContext *fmt_ctx = demuxer.formatContext();
    if (!fmt_ctx) {
        qWarning("demuxer.formatContext()==null. internal error");
        updateNotifyInterval();
        return;
    }
    statistics.bit_rate = fmt_ctx->bit_rate;
    statistics.format = QString().sprintf("%s - %s", fmt_ctx->iformat->name, fmt_ctx->iformat->long_name);
    //AV_TIME_BASE_Q: msvc error C2143
    //fmt_ctx->duration may be AV_NOPTS_VALUE. AVDemuxer.duration deals with this case
    AVDictionaryEntry *tag = NULL;
    while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        statistics.metadata.insert(QString::fromUtf8(tag->key), QString::fromUtf8(tag->value));
    }
    updateNotifyInterval();
}

void AVReader::initCommonStatistics(int s, Statistics::Common *st, AVCodecContext *avctx)
{
    AVFormatContext *fmt_ctx = demuxer.formatContext();
    if (!fmt_ctx) {
        qWarning("demuxer.formatContext()==null. internal error");
        return;
    }
    AVStream *stream = fmt_ctx->streams[s];
    qDebug("stream: %d, duration=%lld (%lld ms), time_base=%f", s, stream->duration, qint64(qreal(stream->duration)*av_q2d(stream->time_base)*1000.0), av_q2d(stream->time_base));
    // AVCodecContext.codec_name is deprecated. use avcodec_get_name. check null avctx->codec?
    st->codec = QLatin1String(avcodec_get_name(avctx->codec_id));
    st->codec_long = QLatin1String(get_codec_long_name(avctx->codec_id));
    st->total_time = QTime(0, 0, 0).addMSecs(stream->duration == (qint64)AV_NOPTS_VALUE ? 0 : int(qreal(stream->duration)*av_q2d(stream->time_base)*1000.0));
    st->start_time = QTime(0, 0, 0).addMSecs(stream->start_time == (qint64)AV_NOPTS_VALUE ? 0 : int(qreal(stream->start_time)*av_q2d(stream->time_base)*1000.0));
    qDebug("codec: %s(%s)", qPrintable(st->codec), qPrintable(st->codec_long));
    st->bit_rate = avctx->bit_rate; //fmt_ctx
    st->frames = stream->nb_frames;
    if (stream->avg_frame_rate.den && stream->avg_frame_rate.num)
        st->frame_rate = av_q2d(stream->avg_frame_rate);
#if (defined FF_API_R_FRAME_RATE && FF_API_R_FRAME_RATE) //removed in libav10
    //FIXME: which 1 should we choose? avg_frame_rate may be nan, r_frame_rate may be wrong(guessed value)
    else if (stream->r_frame_rate.den && stream->r_frame_rate.num) {
        st->frame_rate = av_q2d(stream->r_frame_rate);
        qDebug("%d/%d", stream->r_frame_rate.num, stream->r_frame_rate.den);
    }
#endif //FF_API_R_FRAME_RATE
    //http://ffmpeg.org/faq.html#AVStream_002er_005fframe_005frate-is-wrong_002c-it-is-much-larger-than-the-frame-rate_002e
    //http://libav-users.943685.n4.nabble.com/Libav-user-Reading-correct-frame-rate-fps-of-input-video-td4657666.html
    //qDebug("time: %f~%f, nb_frames=%lld", st->start_time, st->total_time, stream->nb_frames); //why crash on mac? av_q2d({0,0})?
    AVDictionaryEntry *tag = NULL;
    while ((tag = av_dict_get(stream->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        st->metadata.insert(QString::fromUtf8(tag->key), QString::fromUtf8(tag->value));
    }
}

void AVReader::initAudioStatistics(int s)
{return;
}

void AVReader::initVideoStatistics(int s)
{
    AVCodecContext *avctx = demuxer.videoCodecContext();
    statistics.video = Statistics::Common();
    statistics.video_only = Statistics::VideoOnly();
    if (!avctx)
        return;
    statistics.video.available = s == demuxer.videoStream();
    initCommonStatistics(s, &statistics.video, avctx);
    if (vdec) {
        statistics.video.decoder = vdec->name();
        statistics.video.decoder_detail = vdec->description();
    }
    statistics.video_only.coded_height = avctx->coded_height;
    statistics.video_only.coded_width = avctx->coded_width;
    statistics.video_only.gop_size = avctx->gop_size;
    statistics.video_only.pix_fmt = QLatin1String(av_get_pix_fmt_name(avctx->pix_fmt));
    statistics.video_only.height = avctx->height;
    statistics.video_only.width = avctx->width;
    statistics.video_only.rotate = 0;
#if AV_MODULE_CHECK(LIBAVFORMAT, 55, 18, 0, 39, 100)
    quint8 *sd = av_stream_get_side_data(demuxer.formatContext()->streams[s], AV_PKT_DATA_DISPLAYMATRIX, NULL);
    if (sd) {
        double r = av_display_rotation_get((qint32*)sd);
        if (!qIsNaN(r))
            statistics.video_only.rotate = ((int)r + 360) % 360;
    }
#endif
}
// notify statistics change after audio/video thread is set
bool AVReader::setupAudioThread( AVReader *player)
{return false;
}

QVariantList AVReader::getTracksInfo(AVDemuxer *demuxer, AVDemuxer::StreamType st)
{
    QVariantList info;
    if (!demuxer)
        return info;
    QList<int> streams;
    switch (st) {
    case AVDemuxer::AudioStream:
        streams = demuxer->audioStreams();
        break;
    case AVDemuxer::SubtitleStream:
        streams = demuxer->subtitleStreams();
        break;
    case AVDemuxer::VideoStream:
        streams = demuxer->videoStreams();
    default:
        break;
    }
    if (streams.isEmpty())
        return info;
    foreach (int s, streams) {
        QVariantMap t;
        t[QStringLiteral("id")] = info.size();
        t[QStringLiteral("file")] = demuxer->fileName();
        t[QStringLiteral("stream_index")] = QVariant(s);

        AVStream *stream = demuxer->formatContext()->streams[s];
        AVCodecContext *ctx = stream->codec;
        if (ctx) {
            t[QStringLiteral("codec")] = QByteArray(avcodec_descriptor_get(ctx->codec_id)->name);
            if (ctx->extradata)
                t[QStringLiteral("extra")] = QByteArray((const char*)ctx->extradata, ctx->extradata_size);
        }
        AVDictionaryEntry *tag = av_dict_get(stream->metadata, "language", NULL, 0);
        if (!tag)
            tag = av_dict_get(stream->metadata, "lang", NULL, 0);
        if (tag) {
            t[QStringLiteral("language")] = QString::fromUtf8(tag->value);
        }
        tag = av_dict_get(stream->metadata, "title", NULL, 0);
        if (tag) {
            t[QStringLiteral("title")] = QString::fromUtf8(tag->value);
        }
        info.push_back(t);
    }
    //QVariantMap t;
    //t[QStringLiteral("id")] = -1;
    //info.prepend(t);
    return info;
}

bool AVReader::applySubtitleStream(int n,  AVReader *player)
{return false;
}

bool AVReader::tryApplyDecoderPriority(AVReader *player)
{return false;
}

bool AVReader::setupVideoThread(AVReader *player)
{
    demuxer.setStreamIndex(AVDemuxer::VideoStream, video_track);
    // pause demuxer, clear queues, set demuxer stream, set decoder, set ao, resume
    // clear packets before stream changed
    if (vthread) {
        vthread->packetQueue()->clear();
        vthread->setDecoder(0);
    }
    AVCodecContext *avctx = demuxer.videoCodecContext();
    if (!avctx) {
        return false;
    }
    if (vdec) {
        vdec->disconnect();
        delete vdec;
        vdec = 0;
    }
    foreach(VideoDecoderId vid, vc_ids) {
        qDebug("**********trying video decoder: %s...", VideoDecoder::name(vid));
        VideoDecoder *vd = VideoDecoder::create(vid);
        if (!vd) {
            continue;
        }
        //vd->isAvailable() //TODO: the value is wrong now
        vd->setCodecContext(avctx);
        //vd->setOptions(vc_opt); //storm
        vd->setProperty("copyMode", "ZeroCopy"); //storm
        if (vd->open()) {
            vdec = vd;
            //qDebug("**************Video decoder found:%p", vdec);
            qDebug("**************setupVideoThread Video decoder found:%p name: %s", vd, VideoDecoder::name(vid)); //storm
            break;
        }
        delete vd;
    }
    if (!vdec) {
        // DO NOT emit error signals in VideoDecoder::open(). 1 signal is enough
        AVError e(AVError::VideoCodecNotFound);
        qWarning() << e.string();
        emit player->error(e);
        return false;
    }
    QObject::connect(vdec, SIGNAL(error(QtAV::AVError)), player, SIGNAL(error(QtAV::AVError)));
    if (!vthread) {
        vthread = new FFmpegThread(player);
        vthread->setDemuxer(&demuxer);
        vthread->setClock(clock);
        vthread->setStatistics(&statistics);
        ///vthread->setVideoCapture(vcapture);
        ///vthread->setOutputSet(vos);
        ////read_thread->setVideoThread(vthread);
#if 0
        QList<Filter*> filters = FilterManager::instance().videoFilters(NULL); //storm
        if (filters.size() > 0) {
            foreach (Filter *filter, filters) {
                vthread->installFilter(filter);
            }
        }
#endif
        /////QObject::connect(vthread, SIGNAL(finished()), player, SLOT(tryClearVideoRenderers()), Qt::DirectConnection);
    }

    // we set the thre state before the thread start
    // as it maybe clear after by AVDemuxMyThread starting
    vthread->resetState();
    vthread->setDecoder(vdec);

    vthread->setBrightness(fbrightness);
    vthread->setContrast(fcontrast);
    vthread->setSaturation(fsaturation);
    updateBufferValue(vthread->packetQueue());
    initVideoStatistics(demuxer.videoStream());

    return true;
}

// TODO: set to a lower value when buffering
void AVReader::updateBufferValue(PacketBuffer* buf)
{
    const bool video = vthread && buf == vthread->packetQueue();
    const qreal fps = qMax<qreal>(24.0, statistics.video.frame_rate);
    qint64 bv = 0.5*fps;
    if (!video) {
        // if has video, then audio buffer should not block the video buffer (bufferValue == 1, modified in AVDemuxMyThread)
        bv = statistics.audio.frame_rate > 0 && statistics.audio.frame_rate < 60 ?
                        statistics.audio.frame_rate : 3LL;
    }
    if (buffer_mode == BufferTime)
        bv = 600LL; //ms
    else if (buffer_mode == BufferBytes)
        bv = 1024LL;
    // no block for music with cover
    if (video) {
        if (demuxer.hasAttacedPicture() || (statistics.video.frames > 0 && statistics.video.frames < bv))
            bv = qMax<qint64>(1LL, statistics.video.frames);
    }
    buf->setBufferMode(buffer_mode);
    buf->setBufferValue(buffer_value < 0LL ? bv : buffer_value);
}

void AVReader::updateBufferValue()
{
    if (athread)
        updateBufferValue(athread->packetQueue());
    if (vthread)
        updateBufferValue(vthread->packetQueue());
}
//end avplayerprivate

//avplayer
AVClock* AVReader::masterClock()
{
    return clock;
}
#if 0
void AVReader::addVideoRenderer(VideoRenderer *renderer)
{return;
}

void AVReader::removeVideoRenderer(VideoRenderer *renderer)
{return;
}

void AVReader::clearVideoRenderers()
{return;
}

void AVReader::setRenderer(VideoRenderer *r)
{return;
}

VideoRenderer *AVReader::renderer()
{return NULL;
}
#endif
#if 0
QList<VideoRenderer*> AVReader::videoOutputs()
{
    if (!vos)
        return QList<VideoRenderer*>();
    QList<VideoRenderer*> tvos;
    tvos.reserve(vos->outputs().size());
    foreach (AVOutput *out, vos->outputs()) {
        tvos.append(static_cast<VideoRenderer*>(out));
    }
    return tvos;
}

AudioOutput* AVReader::audio()
{return NULL;
}
#endif
void AVReader::setSpeed(qreal speed)
{return;
}

qreal AVReader::fspeed() const
{
    return speed;
}

void AVReader::setInterruptTimeout(qint64 ms)
{
    if (ms < 0LL)
        ms = -1LL;
    if (interrupt_timeout == ms)
        return;
    interrupt_timeout = ms;
    Q_EMIT interruptTimeoutChanged();
    demuxer.setInterruptTimeout(ms);
}

qint64 AVReader::interruptTimeout() const
{
    return interrupt_timeout;
}

void AVReader::setInterruptOnTimeout(bool value)
{return;
}

bool AVReader::isInterruptOnTimeout() const
{
    return demuxer.isInterruptOnTimeout();
}

void AVReader::setFrameRate(qreal value)
{return;
}

qreal AVReader::forcedFrameRate() const
{
    return force_fps;
}

const Statistics& AVReader::fstatistics() const
{
    return statistics;
}
#if 0
bool AVReader::installFilter(AudioFilter *filter, int index)
{return false;
}

bool AVReader::installFilter(VideoFilter *filter, int index)
{return false;
}

bool AVReader::uninstallFilter(AudioFilter *filter)
{return false;
}

bool AVReader::uninstallFilter(VideoFilter *filter)
{return false;
}

QList<Filter*> AVReader::audioFilters() const
{
    return FilterManager::instance().audioFilters(NULL); //storm
}

QList<Filter*> AVReader::videoFilters() const
{
    return FilterManager::instance().videoFilters(NULL); //storm
}
#endif
void AVReader::setPriority(const QVector<VideoDecoderId> &ids)
{return ;
}

template<typename ID, typename T>
static QVector<ID> idsFromNames(const QStringList& names) {
    QVector<ID> decs;
    if (!names.isEmpty()) {
        decs.reserve(names.size());
        foreach (const QString& name, names) {
            if (name.isEmpty())
                continue;
            ID id = T::id(name.toLatin1().constData());
            if (id == 0)
                continue;
            decs.append(id);
        }
    }
    return decs;
}

void AVReader::setVideoDecoderPriority(const QStringList &names)
{return;
}

template<typename ID, typename T>
static QStringList idsToNames(QVector<ID> ids) {
    QStringList decs;
    if (!ids.isEmpty()) {
        decs.reserve(ids.size());
        foreach (ID id, ids) {
            decs.append(QString::fromLatin1(T::name(id)));
        }
    }
    return decs;
}

QStringList AVReader::videoDecoderPriority() const
{
    return idsToNames<VideoDecoderId, VideoDecoder>(vc_ids);
}

void AVReader::setOptionsForFormat(const QVariantHash &dict)
{
    demuxer.setOptions(dict);
}

QVariantHash AVReader::optionsForFormat() const
{
    return demuxer.options();
}

void AVReader::setOptionsForAudioCodec(const QVariantHash &dict)
{return;
}

QVariantHash AVReader::optionsForAudioCodec() const
{
    return ac_opt;
}

void AVReader::setOptionsForVideoCodec(const QVariantHash &dict)
{
    vc_opt = dict;
    const QVariant p(dict.contains(QStringLiteral("priority")));
    if (p.type() == QVariant::StringList) {
        setVideoDecoderPriority(p.toStringList());
        vc_opt.remove(QStringLiteral("priority"));
    }
}

QVariantHash AVReader::optionsForVideoCodec() const
{
    return vc_opt;
}

void AVReader::setMediaEndAction(MediaEndAction value)
{return;
}

MediaEndAction AVReader::mediaEndAction() const
{
    return end_action;
}

/*
 * loaded state is the state of current setted file.
 * For replaying, we can avoid load a seekable file again.
 * For playing a new file, load() is required.
 */
void AVReader::setFile(const QString &path)
{
    // file() is used somewhere else. ensure it is correct
    QString p(path);
    // QFile does not support "file:"
    if (p.startsWith(QLatin1String("file:")))
        p = Internal::Path::toLocal(p);
    reset_state = current_source.type() != QVariant::String || current_source.toString() != p;
    current_source = p;
    // TODO: reset_state = demuxer2.setMedia(path);
    if (reset_state) {
        audio_track = video_track = subtitle_track = 0;
        Q_EMIT sourceChanged();
        //Q_EMIT error(AVError(AVError::NoError));
    }
    // TODO: use absoluteFilePath?
    bloaded = false; //
}

QString AVReader::file() const
{
    if (current_source.type() == QVariant::String)
        return current_source.toString();
    return QString();
}
#if 0
void AVReader::setIODevice(QIODevice* device)
{return;
}

void AVReader::setInput(MediaIO *in)
{return;
}

MediaIO* AVReader::input() const
{return NULL;
}

VideoCapture* AVReader::videoCapture() const
{return NULL;
}
#endif
void AVReader::play(const QString& path)
{
    setFile(path);
    play();
}

bool AVReader::isPlaying() const
{
    return /*(read_thread &&read_thread->isRunning())
            || */(athread && athread->isRunning())
            || (vthread && vthread->isRunning());
}

void AVReader::togglePause()
{return;
}

void AVReader::pause(bool p)
{return;
}

bool AVReader::isPaused() const
{
    return /*(read_thread && read_thread->isPaused())
            ||*/ (athread && athread->isPaused())
            || (vthread && vthread->isPaused());
}

MediaStatus AVReader::mediaStatus() const
{
    return status;
}

void AVReader::setAutoLoad(bool value)
{
    if (auto_load == value)
        return;
    auto_load = value;
    Q_EMIT autoLoadChanged();
}

bool AVReader::isAutoLoad() const
{
    return auto_load;
}

void AVReader::setAsyncLoad(bool value)
{
    if (async_load == value)
        return;
    async_load = value;
    Q_EMIT asyncLoadChanged();
}

bool AVReader::isAsyncLoad() const
{
    return async_load;
}

bool AVReader::isLoaded() const
{
    return bloaded;
}

void AVReader::loadInternal()
{
    QMutexLocker lock(&load_mutex);
    Q_UNUSED(lock);
    // release codec ctx
    //close decoders here to make sure open and close in the same thread if not async load
    if (isLoaded()) {
        if (adec)
            adec->setCodecContext(0);
        if (vdec)
            vdec->setCodecContext(0);
    }
    qDebug() << "Loading " << current_source << " ...";
    if (current_source.type() == QVariant::String) {
        demuxer.setMedia(current_source.toString());
    } else {
#if 0
        if (current_source.canConvert<QIODevice*>()) {
            demuxer.setMedia(current_source.value<QIODevice*>());
        } else { // MediaIO
            demuxer.setMedia(current_source.value<QtAV::MediaIO*>());
        }
#endif
    }
    bloaded = demuxer.load();
    status = demuxer.mediaStatus();
    if (!bloaded) {
        statistics.reset();
        qWarning("Load failed!");
        audio_tracks = getTracksInfo(&demuxer, AVDemuxer::AudioStream);
        Q_EMIT internalAudioTracksChanged(audio_tracks);
        video_tracks = getTracksInfo(&demuxer, AVDemuxer::VideoStream);
        Q_EMIT internalVideoTracksChanged(video_tracks);
        subtitle_tracks = getTracksInfo(&demuxer, AVDemuxer::SubtitleStream);
        Q_EMIT internalSubtitleTracksChanged(subtitle_tracks);
        return;
    }
    subtitle_tracks = getTracksInfo(&demuxer, AVDemuxer::SubtitleStream);
    Q_EMIT internalSubtitleTracksChanged(subtitle_tracks);
    applySubtitleStream(subtitle_track, this);
    audio_tracks = getTracksInfo(&demuxer, AVDemuxer::AudioStream);
    Q_EMIT internalAudioTracksChanged(audio_tracks);
    video_tracks = getTracksInfo(&demuxer, AVDemuxer::VideoStream);
    Q_EMIT internalVideoTracksChanged(video_tracks);
    Q_EMIT durationChanged(duration());
    Q_EMIT chaptersChanged(chapters());
    // setup parameters from loaded media
    media_start_pts = demuxer.startTime();
    // TODO: what about other proctols? some vob duration() == 0
    if (duration() > 0)
        media_end = mediaStartPosition() + duration();
    else
        media_end = kInvalidPosition;
    start_position_norm = normalizedPosition(start_position);
    stop_position_norm = normalizedPosition(stop_position);
    int interval = qAbs(notify_interval);
    initStatistics();
    if (interval != qAbs(notify_interval))
        Q_EMIT notifyIntervalChanged();
}

void AVReader::unload()
{
    if (!isLoaded())
        return;
    QMutexLocker lock(&load_mutex);
    Q_UNUSED(lock);
    bloaded = false;
    demuxer.setInterruptStatus(-1);

    if (adec) { // FIXME: crash if audio external=>internal then replay
        adec->setCodecContext(0);
        delete adec;
        adec = 0;
    }
    if (vdec) {
        vdec->setCodecContext(0);
        delete vdec;
        vdec = 0;
    }
    demuxer.unload();
    Q_EMIT chaptersChanged(0);
    Q_EMIT durationChanged(0LL); // for ui, slider is invalid. use stopped instead, and remove this signal here?
    // ??
    audio_tracks = getTracksInfo(&demuxer, AVDemuxer::AudioStream);
    Q_EMIT internalAudioTracksChanged(audio_tracks);
    video_tracks = getTracksInfo(&demuxer, AVDemuxer::VideoStream);
    Q_EMIT internalVideoTracksChanged(video_tracks);
}

void AVReader::setRelativeTimeMode(bool value)
{return;
}

bool AVReader::relativeTimeMode() const
{
    return relative_time_mode;
}

qint64 AVReader::absoluteMediaStartPosition() const
{
    return media_start_pts;
}

qreal AVReader::durationF() const
{
    return double(demuxer.durationUs())/double(AV_TIME_BASE); //AVFrameContext.duration time base: AV_TIME_BASE
}

qint64 AVReader::duration() const
{
    return demuxer.duration();
}

qint64 AVReader::mediaStartPosition() const
{
    if (relativeTimeMode())
        return 0;
    return demuxer.startTime();
}

qint64 AVReader::mediaStopPosition() const
{
    if (media_end == kInvalidPosition && duration() > 0) {
        // called in stop()
        return mediaStartPosition() + duration();
    }
    return media_end;
}

qreal AVReader::mediaStartPositionF() const
{
    if (relativeTimeMode())
        return 0;
    return double(demuxer.startTimeUs())/double(AV_TIME_BASE);
}


qint64 AVReader::normalizedPosition(qint64 pos)
{
    if (!isLoaded())
        return pos;
    qint64 p0 = mediaStartPosition();
    qint64 p1 = mediaStopPosition();
    if (relativeTimeMode()) {
        p0 = 0;
        if (p1 != kInvalidPosition)
            p1 -= p0; //duration
    }
    if (pos < 0) {
        if (p1 == kInvalidPosition)
            pos = kInvalidPosition;
        else
            pos += p1;
    }
    return qMax(qMin(pos, p1), p0);
}

qint64 AVReader::startPosition() const
{
    return start_position;
}

void AVReader::setStartPosition(qint64 pos)
{return;
}

qint64 AVReader::stopPosition() const
{
    return stop_position;
}

void AVReader::setStopPosition(qint64 pos)
{return;
}

void AVReader::setTimeRange(qint64 start, qint64 stop)
{return;
}

bool AVReader::isSeekable() const
{
    return demuxer.isSeekable();
}

qint64 AVReader::position() const
{
    // TODO: videoTime()?
    const qint64 pts = clock->value()*1000.0;
    if (relativeTimeMode())
        return pts - absoluteMediaStartPosition();
    return pts;
}

void AVReader::setPosition(qint64 position)
{
    // FIXME: strange things happen if seek out of eof
    if (position > stop_position_norm)
        return;
    if (!isPlaying())
        return;
    qint64 pos_pts = position;
    if (pos_pts < 0)
        pos_pts = 0;
    // position passed in is relative to the start pts in relative time mode
    if (relativeTimeMode())
        pos_pts += absoluteMediaStartPosition();
    seeking = true;
    //read_thread->seek(position,pos_pts, seekType());
    vthread->seek(position,pos_pts, seekType());

    Q_EMIT positionChanged(position); //emit relative position
}

int AVReader::repeat() const
{
    return repeat_max;
}

int AVReader::currentRepeat() const
{
    return repeat_current;
}

// TODO: reset current_repeat?
void AVReader::setRepeat(int max)
{
    repeat_max = max;
    if (repeat_max < 0)
        repeat_max = std::numeric_limits<int>::max();
    Q_EMIT repeatChanged(repeat_max);
}

bool AVReader::setExternalAudio(const QString &file)
{return false;
}

QString AVReader::externalAudio() const
{
    return external_audio;
}

const QVariantList& AVReader::externalAudioTracks() const
{
    return external_audio_tracks;
}

const QVariantList &AVReader::internalAudioTracks() const
{
    return audio_tracks;
}

const QVariantList &AVReader::internalVideoTracks() const
{
    return video_tracks;
}

bool AVReader::setAudioStream(const QString &file, int n)
{return false;
}

bool AVReader::setAudioStream(int n)
{return false;
}

bool AVReader::setVideoStream(int n)
{
    if (n < 0)
        return false;
    if (video_track == n)
        return true;
    if (isLoaded()) {
        if (n >= demuxer.videoStreams().size())
            return false;
    }
    video_track = n;
    demuxer.setStreamIndex(AVDemuxer::VideoStream, n);
//    if (!isPlaying())
//        return true;
//    // pause demuxer, clear queues, set demuxer stream, set decoder, set ao, resume
//    bool p = isPaused();
//    //int bv = bufferValue();
//    setBufferMode(BufferTime);
//    pause(true);
//    if (!setupVideoThread(this)) {
//        stop();
//        return false;
//    }
//    if (!p) pause(false);
//    //QTimer::singleShot(10000, this, SLOT(setBufferValue(bv)));
    return true;
}

const QVariantList& AVReader::internalSubtitleTracks() const
{
    return subtitle_tracks;
}

bool AVReader::setSubtitleStream(int n)
{return false;
}

int AVReader::currentAudioStream() const
{
    return demuxer.audioStreams().indexOf(demuxer.audioStream());
}

int AVReader::currentVideoStream() const
{
    return demuxer.videoStreams().indexOf(demuxer.videoStream());
}

int AVReader::currentSubtitleStream() const
{
    return demuxer.subtitleStreams().indexOf(demuxer.subtitleStream());
}

int AVReader::audioStreamCount() const
{
    return demuxer.audioStreams().size();
}

int AVReader::videoStreamCount() const
{
    return demuxer.videoStreams().size();
}

int AVReader::subtitleStreamCount() const
{
    return demuxer.subtitleStreams().size();
}

AVReader::SState AVReader::state() const
{
    return vstate;
}

void AVReader::setState(SState value)
{
    if (vstate == value)
        return;
    if (value == StoppedState) {
        stop();
        return;
    }
    if (value == PausedState) {
        pause(true);
        return;
    }
    // value == PlayingState
    if (vstate == StoppedState) {
        play();
        return;
    }
    if (vstate == PausedState) {
        pause(false);
        return;
    }

}

bool AVReader::load()
{
    if (!current_source.isValid()) {
        qDebug("Invalid media source. No file or IODevice was set.");
        return false;
    }
    if (!checkSourceChange() && (mediaStatus() == QtAV::LoadingMedia || mediaStatus() == LoadedMedia))
        return true;
    if (isLoaded()) {
        // release codec ctx. if not loaded, they are released by avformat. TODO: always let avformat release them?
        if (adec)
            adec->setCodecContext(0);
        if (vdec)
            vdec->setCodecContext(0);
    }
    bloaded = false;
    status = LoadingMedia;
    if (!isAsyncLoad()) {
        loadInternal();
        return bloaded;
    }

    class LoadWorker : public QRunnable {
    public:
        LoadWorker(AVReader *player) : m_player(player) {}
        virtual void run() {
            if (!m_player)
                return;
            m_player->loadInternal();
        }
    private:
        AVReader* m_player;
    };
    // TODO: thread pool has a max thread limit
    loaderThreadPool()->start(new LoadWorker(this));
    return true;
}

void AVReader::play()
{
    //FIXME: bad delay after play from here
    if (isPlaying()) {
        qDebug("play() when playing");
        if (!checkSourceChange())
            return;
        stop();
    }
    if (!load()) {
        qWarning("load error");
        return;
    }
    if (isLoaded()) { // !asyncLoad() is here because load() returned true
        playInternal();
        return;
    }
    connect(this, SIGNAL(loaded()), this, SLOT(playInternal()));
}

void AVReader::playInternal()
{
    {
    QMutexLocker lock(&load_mutex);
    Q_UNUSED(lock);
    if (!demuxer.isLoaded())
        return;
    start_position_norm = normalizedPosition(start_position);
    stop_position_norm = normalizedPosition(stop_position);
    // FIXME: if call play() frequently playInternal may not be called if disconnect here
    disconnect(this, SIGNAL(loaded()), this, SLOT(playInternal()));
    /*if (!setupAudioThread(this)) {
        read_threasetAudioThread(0); //set 0 before delete. ptr is used in demux thread when set 0
        if (athread) {
            qDebug("release audio thread.");
            delete athread;
            athread = 0;//shared ptr?
        }
    }*/ // remove audio storm
    if (!setupVideoThread(this)) {
        //read_thread->setVideoThread(0); //set 0 before delete. ptr is used in demux thread when set 0
        vthread->setVideoThread(0); //set 0 before delete. ptr is used in demux thread when set 0
        if (vthread) {
            qDebug("release video thread.");
            delete vthread;
            vthread = 0;//shared ptr?
        }
    }
    if (!athread && !vthread) {
        bloaded = false;
        qWarning("load failed");
        return;
    }
    // setup clock before avthread.start() becuase avthreads use clock. after avthreads setup because of ao check
    masterClock()->reset();
    // TODO: add isVideo() or hasVideo()?
    if (masterClock()->isClockAuto()) {
        qDebug("auto select clock: audio > external");
        if (!demuxer.audioCodecContext() /*|| !ao || !ao->isOpen()*/ || !athread) {
            masterClock()->setClockType(AVClock::ExternalClock);
            qDebug("No audio found or audio not supported. Using ExternalClock.");
        } else {
            qDebug("Using AudioClock");
            masterClock()->setClockType(AVClock::AudioClock);
        }
    }
    masterClock()->setInitialValue((double)absoluteMediaStartPosition()/1000.0);
    // from previous play()
    if (demuxer.audioCodecContext() && athread) {
        qDebug("Starting audio thread...");
        athread->start();
    }
    if (demuxer.videoCodecContext() && vthread) {
        qDebug("Starting video thread...");
        vthread->start(); //storm
    }
#if 0
    if (demuxer.audioCodecContext() && athread)
        athread->waitForStarted();
#endif
#if 0
    // tmp why? //storm
    if (demuxer.videoCodecContext() && vthread)
        vthread->waitForStarted();
#endif
    //read_thread->setMediaEndAction(mediaEndAction());
    vthread->setMediaEndAction(mediaEndAction());
    //read_thread->start();

    /// demux thread not started, seek tasks will be cleared
    //read_thread->waitForStarted();
    if (timer_id < 0) {
        //timer_id = startNotifyTimer(); //may fail if not in this thread
        QMetaObject::invokeMethod(this, "startNotifyTimer", Qt::AutoConnection);
    }
    vstate = PlayingState;
    if (repeat_current < 0)
        repeat_current = 0;
    } //end lock scoped here to avoid dead lock if connect started() to a slot that call unload()/play()
    if (start_position_norm > 0) {
        if (relativeTimeMode())
            setPosition(qint64((start_position_norm + absoluteMediaStartPosition())));
        else
            setPosition((qint64)(start_position_norm));
    }
    Q_EMIT stateChanged(PlayingState);
    Q_EMIT started(); //we called stop(), so must emit started()
}

void AVReader::stopFromDemuxerThread()
{
    qDebug("demuxer thread emit finished. repeat: %d/%d", currentRepeat(), repeat());
    seeking = false;
    if (currentRepeat() < 0 || (currentRepeat() >= repeat() && repeat() >= 0)) {
        qreal stop_pts = masterClock()->videoTime();
        if (stop_pts <= 0)
            stop_pts = masterClock()->value();
        masterClock()->reset();
        QMetaObject::invokeMethod(this, "stopNotifyTimer");
        // vars not set by user can be reset
        repeat_current = -1;
        start_position_norm = 0;
        stop_position_norm = kInvalidPosition; // already stopped. so not 0 but invalid. 0 can stop the playback in timerEvent
        media_end = kInvalidPosition;
        qDebug("avplayer emit stopped()");
        vstate = StoppedState;
        QMetaObject::invokeMethod(this, "stateChanged", Q_ARG(QtAV::AVReader::SState, vstate));
        QMetaObject::invokeMethod(this, "stopped");
        QMetaObject::invokeMethod(this, "stoppedAt", Q_ARG(qint64, qint64(stop_pts*1000.0)));
        //Q_EMIT stateChanged(vstate);
        //Q_EMIT stopped();
        //Q_EMIT stoppedAt(stop_pts*1000.0);

        /*
         * currently preload is not supported. so always unload. Then some properties will be reset, e.g. duration()
         */
        unload(); //TODO: invoke?
    } else {
        repeat_current++;
        QMetaObject::invokeMethod(this, "play"); //ensure play() is called from player thread
    }
}

void AVReader::aboutToQuitApp()
{
    reset_state = true;
    stop();
    while (isPlaying()) {
        qApp->processEvents();
        qDebug("about to quit.....");
        pause(false); // may be paused. then aboutToQuitApp will not finish
        stop();
    }
    demuxer.setInterruptStatus(-1);
    loaderThreadPool()->waitForDone();
}

void AVReader::setNotifyInterval(int msec)
{
    if (notify_interval == msec)
        return;
    if (notify_interval < 0 && msec <= 0)
        return;
    const int old = qAbs(notify_interval);
    notify_interval = msec;
    updateNotifyInterval();
    Q_EMIT notifyIntervalChanged();
    if (timer_id < 0)
        return;
    if (old != qAbs(notify_interval)) {
        stopNotifyTimer();
        startNotifyTimer();
    }
}

int AVReader::notifyInterval() const
{
    return qAbs(notify_interval);
}

void AVReader::startNotifyTimer()
{
    timer_id = startTimer(qAbs(notify_interval));
}

void AVReader::stopNotifyTimer()
{
    if (timer_id < 0)
        return;
    killTimer(timer_id);
    timer_id = -1;
}

void AVReader::onStarted()
{
    if (speed != 1.0) {
#if 0
        //TODO: check clock type?
        if (ao && ao->isAvailable()) {
            ao->setSpeed(speed);
        }
#endif
        masterClock()->setSpeed(speed);
    } else {
        applyFrameRate();
    }
}

void AVReader::updateMediaStatus(QtAV::MediaStatus tstatus)
{
    if (status == tstatus)
        return;
    status = tstatus;
    Q_EMIT mediaStatusChanged(status);
}

void AVReader::onSeekFinished(qint64 value)
{
    seeking = false;
    Q_EMIT seekFinished(value);
    //clock->updateValue(value/1000.0);
    if (relativeTimeMode())
        Q_EMIT positionChanged(value - absoluteMediaStartPosition());
    else
        Q_EMIT positionChanged(value);
}
#if 0
void AVReader::tryClearVideoRenderers()
{return;
}
#endif
void AVReader::seekChapter(int incr)
{
    if (!chapters())
        return;

    qint64 pos = masterClock()->value() * AV_TIME_BASE;
    int i = 0;

    AVFormatContext *ic = demuxer.formatContext();

    AVRational av_time_base_q;
    av_time_base_q.num = 1;
    av_time_base_q.den = AV_TIME_BASE;

    /* find the current chapter */
    for (i = 0; i < chapters(); ++i) {
        AVChapter *ch = ic->chapters[i];
        if (av_compare_ts(pos, av_time_base_q, ch->start, ch->time_base) < 0) {
            --i;
            break;
        }
    }

    i += incr;
    //i = FFMAX(i, 0);
    if (i <= 0)
        i = 0;
    if (i >= chapters())
        return;

    //av_log(NULL, AV_LOG_VERBOSE, "Seeking to chapter %d.\n", i);
    qDebug() << QString::fromLatin1("Seeking to chapter : ") << QString::number(i);
    setPosition(av_rescale_q(ic->chapters[i]->start, ic->chapters[i]->time_base,
                             av_time_base_q) / 1000);
}

void AVReader::stop()
{
    // check timer_id, <0 return?
    if (reset_state) {
        /*
         * must kill in main thread! If called from user, may be not in main thread.
         * then timer goes on, and player find that stop_position reached(stop_position is already
         * 0 after user call stop),
         * stop() is called again by player and reset state. but this call is later than demuxer stop.
         * so if user call play() immediatly, may be stopped by AVPlayer
         */
        // TODO: invokeMethod "stopNotifyTimer"
        if (timer_id >= 0) {
            qDebug("timer: %d, current thread: %p, player thread: %p", timer_id, QThread::currentThread(), thread());
            if (QThread::currentThread() == thread()) { //called by user in the same thread as player
                stopNotifyTimer();
            } else {
                //TODO: post event.
            }
        }
        // vars not set by user can be reset
        start_position_norm = 0;
        stop_position_norm = 0; // 0 can stop play in timerEvent
        media_end = kInvalidPosition;
    } else { //called by player
        stopNotifyTimer();
    }
    seeking = false;
    reset_state = true;
    repeat_current = -1;
    if (!isPlaying()) {
        qDebug("Not playing~");
        if (mediaStatus() == LoadingMedia || mediaStatus() == LoadedMedia) {
            qDebug("loading media: %d", mediaStatus() == LoadingMedia);
            demuxer.setInterruptStatus(-1);
        }
        return;
    }
#if 0
    while (read_thread->isRunning()) {
        qDebug("stopping demuxer thread...");
        read_thread->stop();
        read_thread->wait(500);
        // interrupt to quit av_read_frame quickly.
        demuxer.setInterruptStatus(-1);
    }
#endif
    qDebug("all audio/video threads stopped... state: %d", vstate);
}

void AVReader::timerEvent(QTimerEvent *te)
{
    if (te->timerId() == timer_id) {
        // killTimer() should be in the same thread as object. kill here?
        if (isPaused()) {
            //return; //ensure positionChanged emitted for stepForward()
        }
        // active only when playing
        const qint64 t = position();
        if (stop_position_norm == kInvalidPosition) { // or check stop_position_norm < 0
            // not seekable. network stream
            Q_EMIT positionChanged(t);
            return;
        }
        if (t < start_position_norm) {
            //qDebug("position %lld < startPosition %lld", t, start_position_norm);
            // or set clock initial value to get correct t
            if (start_position_norm != mediaStartPosition()) {
                setPosition(start_position_norm);
                return;
            }
        }
        if (t <= stop_position_norm) {
            if (!seeking) {
                Q_EMIT positionChanged(t);
            }
            return;
        }
        // atEnd() supports dynamic changed duration. but we can not break A-B repeat mode, so check stoppos and mediastoppos
        if ((!demuxer.atEnd() /*|| read_thread->isRunning()*/) && stopPosition() >= mediaStopPosition()) {
            if (!seeking) {
                Q_EMIT positionChanged(t);
            }
            return;
        }
        // TODO: remove. kill timer in an event;
        if (stop_position_norm == 0) { //stop() by user in other thread, state is already reset
            reset_state = false;
            qDebug("stopPosition() == 0, stop");
            stop();
        }
        // t < start_position is ok. repeat_max<0 means repeat forever
        if (currentRepeat() >= repeat() && repeat() >= 0) {
            reset_state = true; // true is default, can remove here
            qDebug("stopPosition() %lld/%lld reached and no repeat: %d", t, stopPosition(), repeat());
            stop();
            return;
        }
        // FIXME: now stop instead of seek if reach media's end. otherwise will not get eof again
        if (stop_position_norm == mediaStopPosition() || !isSeekable()) {
            // if not seekable, how it can start to play at specified position?
            qDebug("normalized stopPosition() == mediaStopPosition() or !seekable. repeat_current=%d", repeat_current);
            reset_state = false;
            stop(); // repeat after all threads stopped
        } else {
            repeat_current++;
            qDebug("noramlized stopPosition() != mediaStopPosition() and seekable. repeat_current=%d", repeat_current);
            setPosition(start_position_norm);
        }
    }
}

//FIXME: If not playing, it will just play but not play one frame.
void AVReader::stepForward()
{
    // pause clock
    pause(true); // must pause AVDemuxMyThread (set user_paused true)
    ////read_thread->stepForward();
    vthread->stepForward();
}

void AVReader::stepBackward()
{
    clock->pause(true);
    vstate = PausedState;
    Q_EMIT stateChanged(vstate);
    Q_EMIT paused(true);
    ////read_thread->stepBackward();
    vthread->stepBackward();
}

void AVReader::seek(qreal r)
{
    seek(qint64(r*double(duration())));
}

void AVReader::seek(qint64 pos)
{
    setPosition(pos);
}

void AVReader::seekForward()
{
    seek(position() + kSeekMS);
}

void AVReader::seekBackward()
{
    seek(position() - kSeekMS);
}

void AVReader::seekNextChapter()
{
    if (chapters() <= 1)
        return;
    seekChapter(1);
}

void AVReader::seekPreviousChapter()
{
    if (chapters() <= 1)
        return;
    seekChapter(-1);
}

void AVReader::setSeekType(SeekType type)
{
    seek_type = type;
}

SeekType AVReader::seekType() const
{
    return seek_type;
}

qreal AVReader::bufferProgress() const
{
    //const PacketBuffer* buf = read_thread->buffer();
    const PacketBuffer* buf = vthread->buffer();
    return buf ? buf->bufferProgress() : 0;
}

qreal AVReader::bufferSpeed() const
{
    //const PacketBuffer* buf = read_thread->buffer();
    const PacketBuffer* buf = vthread->buffer();
    return buf ? buf->bufferSpeedInBytes() : 0;
}

qint64 AVReader::buffered() const
{
    //const PacketBuffer* buf = read_thread->buffer();
    const PacketBuffer* buf = vthread->buffer();
    return buf ? buf->buffered() : 0LL;
}

void AVReader::setBufferMode(BufferMode mode)
{
    buffer_mode = mode;
}

BufferMode AVReader::bufferMode() const
{
    return buffer_mode;
}

void AVReader::setBufferValue(qint64 value)
{
    if (buffer_value == value)
        return;
    buffer_value = value;
    updateBufferValue();
}

int AVReader::bufferValue() const
{
    return buffer_value;
}

void AVReader::updateClock(qint64 msecs)
{
    clock->updateExternalClock(msecs);
}

int AVReader::brightness() const
{
    return fbrightness;
}

void AVReader::setBrightness(int val)
{
    if (fbrightness == val)
        return;
    fbrightness = val;
    Q_EMIT brightnessChanged(fbrightness);
    if (vthread) {
        vthread->setBrightness(val);
    }
}

int AVReader::contrast() const
{
    return fcontrast;
}

void AVReader::setContrast(int val)
{
    if (fcontrast == val)
        return;
    fcontrast = val;
    Q_EMIT contrastChanged(fcontrast);
    if (vthread) {
        vthread->setContrast(val);
    }
}

int AVReader::hue() const
{
    return 0;
}

void AVReader::setHue(int val)
{
    Q_UNUSED(val);
}

int AVReader::saturation() const
{
    return fsaturation;
}

unsigned int AVReader::chapters() const
{
    return 0;//demuxer.formatContext()->nb_chapters; //storm
}

void AVReader::setSaturation(int val)
{
    if (fsaturation == val)
        return;
    fsaturation = val;
    Q_EMIT saturationChanged(fsaturation);
    if (vthread) {
        vthread->setSaturation(val);
    }
}
//end avplayer

} //namespace QtAV
