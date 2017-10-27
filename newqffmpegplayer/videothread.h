#ifndef VideoThread_H
#define VideoThread_H

#include <QThread>
#include <QImage>

extern "C"{
#include "libavutil/avstring.h"
//#include "libavutil/colorspace.h" //add ffmpeg
#include "colorspace.h" //add ffmpeg
//#include "libavutil/display.h" //add ffmpeg
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libavresample/avresample.h" //add ffmpeg
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"

#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/display.h"

#include "cmdutils.h"

    #include <SDL.h>
    #include <SDL_audio.h>
    #include <SDL_types.h>
    #include <SDL_name.h>
    #include <SDL_main.h>
    #include <SDL_config.h>
}
#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
#define MIN_AUDIOQ_SIZE (20 * 16 * 1024)
#define MIN_FRAMES 5

/* SDL audio buffer size, in samples. Should be small to have precise
   A/V sync as SDL does not have hardware buffer fullness info. */
#define SDL_AUDIO_BUFFER_SIZE 1024

/* no AV sync correction is done if below the AV sync threshold */
#define AV_SYNC_THRESHOLD 0.01
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

#define FRAME_SKIP_FACTOR 0.05

/* maximum audio speed change to get correct sync */
#define SAMPLE_CORRECTION_PERCENT_MAX 10

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
#define AUDIO_DIFF_AVG_NB   20

/* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
#define SAMPLE_ARRAY_SIZE (2 * 65536)

////static int64_t sws_flags = SWS_BICUBIC;

typedef struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    int abort_request;
    SDL_mutex *mutex;
    SDL_cond *cond;
} PacketQueue;

#define VIDEO_PICTURE_QUEUE_SIZE 2
#define SUBPICTURE_QUEUE_SIZE 4

typedef struct VideoPicture {
    double pts;             // presentation timestamp for this picture
    double target_clock;    // av_gettime_relative() time at which this should be displayed ideally
    int64_t pos;            // byte position in file
    SDL_Overlay *bmp;
    //SDL_Texture *bmp;
    int width, height; /* source height & width */
    int allocated;
    int reallocate;
    enum AVPixelFormat pix_fmt;

    AVRational sar;
} VideoPicture;

typedef struct SubPicture {
    double pts; /* presentation time stamp for this picture */
    AVSubtitle sub;
} SubPicture;

enum {
    AV_SYNC_AUDIO_MASTER, /* default choice */
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};

typedef struct PlayerState {
    SDL_Thread *parse_tid;
    SDL_Thread *video_tid;
    SDL_Thread *refresh_tid;
    AVInputFormat *iformat;
    int no_background;
    int abort_request;
    int paused;
    int last_paused;
    int seek_req;
    int seek_flags;
    int64_t seek_pos;
    int64_t seek_rel;
    int read_pause_return;
    AVFormatContext *ic;

    int audio_stream;

    int av_sync_type;
    double external_clock; /* external clock base */
    int64_t external_clock_time;

    double audio_clock;
    double audio_diff_cum; /* used for AV difference average computation */
    double audio_diff_avg_coef;
    double audio_diff_threshold;
    int audio_diff_avg_count;
    AVStream *audio_st;
    AVCodecContext *audio_dec;
    PacketQueue audioq;
    int audio_hw_buf_size;
    uint8_t silence_buf[SDL_AUDIO_BUFFER_SIZE];
    uint8_t *audio_buf;
    uint8_t *audio_buf1;
    unsigned int audio_buf_size; /* in bytes */
    int audio_buf_index; /* in bytes */
    AVPacket audio_pkt_temp;
    AVPacket audio_pkt;
    enum AVSampleFormat sdl_sample_fmt;
    uint64_t sdl_channel_layout;
    int sdl_channels;
    int sdl_sample_rate;
    enum AVSampleFormat resample_sample_fmt;
    uint64_t resample_channel_layout;
    int resample_sample_rate;
    AVAudioResampleContext *avr;
    AVFrame *frame;

    int show_audio; /* if true, display audio samples */
    int16_t sample_array[SAMPLE_ARRAY_SIZE];
    int sample_array_index;
    int last_i_start;
    RDFTContext *rdft;
    int rdft_bits;
    FFTSample *rdft_data;
    int xpos;

    SDL_Thread *subtitle_tid;
    int subtitle_stream;
    int subtitle_stream_changed;
    AVStream *subtitle_st;
    AVCodecContext *subtitle_dec;
    PacketQueue subtitleq;
    SubPicture subpq[SUBPICTURE_QUEUE_SIZE];
    int subpq_size, subpq_rindex, subpq_windex;
    SDL_mutex *subpq_mutex;
    SDL_cond *subpq_cond;

    double frame_timer;
    double frame_last_pts;
    double frame_last_delay;
    double video_clock;             // pts of last decoded frame / predicted pts of next decoded frame
    int video_stream;
    AVStream *video_st;
    AVCodecContext *video_dec;
    PacketQueue videoq;
    double video_current_pts;       // current displayed pts (different from video_clock if frame fifos are used)
    double video_current_pts_drift; // video_current_pts - time (av_gettime_relative) at which we updated video_current_pts - used to have running video pts
    int64_t video_current_pos;      // current displayed file pos
    VideoPicture pictq[VIDEO_PICTURE_QUEUE_SIZE];
    int pictq_size, pictq_rindex, pictq_windex;
    SDL_mutex *pictq_mutex;
    SDL_cond *pictq_cond;

    //    QETimer *video_timer;
    char filename[1024];
    int width, height, xleft, ytop;

    PtsCorrectionContext pts_ctx;

    AVFilterContext *in_video_filter;   // the first filter in the video chain
    AVFilterContext *out_video_filter;  // the last filter in the video chain
    SDL_mutex *video_filter_mutex;

    float skip_frames;
    float skip_frames_index;
    int refresh;

    SpecifierOpt *codec_names;
    int        nb_codec_names;


    //播放控制标志
    bool isPause;  //暂停标志
    bool quit;     //停止
    bool readFinished; //文件读取完毕
    bool readThreadFinished;
    bool videoThreadFinished;

    //SDL_AudioDeviceID audioID;
    int64_t audio_callback_time;
    int loop = 1;
    int infinite_buffer = 0;
    char *vfilters ;
    int autorotate = 1;

    int noautoexit=0;
    int decoder_reorder_pts = -1;
    int64_t start_time = AV_NOPTS_VALUE;
    int64_t duration = AV_NOPTS_VALUE;
    int step = 0;
    PlayerState *global_video_state;
} PlayerState;

class VideoThread : public QThread{
    Q_OBJECT

public:

    enum PlayState{
        Playing,
        Pause,
        Stop
    };
    bool circling;

    explicit VideoThread();
    ~VideoThread();

    bool setFileName(QString path);

    bool play();
    bool pause();
    bool stop(bool isWait = false); //参数表示是否等待所有的线程执行完毕再返回

    void seek(int64_t pos); //单位是微秒

    int64_t getTotalTime(); //单位微秒
    double getCurrentTime(); //单位秒

    void disPlayVideo(QImage img);   //激发信号的函数
    bool isPlaying()
    {
        return mPlayerState == Playing;
    }

signals:
    void sig_GetOneFrame(QImage);   //每获取到一帧图像 就发送此信号

    void sig_StateChanged(VideoThread::PlayState state);  //状态改变信号

    void sig_TotalTimeChanged(qint64 uSec);  //获取到视频时长的时候激发此信号

protected:
    void run();                    //QThread的run函数

private:
#if 0
    void packet_queue_init(PacketQueue *q);
    void packet_queue_flush(PacketQueue *q);
void packet_queue_end(PacketQueue *q);
 int packet_queue_put(PacketQueue *q, AVPacket *pkt);
 void packet_queue_abort(PacketQueue *q);
 int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block);
#endif
 inline void fill_rectangle(SDL_Surface *screen,
                                  int x, int y, int w, int h, int color);
void blend_subrect(uint8_t *dst[4], uint16_t dst_linesize[4],
                          const AVSubtitleRect *rect, int imgw, int imgh);
void free_subpicture(SubPicture *sp);
 void video_image_display(PlayerState *is);
 //int audio_write_get_buf_size(PlayerState *is);
 inline int compute_mod(int a, int b);
 void video_audio_display(PlayerState *s);
 int video_open(PlayerState *is);
 void video_display(PlayerState *is);
 //int refresh_thread(void *opaque);
#if 0
 double get_audio_clock(PlayerState *is);
 double get_video_clock(PlayerState *is);
 double get_external_clock(PlayerState *is);
 double get_master_clock(PlayerState *is);
#endif
 //void stream_seek(PlayerState *is, int64_t pos, int64_t rel, int seek_by_bytes) ;
 //void stream_pause(PlayerState *is) ;//kuli update storm 
 //double compute_target_time(double frame_current_pts, PlayerState *is);
 void video_refresh_timer(void *opaque);
 void player_close(PlayerState *is) ;// kuli update storm : stream_close
 void do_exit(void) ;//kuli update storm
 void alloc_picture(void *opaque);
 //int queue_picture(PlayerState *is, AVFrame *src_frame, double pts, int64_t pos) ;//kuli update storm: add filter for frame
 //int output_picture2(PlayerState *is, AVFrame *src_frame, double pts1, int64_t pos);
 //int get_video_frame(PlayerState *is, AVFrame *frame, int64_t *pts, AVPacket *pkt);
 //int configure_video_filters(AVFilterGraph *graph, PlayerState *is, const char *vfilters);
 //int video_thread(void *arg);
 //int subtitle_thread(void *arg);
 //void update_sample_display(PlayerState *is, short *samples, int samples_size);
 //int synchronize_audio(PlayerState *is, short *samples,
 //                            int samples_size1, double pts);
 //int audio_decode_frame(PlayerState *is, double *pts_ptr);
 //void sdl_audio_callback(void *opaque, Uint8 *stream, int len);
 AVCodec *find_codec_or_die(const char *name, enum AVMediaType type);
 AVCodec *choose_decoder(PlayerState *is, AVFormatContext *ic, AVStream *st);
 int stream_component_open(PlayerState *is, int stream_index);
 //void stream_component_close(PlayerState *is, int stream_index);
 //int decode_interrupt_cb(void *ctx);
 //void stream_close(PlayerState *is);
 int stream_setup(PlayerState *is);
 //int decode_thread(void *arg) ;// kuli update storm
 int stream_open(PlayerState *is,
                       const char *filename, AVInputFormat *iformat);
 void stream_cycle_channel(PlayerState *is, int codec_type);
 void toggle_full_screen(void);
 void toggle_pause(void);
 void step_to_next_frame(void);
 void toggle_audio_display(void);
 void seek_chapter(PlayerState *is, int incr);
 void event_loop(void);
#if 0
 int opt_frame_size(void *optctx, const char *opt, const char *arg);
 int opt_width(void *optctx, const char *opt, const char *arg);
 int opt_height(void *optctx, const char *opt, const char *arg);
 int opt_format(void *optctx, const char *opt, const char *arg);
 int opt_frame_pix_fmt(void *optctx, const char *opt, const char *arg);
 int opt_sync(void *optctx, const char *opt, const char *arg);
 int opt_seek(void *optctx, const char *opt, const char *arg);
 int opt_duration(void *optctx, const char *opt, const char *arg);
#endif
 void opt_input_file(void *optctx, const char *filename);
private:
    QString mFileName;

    //VideoState mVideoState;
    PlayerState mis;

    PlayState mPlayerState; //播放状态



    /* options specified by the user */
    AVInputFormat *file_iformat;
    const char *input_filename;
    const char *window_title;
    int fs_screen_width;
    int fs_screen_height;
    int screen_width  = 0;
    int screen_height = 0;
    int audio_disable=0;
    int video_disable=0;
#if 0
    int wanted_stream[AVMEDIA_TYPE_NB] = {
        [AVMEDIA_TYPE_AUDIO]    = -1,
        [AVMEDIA_TYPE_VIDEO]    = -1,
        [AVMEDIA_TYPE_SUBTITLE] = -1,
    };
#endif
    int wanted_stream[AVMEDIA_TYPE_NB];
    int seek_by_bytes = -1;
    int display_disable=0;
    int show_status = 1;
    int av_sync_type = AV_SYNC_EXTERNAL_CLOCK;//AV_SYNC_VIDEO_MASTER; //AV_SYNC_AUDIO_MASTER;
    //int64_t start_time = AV_NOPTS_VALUE;
    //int64_t duration = AV_NOPTS_VALUE;
    //int step = 0;
    int workaround_bugs = 1;
    int fast = 0;
    int genpts = 0;
    int idct = FF_IDCT_AUTO;
    enum AVDiscard skip_frame       = AVDISCARD_DEFAULT;
    enum AVDiscard skip_idct        = AVDISCARD_DEFAULT;
    enum AVDiscard skip_loop_filter = AVDISCARD_DEFAULT;
    int error_concealment = 3;
    //int noautoexit;
    //int decoder_reorder_pts = -1;
    int exit_on_keydown;
    int exit_on_mousedown;
    //int loop = 1;
    int framedrop = 1;
    //int infinite_buffer = 0;

    //int rdftspeed = 20;
    //char *vfilters = NULL;
    //int autorotate = 1;

    /* current context */
    int is_full_screen;
    //static PlayerState player_state;
    PlayerState *player = NULL;//&player_state;
    //int64_t audio_callback_time;

    //AVPacket flush_pkt;

    SDL_Surface *screen;

    //PlayerState *global_video_state;
};

#endif // VideoThread_H
