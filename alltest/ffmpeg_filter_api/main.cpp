#include <QCoreApplication>
#include <QtCore>
#include <QDebug>
#include <QtGlobal>
#include <QString>
#include <QByteArray>
#include <QObject> 
#include <QFontMetrics> 
#include <QFont> 
#include <cstddef>     /* offsetof */
#include <QVector>
#include <QtMath>
#include <iostream>
#include <tr1/memory>

#include <QImage>
#include <QThread>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>

#include <libavutil/eval.h>
#include <libavutil/pixdesc.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
}
#include "filter_crop.h"
#include "filter_pad.h"
#include "filter_overlay.h"
#include "image_avframe.h"
#include "filter_zoompan.h"
#include "filter_blend.h"
#include "filter_colorchannelmixer.h"
#include "filter_eq.h"
#include "filter_rotate.h"
#include "filter_hflip.h"
#include "filter_vflip.h"
#include "filter_subtitles.h"
#include <omp.h>
#define OPEN_MP_NUM_PROCESSORS omp_get_num_procs()
#define qInfo qDebug
static AVFormatContext *fmt_ctx;
static AVCodecContext *dec_ctx;
AVFilterContext *buffersink_ctx;
AVFilterContext *buffersrc_ctx;
AVFilterGraph *filter_graph;
static int video_stream_index = -1;
///static int64_t last_pts = AV_NOPTS_VALUE;
static int64_t sws_flags = SWS_BILINEAR;//SWS_BICUBIC;

//AVFrame* copy_frame(AVFrame* in, int width, int height);
AVFrame* copy_frame(AVFrame* in, int format, int width, int height);
static int open_input_file(const char *filename);
//w:320 h:240 fmt:yuvj444p sar:1/1 -> w:320 h:240 fmt:yuva420p sar:1/1 flags:0x2
//AVFrame* convertFormat(AVFrame *frame, int format);
AVFrame* convertFormat(AVFrame *frame, int dstW, int dstH, int format);
#define BYTE uint8_t
BYTE** CreatImage(BYTE* image, unsigned int width, unsigned int height);
int TempltExcute(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y);
void SetPixel2(BYTE** imageBuf1, int x, int y, int a);
int GetAsh(BYTE** imageBuf, int x, int y);

void displayFrame(AVFrame *frame);

int testVideoFilter(int argc, char *argv[]);
int testZoomPan(int argc, char *argv[]);
int testPicLut(int argc, char *argv[]);
int testColorChannelMixer(int argc, char *argv[]);
int testSubtitles(int argc, char *argv[]);
int testTransparentBGSubtitles(int argc, char *argv[]);
int testScale(int argc, char *argv[]);
///int testBlend(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    av_log(NULL, AV_LOG_INFO, "main\n");

    //testScale(argc, argv);

    //testVideoFilter(argc, argv);

    //testZoomPan(argc, argv);

    //for(int i=0; i<1000;i++)
    {
    //testPicLut(argc, argv);
    //QThread::msleep(100);
    }

    //testColorChannelMixer(argc, argv);

    //testSubtitles(argc, argv);

    testTransparentBGSubtitles(argc, argv);

    ////testBlend(argc, argv);
    return 0;
}
static int open_input_file(const char *filename)
{
    int ret;
    AVCodec *dec;

    if ((ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }

    if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }

    /* select the video stream */
    ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &dec, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");
        return ret;
    }
    video_stream_index = ret;

    /* create decoding context */
    dec_ctx = avcodec_alloc_context3(dec);
    if (!dec_ctx)
        return AVERROR(ENOMEM);
    avcodec_parameters_to_context(dec_ctx, fmt_ctx->streams[video_stream_index]->codecpar);
    av_opt_set_int(dec_ctx, "refcounted_frames", 1, 0);

    /* init the video decoder */
    if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
        return ret;
    }

    return 0;
}

AVFrame* convertFormat(AVFrame *frame, int dstW, int dstH, int format)
{
    AVFrame *pFrame=NULL;
    uint8_t *out_buffer=NULL; //解码后的rgb数据
    struct SwsContext *img_convert_ctx;  //用于解码后的视频格式转换

    double time = av_gettime_relative() / 1000.0;
    img_convert_ctx = sws_getContext(frame->width, frame->height,
            //pCodecCtx->pix_fmt, frame->width, frame->height,
            (AVPixelFormat)frame->format, dstW, dstH,
            (AVPixelFormat)format, sws_flags, NULL, NULL, NULL);

    //pFrame = copy_frame(frame, format, dstW, dstH);
#if 1
    pFrame = av_frame_alloc();
    memset(pFrame, 0, sizeof(AVFrame));
    //

    int numBytes = avpicture_get_size((AVPixelFormat)format, dstW, dstH); //av_image_get_buffer_size
    //av_image_get_buffer_size(pix_fmt, width, height, 1);

    out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    qDebug()<<"convertFormat new out_buffer: "<<out_buffer<<" size: "<<(numBytes * sizeof(uint8_t))
        <<" width: "<<dstW<<" height: "<<dstH<<" format: "<<format;
    avpicture_fill((AVPicture *) pFrame, out_buffer, (AVPixelFormat)format,
            dstW, dstH); 
    //av_image_fill_arrays(frame->data, frame->linesize, buf, avctx->pix_fmt, avctx->width, avctx->height, 1);
    pFrame->format = format;
    pFrame->width  = dstW;
    pFrame->height = dstH;
    int ret = av_frame_copy_props(pFrame, frame);
    if (ret < 0) {
        av_frame_unref(pFrame);
        fprintf(stderr, "Could not av_frame_copy_props\n");
        exit(1);
    }
#endif
    sws_scale(img_convert_ctx,
            (uint8_t const * const *) frame->data,
            frame->linesize, 0, frame->height, pFrame->data,
            pFrame->linesize);
#if 0
    qDebug()<<"convertFormat free out_buffer: "<<out_buffer;
    av_free(out_buffer); 
    qDebug()<<"convertFormat free pFrame: "<<pFrame;
    av_frame_free(&pFrame);
    ////av_free(pFrame);
#endif
    sws_freeContext(img_convert_ctx);

    ////frame = pFrame;
    av_log(NULL, AV_LOG_INFO, "convertFormat waste_time: %f\n", av_gettime_relative() / 1000.0-time);
    return pFrame;
}


void displayFrame(AVFrame *frame)
{
#if 1
    AVFrame *pFrame=NULL;
    uint8_t *out_buffer=NULL; //解码后的rgb数据
#endif
    struct SwsContext *img_convert_ctx;  //用于解码后的视频格式转换
    ////AVCodecContext *pCodecCtx = dec_ctx;
    static int idx=0;
    idx++;

    pFrame = av_frame_alloc();
    //
    img_convert_ctx = sws_getContext(frame->width, frame->height,
            //pCodecCtx->pix_fmt, frame->width, frame->height,
            (AVPixelFormat)frame->format, frame->width, frame->height,
            AV_PIX_FMT_RGBA, sws_flags, NULL, NULL, NULL);

    int numBytes = avpicture_get_size(AV_PIX_FMT_RGBA, frame->width,frame->height);

    if(!out_buffer)
        out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    qDebug()<<"displayFrame new out_buffer: "<<out_buffer<<" size: "<<(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrame, out_buffer, AV_PIX_FMT_RGBA,
            frame->width, frame->height);

    sws_scale(img_convert_ctx,
            (uint8_t const * const *) frame->data,
            frame->linesize, 0, frame->height, pFrame->data,
            pFrame->linesize);
    //把这个RGB数据 用QImage加载
    //QImage tmpImg((uchar *)out_buffer,frame->width,frame->height,QImage::Format_RGB32);
    ////QImage tmpImg((uchar *)out_buffer,frame->width,frame->height,QImage::Format_RGB32);
    QImage tmpImg((uchar *)out_buffer,frame->width,frame->height,QImage::Format_RGBA8888);
    tmpImg.save(QString("images/%1.png").arg(idx));
    //tmpImg.save(QString("/home/gino/ffmpeg-3.0.2/macjpg/%1.jpg").arg(idx));
    //tmpImg.save(QString("/c/shareproject/jpg/tmp/%1.jpg").arg(idx));

    qDebug()<<"displayFrame free out_buffer: "<<out_buffer;
    av_free(out_buffer); 
    qDebug()<<"displayFrame free pFrame: "<<pFrame;
    av_frame_free(&pFrame);
    ////av_free(pFrame);
    sws_freeContext(img_convert_ctx);
}
#define FILTER_BLEND
//#define FILTER_OVERLAY
//#define FILTER_CROP
//#define FILTER_PAD
int testVideoFilter(int argc, char *argv[])
{
    av_log(NULL, AV_LOG_INFO, "testVideoFilter\n");
    int ret;
    AVPacket packet;
    AVFrame *frame = av_frame_alloc();
    //AVFrame *filt_frame = av_frame_alloc();
    int64_t frame_count_out=1;
    double pts;
    //AVFormatContext *fmt_ctx = NULL;
    AVFrame *ori_frame = NULL;
    uint8_t* tmp=NULL;
    double time;

#ifdef FILTER_BLEND
    //blend
    AVFrame *pblend_frame = NULL;
    AVCodecContext *blend_ctx=NULL;
    BlendContext bc, *s=&bc;
#endif
#ifdef FILTER_OVERLAY
    //overlay
    AVFrame *overlay_frame = NULL;
    AVCodecContext *overlay_ctx=NULL;
    OverlayContext oc, *s=&oc;
#endif
#ifdef FILTER_CROP
    //crop
    CropContext cc, *s=&cc;
    char w[]="512";//"640";
    char h[]="386";//"192";//"360";
#endif
#ifdef FILTER_PAD
    //pad
    PadContext pc, *s=&pc;
    char w[]="1024";
    char h[]="786";
#endif
    char x[]="0";
    char y[]="400";

    if (!frame ) {
        perror("Could not allocate frame");
        exit(1);
    }
    if (argc != 3) {
        fprintf(stderr, "Usage: %s videofile jpgfile\n", argv[0]);
        exit(1);
    }

    av_register_all();
    avfilter_register_all();


    if ((ret = open_input_file(argv[1])) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "error open file: %s\n", argv[1]);
        goto end;
    }
#ifdef FILTER_CROP
    //crop
    //crop_init(s, w, h , x, y, 1, 0);
    crop_init_int(s, 500, 300 , 200, 100, 1, 1);
    //crop_config_input(dec_ctx, s);
#endif
#ifdef FILTER_PAD
    //pad
    //pad_init(s, w, h , x, y);
    pad_init_int(s, 1024, 786 , 50, 100);
#endif
#ifdef FILTER_OVERLAY
    //overlay
    image_to_avframe(argv[2], fmt_ctx, overlay_ctx, overlay_frame);
    //overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_YUVA420P);
    overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_RGBA);
    displayFrame(overlay_frame);
    overlay_init(s, x, y);
#endif
#ifdef FILTER_BLEND
    //blend
    image_to_avframe(argv[2], fmt_ctx, blend_ctx, ori_frame);
    //pblend_frame=convertFormat(pblend_frame, AV_PIX_FMT_YUVA420P);
    pblend_frame=convertFormat(ori_frame, ori_frame->width, ori_frame->height, AV_PIX_FMT_YUVA420P);
    av_frame_free(&ori_frame);
    //pblend_frame=convertFormat(pblend_frame, pblend_frame->width, pblend_frame->height,AV_PIX_FMT_RGBA);
    displayFrame(pblend_frame);
#endif


    /* read all packets */
    while (1) {
        if ((ret = av_read_frame(fmt_ctx, &packet)) < 0)
            break;

        if (packet.stream_index == video_stream_index) {
            ret = avcodec_send_packet(dec_ctx, &packet);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
                break;
            }

            while (ret >= 0) {
                ret = avcodec_receive_frame(dec_ctx, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
                    goto end;
                }

                if (ret >= 0) {
                    frame->pts = av_frame_get_best_effort_timestamp(frame);

#ifdef FILTER_CROP
    //crop
                    AVFrame* scale_frame=convertFormat(frame, frame->width, frame->height, AV_PIX_FMT_RGBA);
                    crop_config_input(scale_frame, s);
                    pts = (double)frame_count_out/25;
                    crop_filter_frame(s, scale_frame, frame_count_out++, pts);
                    displayFrame(scale_frame);
#endif
#ifdef FILTER_PAD
    //pad
                    AVFrame* scale_frame=convertFormat(frame, frame->width, frame->height, AV_PIX_FMT_RGBA);
                    pad_config_input(scale_frame, s);
                    //AVFrame* out=NULL;
                    AVFrame* out=convertFormat(frame, s->w, s->h, AV_PIX_FMT_RGBA);
                    pad_filter_frame(s, scale_frame, out);
                    ///av_log(NULL, AV_LOG_INFO, "out: %d\n", (int)out);
                    displayFrame(out);
#endif
#ifdef FILTER_OVERLAY
    //overlay
                    AVFrame* scale_frame=convertFormat(frame, frame->width, frame->height, AV_PIX_FMT_RGBA);
                    overlay_config_input_main(scale_frame, s);
                    overlay_config_input_overlay(scale_frame, overlay_frame, s);
                    pts = (double)frame_count_out/25;
                    overlay_blend(s, scale_frame, overlay_frame, frame_count_out++, pts);
                    displayFrame(scale_frame);
#endif
#ifdef FILTER_BLEND
    //blend
                    //AVFrame* scale_frame=convertFormat(frame, pblend_frame->width, pblend_frame->height, AV_PIX_FMT_RGBA);
                    AVFrame* scale_frame=convertFormat(frame, pblend_frame->width, pblend_frame->height, AV_PIX_FMT_YUVA420P);
                    //displayFrame(scale_frame);

                    blend_init(s, scale_frame, pblend_frame, "if(crossfade,1.5,2)");
                    AVFrame* out=copy_frame(scale_frame, scale_frame->format, scale_frame->width, scale_frame->height);

                    //pts = frame->pts == AV_NOPTS_VALUE ? NAN : frame->pts * av_q2d(dec_ctx->time_base);
                    pts = (double)frame_count_out/25;

                    time = av_gettime_relative() / 1000.0;
                    blend_frame(s, scale_frame, pblend_frame, out, frame_count_out++, pts);
                    av_log(NULL, AV_LOG_INFO, "testVideoFilter blend_frame waste_time: %f\n", av_gettime_relative() / 1000.0-time);
                    displayFrame(out);
                    av_frame_free(&scale_frame); //uncomplete
                    av_frame_free(&out); //uncomplete
#endif

                    av_frame_unref(frame); //uncomplete
                }
            }
        }
        av_packet_unref(&packet);
    }
end:
#ifdef FILTER_CROP
    //crop
    crop_uninit(s);
#endif
#ifdef FILTER_PAD
    //pad
    pad_uninit(s);
#endif
#ifdef FILTER_OVERLAY
    //overlay
    image_avframe_close(fmt_ctx, overlay_ctx);
    overlay_uninit(s);
#endif
#ifdef FILTER_BLEND
    //blend_uninit(s);
    //blend
    tmp=pblend_frame->data[0];
    av_frame_free(&pblend_frame);
    av_free(tmp);
    image_avframe_close(fmt_ctx, blend_ctx);
#endif
    ///avfilter_graph_free(&filter_graph);
    avcodec_free_context(&dec_ctx);
    avformat_close_input(&fmt_ctx);
    av_frame_free(&frame);
    ///av_frame_free(&filt_frame);

    if (ret < 0 && ret != AVERROR_EOF) {
        ////fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        qInfo()<<"Error occurred ret: "<<ret;
        exit(1);
    }

    return 0;
}

void colorTemperatureToRGB(float kelvin, ColorChannelMixerContext *s)
{
    float temp = kelvin / 100;
    float red, green, blue;

    if( temp <= 66 )
    { 
        red = 255; 
        green = temp;
        green = 99.4708025861 * qLn(green) - 161.1195681661;
        
        if( temp <= 19){
            blue = 0;
        } 
        else {
            blue = temp-10;
            blue = 138.5177312231 * qLn(blue) - 305.0447927307;
        }
    } 
    else 
    {
        red = temp - 60;
        red = 329.698727446 * qPow(red, -0.1332047592);
        
        green = temp - 60;
        green = 288.1221695283 * qPow(green, -0.0755148492 );

        blue = 255;
    }
    qDebug()<<"red: "<<red<<" green: "<<green<<" blue: "<<blue;
#if 0
    lutrgb=QString("lutrgb=%1:%2:%3")
        .arg(red<0?0:(red>255?255:red))
        .arg(green<0?0:(green>255?255:green))
        .arg(blue<0?0:(blue>255?255:blue))
        ;
#endif
    // colorbalance=.667:0.772:1
#if 0
    lutrgb=QString("colorbalance=rm=%1:gm=%2:bm=%3")
        .arg(red<0?0:(red>255?1:(red*2/255-1)))
        .arg(green<0?0:(green>255?1:(green*2/255-1)))
        .arg(blue<0?0:(blue>255?1:(blue*2/255-1)))
        ;
#endif
    s->rr = red<0?0:(red>255?1:(red*4/255-2));
    s->gg = green<0?0:(green>255?1:(green*4/255-2));
    s->bb = blue<0?0:(blue>255?1:(blue*4/255-2));
#if 0
    lutrgb=QString("colorchannelmixer=%1:0:0:0: 0:%2:0:0: 0:0:%3")
        .arg(red<0?0:(red>255?1:(red*4/255-2)))
        .arg(green<0?0:(green>255?1:(green*4/255-2)))
        .arg(blue<0?0:(blue>255?1:(blue*4/255-2)))
        ;
#endif
}

int testPicLut(int argc, char *argv[])
{
    //saturation: 饱和度, brightness: 亮度, contrast: 对比度
    //黑白 eq=saturation=0
    AVFrame *ori_frame = NULL;
    AVCodecContext *overlay_ctx=NULL;
    int64_t frame_count=100;

    av_log(NULL, AV_LOG_INFO, "testPicLut\n");
    if (argc != 3) {
        fprintf(stderr, "Usage: %s any jpgfile\n", argv[0]);
        exit(1);
    }
    av_register_all();

    image_to_avframe(argv[2], fmt_ctx, overlay_ctx, ori_frame);
    image_avframe_close(fmt_ctx, overlay_ctx);

    //crop
    {
        CropContext cc, *s=&cc;
        crop_init_int(s, 512, 385 , 0, 400, 1, 1);
        AVFrame * overlay_frame=convertFormat(ori_frame, ori_frame->width, ori_frame->height, AV_PIX_FMT_RGBA);
        crop_config_input(overlay_frame, s);
        crop_filter_frame(s, overlay_frame, 0, 0);
        displayFrame(overlay_frame);
        uint8_t* tmp=overlay_frame->data[0];
        av_frame_free(&overlay_frame);
        av_free(tmp);
        goto END_TESTPIC;
    }

    {
        //overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_RGBA);
        AVFrame * overlay_frame=convertFormat(ori_frame, ori_frame->width, ori_frame->height, AV_PIX_FMT_YUVA420P);
        displayFrame(overlay_frame);

        EQContext ec, *s=&ec;
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        double time = av_gettime_relative() / 1000.0;
        double brightness=0; // -100~100 -1.0~1.0 0
        double contrast=0;   // -100~100 -2.0~2.0 1
        double saturation=200; // -100~100 0.0~3.0  1
        //eq_initialize(s, brightness/100, contrast/50, (saturation+100)*3/200);
        //eq_initialize(s, 0, 1, 0);
        eq_initialize(s, brightness, contrast, saturation);
        eq_filter_frame(s, overlay_frame, 1, pFrame);
        av_log(NULL, AV_LOG_INFO, "eq_filter_frame waste_time: %f\n", av_gettime_relative() / 1000.0-time);
        displayFrame(pFrame);
        av_frame_free(&pFrame);
        //eq_uninit(s);
        uint8_t* tmp=overlay_frame->data[0];
        av_frame_free(&overlay_frame);
        av_free(tmp);
    }

    {
        AVFrame * overlay_frame=convertFormat(ori_frame, ori_frame->width, ori_frame->height, AV_PIX_FMT_RGBA);
        //overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_YUVA420P);
        RotContext ec, *s=&ec;
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        char angle[]="90*PI/180";
        //char* outw=QString::number(pFrame->width).toStdString().c_str();
        //char* outh=QString::number(pFrame->height).toStdString().c_str();
        char outw[]="512";
        char outh[]="384";
        char color[]="black";
        rotate_init(s, angle, outw, outh, color);
        rotate_config_props(s, overlay_frame);
        rotate_filter_frame(s, overlay_frame, 1, pFrame);
        displayFrame(pFrame);
        av_frame_free(&pFrame);
        rotate_uninit(s);
        uint8_t* tmp=overlay_frame->data[0];
        av_frame_free(&overlay_frame);
        av_free(tmp);
    }
    {
        AVFrame * overlay_frame=convertFormat(ori_frame, ori_frame->width, ori_frame->height, AV_PIX_FMT_RGBA);
        //overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_YUVA420P);
        HFlipContext ec, *s=&ec;
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);

        hflip_init(s);
        hflip_config_props(s, overlay_frame);
        hflip_filter_frame(s, overlay_frame, pFrame);
        displayFrame(pFrame);
        av_frame_free(&pFrame);
        uint8_t* tmp=overlay_frame->data[0];
        av_frame_free(&overlay_frame);
        av_free(tmp);
    }
    {
        AVFrame * overlay_frame=convertFormat(ori_frame, ori_frame->width, ori_frame->height, AV_PIX_FMT_RGBA);
        //overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_YUVA420P);
        VFlipContext ec, *s=&ec;
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);

        vflip_init(s);
        vflip_config(s, overlay_frame);
        vflip_filter_frame(s, overlay_frame, pFrame);
        displayFrame(pFrame);
        av_frame_free(&pFrame);
        uint8_t* tmp=overlay_frame->data[0];
        av_frame_free(&overlay_frame);
        av_free(tmp);
    }
END_TESTPIC:
    av_frame_free(&ori_frame);
    return 0;
}

int testColorChannelMixer(int argc, char *argv[])
{
    AVFrame *overlay_frame = NULL;
    AVCodecContext *overlay_ctx=NULL;
    ColorChannelMixerContext cc, *s=&cc;
    int64_t frame_count=100;

    av_log(NULL, AV_LOG_INFO, "ColorChannelMixer\n");
    if (argc != 3) {
        fprintf(stderr, "Usage: %s any jpgfile\n", argv[0]);
        exit(1);
    }
    av_register_all();

    image_to_avframe(argv[2], fmt_ctx, overlay_ctx, overlay_frame);
    //overlay_frame=convertFormat(overlay_frame, AV_PIX_FMT_YUVA420P);
    //overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_YUVA420P);
    overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_RGBA);
    displayFrame(overlay_frame);

#if 1
    for(int i=-100; i<frame_count; i+=10)
    {
        colorchannelmixer_init(s);
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        colorTemperatureToRGB(i*195+20500, s);
        colorchannelmixer_config_output(s, overlay_frame->format);
        qDebug()<<"ColorChannelMixer s->rr: "<<s->rr<<" s->gg: "<<s->gg<<" s->rr: "<<s->rr;
        colorchannelmixer_filter_frame(s, overlay_frame, pFrame);
        displayFrame(pFrame);
        av_frame_free(&pFrame);
        colorchannelmixer_uninit(s);
    }
#endif
    //负片
    {
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        AVFrame* in=overlay_frame;
        AVFrame* out=pFrame;
        {
#define R 0
#define G 1
#define B 2
#define A 3
            uint8_t rgba_map[4];
            ff_fill_rgba_map(s->rgba_map, (AVPixelFormat)overlay_frame->format);
            const uint8_t roffset = s->rgba_map[R];
            const uint8_t goffset = s->rgba_map[G];
            const uint8_t boffset = s->rgba_map[B];
            const uint8_t aoffset = s->rgba_map[A];
            const uint8_t *srcrow = in->data[0];
            uint8_t *dstrow;
            int i, j;

            dstrow = out->data[0];
            for (i = 0; i < in->height; i++) {
                const uint8_t *src = srcrow;
                uint8_t *dst = dstrow;

                for (j = 0; j < in->width * 4; j += 4) {
                    const uint8_t rin = src[j + roffset];
                    const uint8_t gin = src[j + goffset];
                    const uint8_t bin = src[j + boffset];
                    const uint8_t ain = src[j + aoffset];
                    dst[j + roffset] = 255-rin;
                    dst[j + goffset] = 255-gin;
                    dst[j + boffset] = 255-bin;
                    //dst[j + aoffset] = ;
                }
                srcrow += in->linesize[0];
                dstrow += out->linesize[0];
            }
        }
        displayFrame(pFrame);
        av_frame_free(&pFrame);
    }
    //Emboss 浮雕
    {
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        AVFrame* in=overlay_frame;
        AVFrame* out=pFrame;
        {
            int r = 0, g = 0, b = 0;
            int width=in->width;
            int height=in->height;
            uint8_t* tempPixel = out->data[0];
            uint8_t* pixel= in->data[0];

            //int sum = width * height *4;
            //memcpy(tempPixel, pixel, sum);		
            for(int i = 0; i < height-1; i++) 
            {
                for (int j = 0; j < (width-1)*4; j+=4) 
                {
                    //处理像素值
                    b = abs(pixel[i*width*4+j] 
                            -pixel[(i+1)*width*4+j+4]+128);
                    g = abs(pixel[i*width*4+j+1]
                            -pixel[(i+1)*width*4+j+5]+128);
                    r = abs(pixel[i*width*4+j+2]
                            -pixel[(i+1)*width*4+j+6]+128);

                    //对于越界的像素值进行处理
                    if (r>255)
                        r=255;

                    if (g>255)
                        g=255;

                    if (b>255)
                        b=255;

                    tempPixel[i*width*4 + j]		= b;//blue
                    tempPixel[i*width*4 + j + 1]	= g;//green
                    tempPixel[i*width*4 + j + 2]	= r;//red
                }
            }
            for (int k = width * 4 * (height-1); k < width*4*height; k += 4) 
            {
                tempPixel[k]=128;
                tempPixel[k+1]=128;
                tempPixel[k+2]=128;
            }

            for (int l = (width-1) * 4; l < width*4*height; l += width*4) 
            {
                tempPixel[l]=128;
                tempPixel[l+1]=128;
                tempPixel[l+2]=128;
            }
        }
        displayFrame(pFrame);
        av_frame_free(&pFrame);
    }
    //黑白 no
    {
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        AVFrame* in=overlay_frame;
        AVFrame* out=pFrame;
        {
            int width=in->width;
            int height=in->height;
            uint8_t* tempPixel = out->data[0];
            uint8_t* pixel= in->data[0];

            int sum = width * height *4;
            for(int i = 0; i < sum; i += 4)
            {
                //平均值法
                tempPixel[i]	 = (pixel[i] + pixel[i+1] + pixel[i+2]) / 3; //blue
                tempPixel[i + 1] = pixel[i];										 //green
                tempPixel[i + 2] = pixel[i];										 //red
            }
        }
        displayFrame(pFrame);
        av_frame_free(&pFrame);
    }
    //雾化 HorFog 浓雾
    {
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        AVFrame* in=overlay_frame;
        AVFrame* out=pFrame;
        {
            int f = 10;
            int k;
            int n;
            int width=in->width;
            int height=in->height;
            uint8_t* tempPixel = out->data[0];
            uint8_t* pixel= in->data[0];

            //int sum = width * height *4;
            //memcpy(tempPixel, pixel, sum);		
            for(int i = 0; i < height; i++) 
            {
                for (int j = 0; j < width*4; j+=4) 
                {
                    k = abs(rand() % f);
                    n = j + k*4;

                    if (n>(width-1) * 4)
                        n = (width-1)*4;

                    tempPixel[i*width*4 + j]     = pixel[i*width*4 + n];
                    tempPixel[i*width*4 + j + 1] = pixel[i*width*4 + n + 1];
                    tempPixel[i*width*4 + j + 2] = pixel[i*width*4 + n + 2];
                    tempPixel[i*width*4 + j + 3] = pixel[i*width*4 + n + 3];
                }
            }
        }
        displayFrame(pFrame);
        av_frame_free(&pFrame);
    }
    //垂直雾化 VerFog 浓雾
    {
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        AVFrame* in=overlay_frame;
        AVFrame* out=pFrame;
        {
            int f = 10;
            int k;
            int m;
            int width=in->width;
            int height=in->height;
            uint8_t* tempPixel = out->data[0];
            uint8_t* pixel= in->data[0];

            //int sum = width * height *4;
            //memcpy(tempPixel, pixel, sum);		
            for(int i = 0; i < height; i++) 
            {
                for (int j = 0; j < width*4; j+=4) 
                {
                    k = abs(rand() % f);
                    m = i + k;
                    if (m>height-1)
                        m = height-1;

                    tempPixel[i*width*4 + j]     = pixel[m*width*4 + j];
                    tempPixel[i*width*4 + j + 1] = pixel[m*width*4 + j + 1];
                    tempPixel[i*width*4 + j + 2] = pixel[m*width*4 + j + 2];
                    tempPixel[i*width*4 + j + 3] = pixel[m*width*4 + j + 3];
                }
            }
        }
        displayFrame(pFrame);
        av_frame_free(&pFrame);
    }
    //复合雾化 ComFog 浓雾
    {
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        AVFrame* in=overlay_frame;
        AVFrame* out=pFrame;
        {
            int f = 10;
            int k;
            int m, n;
            int width=in->width;
            int height=in->height;
            uint8_t* tempPixel = out->data[0];
            uint8_t* pixel= in->data[0];

            //int sum = width * height *4;
            //memcpy(tempPixel, pixel, sum);		
            for(int i = 0; i < height; i++) 
            {
                for (int j = 0; j < width*4; j+=4) 
                {
                    k = abs(rand() % f);
                    m = i + k;
                    n = j + k * 4;

                    // 对超出图像区域的点进行相应处理
                    if (m > height-1) 
                        m = height-1;

                    if (n > (width-1) * 4) 
                        n = (width-1) * 4;

                    // 更新像素数组
                    tempPixel[i*width*4 + j]     = pixel[m*width*4 + n];
                    tempPixel[i*width*4 + j + 1] = pixel[m*width*4 + n + 1];
                    tempPixel[i*width*4 + j + 2] = pixel[m*width*4 + n + 2];
                    tempPixel[i*width*4 + j + 3] = pixel[m*width*4 + n + 3];
                }
            }
        }
        displayFrame(pFrame);
        av_frame_free(&pFrame);
    }
    //Mosaic 马赛克
    {
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        AVFrame* in=overlay_frame;
        AVFrame* out=pFrame;
        {
            int w=in->width;
            int h=in->height;
            int size=10; //马赛克方块的大小
            uint8_t* tempPixel = out->data[0];
            uint8_t* pixel= in->data[0];

            BYTE** imageBuf0 = CreatImage(pixel, w, h);
            BYTE** imageBuf1 = CreatImage(tempPixel, w, h);
            int x = 0,y = 0;        //马赛克方格的左上角坐标
            int tx = 0,ty = 0;      //马赛克方格的右下角坐标
            int i, j;
            while(true)
            {
                if(tx == w)  //如果已经处理完一行
                {
                    if(ty == h) break;

                    //确定马赛克方格的上方边界
                    y = ty;  

                    //确定马赛克方格的下方边界
                    if(y + size <= h)
                        ty = y + size;
                    else
                        ty = h;

                    //把方格位置移动到下一行首部
                    x = 0;
                }

                //确定马赛克方格的左边边界
                if(x + size <= w)
                    tx = x + size;
                else
                    tx = w;

                for(i = y;i < ty; i++)
                    for(j = x; j<tx; j++)
                    {
                        //把马赛克方格填充为中点的象素
                        imageBuf1[i][j*4]	  = imageBuf0[(y+ty) / 2][(x+tx)/2*4];
                        imageBuf1[i][j*4 + 1] = imageBuf0[(y+ty) / 2][(x+tx)/2*4 + 1];
                        imageBuf1[i][j*4 + 2] = imageBuf0[(y+ty) / 2][(x+tx)/2*4 + 2];
                        imageBuf1[i][j*4 + 3] = imageBuf0[(y+ty) / 2][(x+tx)/2*4 + 3];
                    }
                //水平移动到下一个马赛克方格
                x = tx;
            }

            free(imageBuf0);
            free(imageBuf1);
        }
        displayFrame(pFrame);
        av_frame_free(&pFrame);
    }

    //素描 LaplacianB
    {
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        AVFrame* in=overlay_frame;
        AVFrame* out=pFrame;
        {
            int w=in->width;
            int h=in->height;
            uint8_t* tempPixel = out->data[0];
            uint8_t* pixel= in->data[0];

            //定义临时图象存储空间
            BYTE* tempImage;

            tempImage = (BYTE*)malloc(sizeof(BYTE)*w*h*4);
            //将图像转化为矩阵形式
            BYTE** imageBuf0 = CreatImage(pixel, w, h);
            BYTE** imageBuf1 = CreatImage(tempPixel, w, h);
            BYTE** tempImageBuf = CreatImage(tempImage, w, h);
            double scale = 2;
            //拉普拉斯正相模板
            int templt[9]={ 1, 1, 1, 1,-8, 1, 1, 1, 1 };
            //噪声检测模板
            int templtTest1[9]={ 1, 1,-1, 1, 0,-1, 1,-1,-1 };
            int templtTest2[9]={ 1, 1, 1,-1, 0, 1,-1,-1,-1 };
            //模糊处理模板
            int templtAve[9]={ 1, 1, 1, 1, 4, 1, 1, 1, 1};
            int x,y;
            int a,b,b1,b2;

            //依次对原图像的每个像素进行处理
            for(y = 1; y < h - 1; y++)
            {
                for(x = 1; x < w - 1; x++)
                {
                    //拉普拉斯卷积运算
                    a=TempltExcute(imageBuf0, w, h, templt, 3, x, y);
                    //噪声检测
                    b1=abs(TempltExcute(imageBuf0, w, h, templtTest1, 3, x, y));
                    b2=abs(TempltExcute(imageBuf0, w, h, templtTest2, 3, x, y));
                    b=b1>b2?b1:b2;
                    if(b<25) a=0;
                    else
                    {
                        a = (int)(a * scale);
                        //过限处理
                        if(a > 255) a = 255;
                        else if(a < 32) a=0;
                    }
                    //反色处理
                    a=255-a;
                    SetPixel2(tempImageBuf, x, y, a);
                }
            }

            //模糊处理
            for(y = 1; y < h - 1; y++)
            {
                for(x = 1; x < w - 1; x++)
                {
                    a=TempltExcute(tempImageBuf, w, h, templtAve, 3, x, y) / 12;
                    SetPixel2(imageBuf1, x, y, a);
                }
            }

            free(tempImage);
            free(imageBuf0);
            free(imageBuf1);
            free(tempImageBuf);

        }
        displayFrame(pFrame);
        av_frame_free(&pFrame);
    }
    //黑白 grayscale
    {
        colorchannelmixer_init(s);
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        colorchannelmixer_set_rgb(s, .3, .4,.3,0,.3,.4,.3,0,.3,.4,.3);
        colorchannelmixer_config_output(s, overlay_frame->format);
        qDebug()<<"ColorChannelMixer s->rr: "<<s->rr<<" s->gg: "<<s->gg<<" s->rr: "<<s->rr;
        colorchannelmixer_filter_frame(s, overlay_frame, pFrame);
        displayFrame(pFrame);
        av_frame_free(&pFrame);
        colorchannelmixer_uninit(s);
    }
    //怀旧 sepia tones
    {
        colorchannelmixer_init(s);
        AVFrame* pFrame=copy_frame(overlay_frame, overlay_frame->format, overlay_frame->width, overlay_frame->height);
        colorchannelmixer_set_rgb(s, .393,.769,.189,0,.349,.686,.168,0,.272,.534,.131);
        colorchannelmixer_config_output(s, overlay_frame->format);
        qDebug()<<"ColorChannelMixer s->rr: "<<s->rr<<" s->gg: "<<s->gg<<" s->rr: "<<s->rr;
        colorchannelmixer_filter_frame(s, overlay_frame, pFrame);
        displayFrame(pFrame);
        av_frame_free(&pFrame);
        colorchannelmixer_uninit(s);
    }
    //av_frame_free(&overlay_frame); //error uncomplete
////    sws_freeContext(overlay_ctx);
    return 0;
}

enum MMEffectType{
    MMET_UNKNOW=0,
    MMET_TRANSITION_FRAG,
    MMET_TRANSITION_FFMPEG,
    MMET_FILTER_FRAG,
    MMET_FILTER_FFMPEG,
    MMET_MOTION_FRAG,
    MMET_MOTION_FFMPEG,
};
#if 0
typedef struct STZoompanVar {
    int allframes;
}STZoompanVar;
typedef struct STZoompanEle {
    STZoompanEle(){}
    STZoompanEle(const QByteArray& des, const QVector<int>& o, const QVector<int>& t)
        : description(des), offset(o), type(t){}
    //QString description;
    QByteArray description;
    QVector<int> offset;
    QVector<int> type;
}STZoompanEle;
typedef struct STEffectsInfo {
    STEffectsInfo(){}
    //STEffectsInfo(int t, const QString& n, const STZoompanEle& z, const STZoompanEle& x, const STZoompanEle& y)
    //    :type(t),name(n),zoom_expr(z),x_expr(x),y_expr(y){}
    STEffectsInfo(int t, const QString& n, const QVector<STZoompanEle>& e)
        :type(t),name(n),elements(e){}
    int type;
    QString name;
    QVector<STZoompanEle> elements;
    //STZoompanEle x_expr;
    //STZoompanEle y_expr;
}STEffectsInfo;
#define OFFSET(x) offsetof(struct STZoompanVar, x)

#endif
typedef struct STEffectsInfo {
    STEffectsInfo(){}
    STEffectsInfo(int t, const QString& n, const ZPMincontext& z)
        :type(t),name(n),zoompan(z){}
    int type;
    QString name;
    ZPMincontext zoompan;
    //QByteArray duration;
}STEffectsInfo;
int testZoomPan(int argc, char *argv[])
{
#if 0
    {
    QMap<QString /*id*/, STEffectsInfo > allEffects;
    //allEffects.insert("Trans013", STEffectsInfo(MMET_MOTION_FFMPEG, "CrossZoom", STZoompanEle("1.4", {-1}, {-1}),STZoompanEle("(iw-mw)/2", {-1}, {-1}),STZoompanEle("max((zoom- 1)*(1-on/%1)*ih,0", {OFFSET(allframes)}, {AV_OPT_TYPE_INT}) ));
    allEffects.insert("Trans013", STEffectsInfo(MMET_MOTION_FFMPEG, "CrossZoom", {{"1.4", {-1}, {-1}},{"(iw-mw)/2", {-1}, {-1}},{"max((zoom- 1)*(1-on/%1)*ih,0", {OFFSET(allframes)}, {AV_OPT_TYPE_INT}}} ));
    QString id="Trans013";
    QMap<QString /*id*/, STEffectsInfo >::iterator iter=allEffects.find(id);
    if (iter != allEffects.end() && iter.key() == id)
    {
        QVector<STZoompanEle>& elements=iter.value().elements;
    }
    }
#endif
    //global
    QMap<QString /*id*/, STEffectsInfo > allEffects;
    allEffects.insert("Trans050", STEffectsInfo(MMET_MOTION_FFMPEG, "panupleft"
                , ZPMincontext("1.4","0","max((zoom-1)*(1-on/duration)*ih,0)", 0, 0, 0, 0, "0", "(zoom-1)*ih" )));
    allEffects.insert("Trans051", STEffectsInfo(MMET_MOTION_FFMPEG, "panup"
                , ZPMincontext("1.4","(iw-mw)/2","max((zoom- 1)*(1-on/duration)*ih,0)", 0, 0, 1, 0, "0", "(zoom-1)*ih" )));
    allEffects.insert("Trans052", STEffectsInfo(MMET_MOTION_FFMPEG, "panupright"
                , ZPMincontext("1.4","(zoom-1)*iw","max((zoom-1)*(1-on/duration)*ih,0)", 0, 0, 0, 0, "0", "(zoom-1)*ih" )));
    allEffects.insert("Trans053", STEffectsInfo(MMET_MOTION_FFMPEG, "pandownleft"
                , ZPMincontext("1.4","0","min(on*(zoom-1)*ih/duration,(zoom-1)*ih/2)", 0, 0, 0, 0, "0", "0" )));
    allEffects.insert("Trans054", STEffectsInfo(MMET_MOTION_FFMPEG, "pandown"
                , ZPMincontext("1.4","(iw-mw)/2","min(on*(zoom-1)*ih/duration,(zoom-1)*ih)", 0, 0, 1, 0, "0", "0" )));
    allEffects.insert("Trans055", STEffectsInfo(MMET_MOTION_FFMPEG, "pandownright"
                , ZPMincontext("1.4","(zoom-1)*iw","min(on*(zoom-1)*ih/duration,(zoom-1)*ih/2)", 0, 0, 0, 0, "0", "0" )));
    allEffects.insert("Trans056", STEffectsInfo(MMET_MOTION_FFMPEG, "panrighttop"
                , ZPMincontext("1.4","min(on*(zoom-1)*iw/duration,iw)","0", 0, 0, 0, 0, "0", "0" )));
    allEffects.insert("Trans057", STEffectsInfo(MMET_MOTION_FFMPEG, "panright"
                , ZPMincontext("1.4","min(on*(zoom-1)*iw/duration,(zoom-1)*iw/2)","(ih-mh)/2", 0, 0, 0, 1, "0", "0" )));
    allEffects.insert("Trans058", STEffectsInfo(MMET_MOTION_FFMPEG, "panrightbottom"
                , ZPMincontext("1.4","min(on*(zoom-1)*iw/duration,(zoom-1)*iw/2)","(zoom-1)*ih/2", 0, 0, 0, 0, "0", "0" )));
    allEffects.insert("Trans059", STEffectsInfo(MMET_MOTION_FFMPEG, "panlefttop"
                , ZPMincontext("1.4","'max(floor((zoom-1)*(1-on/duration)*iw),0)","0", 0, 0, 0, 0, "0.2*iw", "0" )));
    allEffects.insert("Trans061", STEffectsInfo(MMET_MOTION_FFMPEG, "panleft"
                , ZPMincontext("1.4","max(floor((zoom-1)*(1-on/duration)*iw),0)","(zoom-1)", 0, 0, 0, 0, "0.2*iw", "0" )));
    allEffects.insert("Trans062", STEffectsInfo(MMET_MOTION_FFMPEG, "panleftbottom"
                , ZPMincontext("1.4","max(floor((zoom-1)*(1-on/duration)*iw),0)","(zoom-1)*ih/2", 0, 0, 0, 0, "0.2*iw", "0" )));
    allEffects.insert("Trans063", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomincenter"
                , ZPMincontext("1+0.01*on","(iw-mw)/2","(ih-mh)/2", 0, 0, 1, 1, "0", "0" )));
    allEffects.insert("Trans064", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomintopleft"
                , ZPMincontext("1+0.01*on","(iw-mw)/2","0", 0, 0, 1, 0, "0", "0" )));
    allEffects.insert("Trans065", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomintopright"
                , ZPMincontext("1+0.01*on","iw-mw","0", 1, 0, 0, 0, "0", "0" )));
    allEffects.insert("Trans066", STEffectsInfo(MMET_MOTION_FFMPEG, "zoominright"
                , ZPMincontext("1+0.01*on","(iw-mw)/2","0", 0, 0, 1, 0, "0", "0" )));
    allEffects.insert("Trans067", STEffectsInfo(MMET_MOTION_FFMPEG, "zoominbottomright"
                , ZPMincontext("1+0.01*on","iw-mw","ih-mh", 1, 1, 0, 0, "0", "0" )));
    allEffects.insert("Trans068", STEffectsInfo(MMET_MOTION_FFMPEG, "zoominbottom"
                , ZPMincontext("1+0.01*on","(iw-mw)/2","ih-mh", 0, 1, 1, 0, "0", "0" )));
    allEffects.insert("Trans069", STEffectsInfo(MMET_MOTION_FFMPEG, "zoominbottomleft"
                , ZPMincontext("1+0.01*on","0","ih-mh", 0, 1, 0, 0, "0", "0" )));
    allEffects.insert("Trans070", STEffectsInfo(MMET_MOTION_FFMPEG, "zoominleft"
                , ZPMincontext("1+0.01*on","0","(ih-mh)/2", 0, 0, 0, 1, "0", "0" )));
    allEffects.insert("Trans071", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutcenter"
                , ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","(iw-mw)/2","(ih-mh)/2", 0, 0, 1, 1, "0", "0" )));
    allEffects.insert("Trans072", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomouttopleft"
                , ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","0","0", 0, 0, 0, 0, "0", "0" )));
    allEffects.insert("Trans073", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomouttop"
                , ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","(iw-mw)/2","0", 0, 0, 1, 0, "0", "0" )));
    allEffects.insert("Trans074", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutright"
                , ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","iw-mw","(ih-mh)/2", 1, 0, 0, 1, "0", "0" )));
    allEffects.insert("Trans075", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutbottomright"
                , ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","iw-mw","ih-mh", 1, 1, 0, 0, "0", "0" )));
    allEffects.insert("Trans076", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutbottom"
                , ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","(iw-mw)/2","ih-mh", 0, 1, 1, 0, "0", "0" )));
    allEffects.insert("Trans077", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutbottomleft"
                , ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","0","ih-mh", 0, 1, 0, 0, "0", "0" )));
    allEffects.insert("Trans078", STEffectsInfo(MMET_MOTION_FFMPEG, "zoomoutleft"
                , ZPMincontext("if(lte(on,1),1.4,max(1.001,1.4-0.01*on))","0","(ih-mh)/2", 0, 0, 0, 1, "0", "0" )));

    //class effect
    ZPContext zc, *s=&zc;
    ZPMincontext* zoompan=NULL;
    QByteArray duration;
    //构造函数
    memset(s, 0, sizeof(ZPcontext));

    //seteffectid()
    QString id="Trans051";
    QMap<QString /*id*/, STEffectsInfo >::iterator iter=allEffects.find(id);
    if (iter != allEffects.end() && iter.key() == id)
    {
        zoompan = &iter.value().zoompan;
        s->initial(*zoompan);
    }

    //getframe()
    if(!zoompan)
        return -1;
    AVFrame *overlay_frame = NULL;
    AVCodecContext *overlay_ctx=NULL;
    AVRational framerate;
    int64_t frame_count=10;

    av_log(NULL, AV_LOG_INFO, "testZoomPan\n");
    if (argc != 3) {
        fprintf(stderr, "Usage: %s jpgfile\n", argv[0]);
        exit(1);
    }
    av_register_all();

    image_to_avframe(argv[2], fmt_ctx, overlay_ctx, overlay_frame);
    //overlay_frame=convertFormat(overlay_frame, AV_PIX_FMT_YUVA420P);
    overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_YUVA420P);
    //overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_RGBA);

    framerate.num=24;
    framerate.den=1;
#if 0
    //zoompan_init(s, "zoom+0.01", overlay_frame->width, overlay_frame->height, "0", "0", QString::number(frame_count).toStdString().c_str());
    memset(s, 0, sizeof(ZPcontext));
    s->zoom_expr_str="1.4";//"zoom+0.01";//"1.4";
    s->x_expr_str="(iw-mw)/2";//"0";//"(iw-mw)/2";
    //double yChangeVar=1;
    //int allframes=25;
    QByteArray arry;
    //arry.append(QString("max((zoom- 1)*(1-on/%1)*ih,0)").arg(allframes));
    arry.append(QString("max((zoom- 1)*(1-on/duration)*ih,0)"));
    s->y_expr_str=arry.data();//"0"
    s->mwhx=1;
    s->ox_expr_str="0";
    s->oy_expr_str="(zoom-1)*ih";//"0";//"(zoom-1)*ih";
    //s->prev_zoom = 1;

    QByteArray arrd;
    arrd.append(QString::number(frame_count));
    s->duration_expr_str=arrd.data();
    s->w=overlay_frame->width;
    s->h=overlay_frame->height;
#endif
    duration.clear();duration.append(QString::number(frame_count));
    s->duration_expr_str=duration.data();
    s->w=overlay_frame->width;
    s->h=overlay_frame->height;
    zoompan_filter_frame(s, overlay_frame->format);

    for(int i=1; i<=frame_count+5; i++)
    {
        //AVFrame* pFrame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_RGBA);
        AVFrame* pFrame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_YUVA420P);
        zoompan_request_frame(s, overlay_frame, i, pFrame);
        displayFrame(pFrame);
        av_frame_free(&pFrame);
    }
    for(int i=1; i<=frame_count+5; i++)
    {
        AVFrame* pFrame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_RGBA);
        zoompan_request_frame(s, overlay_frame, i, pFrame);
        displayFrame(pFrame);
        av_frame_free(&pFrame);
    }
    //av_frame_free(&overlay_frame); //error uncomplete
////    sws_freeContext(overlay_ctx);
    return 0;
}

AVFrame* copy_frame(AVFrame* in, int format, int width, int height)
{
    AVFrame* frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    memset(frame, 0, sizeof(AVFrame));
    frame->format = format;
    frame->width  = width;
    frame->height = height;
    int ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }
    ret = av_frame_copy_props(frame, in);
    if (ret < 0) {
        av_frame_unref(frame);
        fprintf(stderr, "Could not av_frame_copy_props\n");
        exit(1);
    }
    return frame;
}
//-----------------------------------------
//	作用:		把线形存储的像素转化为二维数组形式
//	参数:		
//		image	线形存储的象素
//		width	图像的宽度
//		height	图象的高度
//-----------------------------------------
BYTE** CreatImage(BYTE* image, unsigned int width, unsigned int height)
{
	BYTE** imageBuf = (BYTE**)malloc(sizeof(BYTE*) * (height));
	for(unsigned int y = 0; y < height; y++)
	{
		//使imageBuf中每个指针分别指向其下标表示的行的行首地址
		imageBuf[y] = image + y * width * 4;
	}

	return imageBuf;
}
//-----------------------------------------
//模板的卷积运算
//-----------------------------------------
int TempltExcute(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y)
{
	int i,j;
	int m=0,px,py;
	//分别对模板上每个位置进行计算
	for(i=0; i<tw; i++)
    {
		for(j=0; j<tw; j++)
		{
			//计算像素在原图像上的位置
			py=y-tw/2+i;
			px=x-tw/2+j;
			//将每个像素的灰度乘以权值再相加
			m+=GetAsh(imageBuf0,px,py) * templt[i*tw+j];
		}
    }
    return m;
}


//-----------------------------------------
// 用a表示的灰度填充指定像素
//-----------------------------------------
void SetPixel2(BYTE** imageBuf1, int x, int y, int a)
{
	imageBuf1[y][x*4] = a;
	imageBuf1[y][x*4+1] = a;
	imageBuf1[y][x*4+2] = a;
	imageBuf1[y][x*4+3]= 255;
}

int GetAsh(BYTE** imageBuf, int x, int y)
{
    int clr;
    clr = (imageBuf[y][x*4] + imageBuf[y][x*4+1]
            +imageBuf[y][x*4+2]) / 3;
    return clr;
}
int testTransparentBGSubtitles(int argc, char *argv[])
{
    AVFrame *avframe = NULL;
    AVCodecContext *overlay_ctx=NULL;
    AssContext cc, *s=&cc;
    int64_t frame_count=100;

    av_log(NULL, AV_LOG_INFO, "testTransparentBGSubtitles\n");
    av_register_all();

    int neww=1280;
    int newh=720;

    QImage m_blackImage(neww, newh, QImage::Format_RGBA8888);
	//m_blackImage->fill(QColor("#000000"));
	//m_blackImage.fill(QColor(0, 0, 0, 20));
	m_blackImage.fill(QColor(0, 0, 0, 0));
    //m_blackImage.save("transparent.png");
    //return 0;
    avframe = av_frame_alloc();
    if (!avframe) 
    {
        qInfo()<<"SubtitleReader::GetFrame error av_frame_alloc";
        return -1;
    }
    memset(avframe, 0, sizeof(AVFrame));
    avframe->format = AV_PIX_FMT_RGBA;
    avframe->width  = neww;
    avframe->height = newh;
    int ret = av_frame_get_buffer(avframe, 32);
    if (ret < 0) {
        qInfo()<<"SubtitleReader::GetFrame error av_frame_get_buffer";
        //goto END_GetFrame;
        return -1;
    }
    //frame
	//unsigned char* qbuffer = new unsigned char[buffer_size]();
    //av_image_fill_arrays(avframe->data, avframe->linesize, m_qbuffer, AV_PIX_FMT_RGBA, neww, newh, 1);
    av_image_fill_arrays(avframe->data, avframe->linesize, m_blackImage.bits(), AV_PIX_FMT_RGBA, neww, newh, 1);
    displayFrame(avframe);

    ret=subtitles_init_subtitles(s, "", "subtitle.srt",0, 0);
    //subtitles_config_input(s, AV_PIX_FMT_YUV420P, neww, newh );
    subtitles_config_input(s, AV_PIX_FMT_RGBA, neww, newh );
    subtitles_filter_frame(s, avframe, 1, 24, 1);
    displayFrame(avframe);

    return 0;
}
int testSubtitles(int argc, char *argv[])
{
    AVFrame *overlay_frame = NULL;
    AVCodecContext *overlay_ctx=NULL;
    AssContext cc, *s=&cc;
    int64_t frame_count=100;

    av_log(NULL, AV_LOG_INFO, "ColorChannelMixer\n");
    if (argc != 3) {
        fprintf(stderr, "Usage: %s any jpgfile\n", argv[0]);
        exit(1);
    }
    av_register_all();

    image_to_avframe(argv[2], fmt_ctx, overlay_ctx, overlay_frame);
    //overlay_frame=convertFormat(overlay_frame, AV_PIX_FMT_YUVA420P);
    overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_YUV420P);
    //overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_RGBA);
    displayFrame(overlay_frame);

    int w=1280;
    int h=720;
    int ret=subtitles_init_subtitles(s, "", "subtitle.srt",0, 0);
    subtitles_config_input(s, AV_PIX_FMT_YUV420P, w, h );
    subtitles_filter_frame(s, overlay_frame, 1, 24, 1);
    displayFrame(overlay_frame);

    return 0;
}

int testScale(int argc, char *argv[])
{
    AVFrame *overlay_frame = NULL;
    AVCodecContext *overlay_ctx=NULL;
    double time, alltime;

    av_log(NULL, AV_LOG_INFO, "testScale\n");
    if (argc != 3) {
        fprintf(stderr, "Usage: %s any jpgfile\n", argv[0]);
        exit(1);
    }
    av_register_all();

    image_to_avframe(argv[2], fmt_ctx, overlay_ctx, overlay_frame);
    //overlay_frame=convertFormat(overlay_frame, AV_PIX_FMT_YUVA420P);
    //overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_YUV420P);
    overlay_frame=convertFormat(overlay_frame, overlay_frame->width, overlay_frame->height, AV_PIX_FMT_RGBA);
    //displayFrame(overlay_frame);

    uint8_t *dst_data[4];
    int dst_linesize[4];
    int src_w = overlay_frame->width, src_h = overlay_frame->height;
    //int dst_w=src_w, dst_h=src_h;
    int dst_w=1280, dst_h=720;
    enum AVPixelFormat src_pix_fmt = (AVPixelFormat)overlay_frame->format, dst_pix_fmt = AV_PIX_FMT_RGBA;
    int dst_bufsize;
    struct SwsContext *sws_ctx;
    int ret;
    int align=1;

    alltime = av_gettime_relative() / 1000.0;
    /* create scaling context */
    sws_ctx = sws_getContext(src_w, src_h, src_pix_fmt,
                             dst_w, dst_h, dst_pix_fmt,
                             SWS_BICUBLIN, NULL, NULL, NULL);
    if (!sws_ctx) {
        fprintf(stderr,
                "Impossible to create scale context for the conversion "
                "fmt:%s s:%dx%d -> fmt:%s s:%dx%d\n",
                av_get_pix_fmt_name(src_pix_fmt), src_w, src_h,
                av_get_pix_fmt_name(dst_pix_fmt), dst_w, dst_h);
        ret = AVERROR(EINVAL);
        goto end;
    }
    /* buffer is going to be written to rawvideo file, no alignment */
    if ((ret = av_image_alloc(dst_data, dst_linesize,
                              dst_w, dst_h, dst_pix_fmt, align)) < 0) {
        fprintf(stderr, "Could not allocate destination image\n");
        goto end;
    }
    dst_bufsize = ret;

    time = av_gettime_relative() / 1000.0;
    /* convert to destination format */

    //omp_set_num_threads(OPEN_MP_NUM_PROCESSORS);
    //omp_set_nested(true);
    //#pragma omp parallel
    {
    sws_scale(sws_ctx, (const uint8_t * const*)overlay_frame->data,
            overlay_frame->linesize, 0, src_h, dst_data, dst_linesize);
    }
    av_log(NULL, AV_LOG_INFO, "sws_scale waste_time: %f all_waste_time: %f dst_bufsize: %d\n"
            , av_gettime_relative() / 1000.0-time,av_gettime_relative() / 1000.0-alltime, dst_bufsize );

    //buffer -> avframe 
    //buffer -> qimage
end:
    //av_buffer_unref(&frame->buf[0]);
    av_freep(&dst_data[0]);
    sws_freeContext(sws_ctx);
    return ret < 0;


    //av_image_copy
}
