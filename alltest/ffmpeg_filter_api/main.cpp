#include <QCoreApplication>
#include <QtCore>
#include <QDebug>
#include <QString>
#include <QObject> 
#include <QFontMetrics> 
#include <QFont> 
#include <cstddef>     /* offsetof */
#include <QVector>
#include <QtMath>
#include <iostream>
#include <tr1/memory>

#include <QImage>
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

static AVFormatContext *fmt_ctx;
static AVCodecContext *dec_ctx;
AVFilterContext *buffersink_ctx;
AVFilterContext *buffersrc_ctx;
AVFilterGraph *filter_graph;
static int video_stream_index = -1;
///static int64_t last_pts = AV_NOPTS_VALUE;
static int64_t sws_flags = SWS_BILINEAR;//SWS_BICUBIC;

static int open_input_file(const char *filename);
//w:320 h:240 fmt:yuvj444p sar:1/1 -> w:320 h:240 fmt:yuva420p sar:1/1 flags:0x2
AVFrame* convertFormat(AVFrame *frame, int format);

void displayFrame(AVFrame *frame);

int testVideoFilter(int argc, char *argv[]);

int testZoomPan(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    av_log(NULL, AV_LOG_INFO, "main\n");

    ////testVideoFilter(argc, argv);

    testZoomPan(argc, argv);
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

AVFrame* convertFormat(AVFrame *frame, int format)
{
    AVFrame *pFrame=NULL;
    uint8_t *out_buffer=NULL; //解码后的rgb数据
    struct SwsContext *img_convert_ctx;  //用于解码后的视频格式转换

    pFrame = av_frame_alloc();
    memset(pFrame, 0, sizeof(AVFrame));
    //
    img_convert_ctx = sws_getContext(frame->width, frame->height,
            //pCodecCtx->pix_fmt, frame->width, frame->height,
            (AVPixelFormat)frame->format, frame->width, frame->height,
            (AVPixelFormat)format, sws_flags, NULL, NULL, NULL);

    int numBytes = avpicture_get_size((AVPixelFormat)format, frame->width,frame->height);

    out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    qDebug()<<"convertFormat new out_buffer: "<<out_buffer<<" size: "<<(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrame, out_buffer, (AVPixelFormat)format,
            frame->width, frame->height);

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

    pFrame->format = format;
    pFrame->width  = frame->width;
    pFrame->height = frame->height;
    int ret = av_frame_copy_props(pFrame, frame);
    if (ret < 0) {
        av_frame_unref(pFrame);
        fprintf(stderr, "Could not av_frame_copy_props\n");
        exit(1);
    }

    ////frame = pFrame;
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
    tmpImg.save(QString("images/%1.jpg").arg(idx));

    qDebug()<<"displayFrame free out_buffer: "<<out_buffer;
    av_free(out_buffer); 
    qDebug()<<"displayFrame free pFrame: "<<pFrame;
    av_frame_free(&pFrame);
    ////av_free(pFrame);
    sws_freeContext(img_convert_ctx);
}

int testVideoFilter(int argc, char *argv[])
{
    av_log(NULL, AV_LOG_INFO, "testVideoFilter\n");
    int ret;
    AVPacket packet;
    AVFrame *frame = av_frame_alloc();
    AVFrame *filt_frame = av_frame_alloc();

#if 1
    //overlay
    AVFrame *overlay_frame = NULL;
    AVCodecContext *overlay_ctx=NULL;
    OverlayContext oc, *s=&oc;
#endif
#if 0
    //crop
    CropContext cc, *s=&cc;
    char w[]="256";//"640";
    char h[]="192";//"360";
#endif
#if 0
    //pad
    PadContext pc, *s=&pc;
    char w[]="1024";
    char h[]="786";
#endif
    char x[]="0";
    char y[]="0";

    if (!frame || !filt_frame) {
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
#if 0
    //crop
    crop_init(s, w, h , x, y, 1, 0);
    crop_config_input(dec_ctx, s);
#endif
#if 0
    //pad
    pad_init(s, w, h , x, y);
    pad_config_input(dec_ctx, s);
#endif
#if 1
    //overlay
    image_to_avframe(argv[2], overlay_ctx, overlay_frame);
    overlay_frame=convertFormat(overlay_frame, AV_PIX_FMT_YUVA420P);
    displayFrame(overlay_frame);
    overlay_init(s, x, y);
    overlay_config_input_main(dec_ctx, s);
    overlay_config_input_overlay(dec_ctx, overlay_frame, s);
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

#if 0
    //crop
                    crop_filter_frame(dec_ctx, s, frame);
                    displayFrame(frame);
#endif
#if 0
    //pad
                    AVFrame* out=NULL;
                    pad_filter_frame(dec_ctx, s, frame, out);
                    ///av_log(NULL, AV_LOG_INFO, "out: %d\n", (int)out);
                    displayFrame(out);
#endif
#if 1
    //overlay
                    overlay_blend(dec_ctx, s, frame, overlay_frame, 1);
                    displayFrame(frame);
#endif

                    av_frame_unref(frame);
                }
            }
        }
        av_packet_unref(&packet);
    }
end:
#if 0
    //crop
    crop_uninit(s);
#endif
#if 0
    //pad
    pad_uninit(s);
#endif
#if 1
    //overlay
    image_avframe_close(overlay_ctx);
    overlay_uninit(s);
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

int testZoomPan(int argc, char *argv[])
{
    AVFrame *overlay_frame = NULL;
    AVCodecContext *overlay_ctx=NULL;
    ZPContext zc, *s=&zc;
    AVRational framerate;
    int64_t frame_count=10;

    av_log(NULL, AV_LOG_INFO, "testZoomPan\n");
    if (argc != 3) {
        fprintf(stderr, "Usage: %s jpgfile\n", argv[0]);
        exit(1);
    }
    av_register_all();

    image_to_avframe(argv[2], overlay_ctx, overlay_frame);
    overlay_frame=convertFormat(overlay_frame, AV_PIX_FMT_YUVA420P);

    framerate.num=24;
    framerate.den=1;
    zoompan_init(s, "zoom+0.01", overlay_frame->width, overlay_frame->height, "0", "0", QString::number(frame_count).toStdString().c_str(), framerate);
    zoompan_filter_frame(overlay_ctx, s, overlay_frame, 0, 0);

    for(int i=0; i<frame_count; i++)
    {
        AVFrame* pFrame=convertFormat(overlay_frame, AV_PIX_FMT_YUVA420P);
        request_frame(overlay_ctx, s, i, pFrame);
        displayFrame(pFrame);
        av_frame_free(&pFrame);
    }
    //av_frame_free(&overlay_frame); //error uncomplete
////    sws_freeContext(overlay_ctx);
    return 0;
}
