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

static AVFormatContext *fmt_ctx;
static AVCodecContext *dec_ctx;
AVFilterContext *buffersink_ctx;
AVFilterContext *buffersrc_ctx;
AVFilterGraph *filter_graph;
static int video_stream_index = -1;
///static int64_t last_pts = AV_NOPTS_VALUE;
static int64_t sws_flags = SWS_BILINEAR;//SWS_BICUBIC;

static const char *const var_names[] = {
    "in_w", "iw",   ///< width  of the input video
    "in_h", "ih",   ///< height of the input video
    "out_w", "ow",  ///< width  of the cropped video
    "out_h", "oh",  ///< height of the cropped video
    "a",
    "sar",
    "dar",
    "hsub",
    "vsub",
    "x",
    "y",
    "n",            ///< number of frame
    "pos",          ///< position in the file
    "t",            ///< timestamp expressed in seconds
    NULL
};

enum var_name {
    VAR_IN_W,  VAR_IW,
    VAR_IN_H,  VAR_IH,
    VAR_OUT_W, VAR_OW,
    VAR_OUT_H, VAR_OH,
    VAR_A,
    VAR_SAR,
    VAR_DAR,
    VAR_HSUB,
    VAR_VSUB,
    VAR_X,
    VAR_Y,
    VAR_N,
    VAR_POS,
    VAR_T,
    VAR_VARS_NB
};

typedef struct CropContext {
    const AVClass *class1;
    int  x;             ///< x offset of the non-cropped area with respect to the input area
    int  y;             ///< y offset of the non-cropped area with respect to the input area
    int  w;             ///< width of the cropped area
    int  h;             ///< height of the cropped area

    AVRational out_sar; ///< output sample aspect ratio
    int keep_aspect;    ///< keep display aspect ratio when cropping
    int exact;          ///< exact cropping, for subsampled formats

    int max_step[4];    ///< max pixel step for each plane, expressed as a number of bytes
    int hsub, vsub;     ///< chroma subsampling
    char *x_expr, *y_expr, *w_expr, *h_expr;
    AVExpr *x_pexpr, *y_pexpr;  /* parsed expressions for x and y */
    double var_values[VAR_VARS_NB];
} CropContext;
static inline int normalize_double(int *n, double d)
{
    int ret = 0;

    if (isnan(d)) {
        ret = AVERROR(EINVAL);
    } else if (d > INT_MAX || d < INT_MIN) {
        *n = d > INT_MAX ? INT_MAX : INT_MIN;
        ret = AVERROR(EINVAL);
    } else
        *n = lrint(d);

    return ret;
}
int crop_config_input(AVCodecContext *c, CropContext* s)
{
    const AVPixFmtDescriptor *pix_desc = av_pix_fmt_desc_get(c->pix_fmt);
    int ret;
    const char *expr;
    double res;
    AVRational dar; //storm

    s->var_values[VAR_IN_W]  = s->var_values[VAR_IW] = c->width;//ctx->inputs[0]->w;
    s->var_values[VAR_IN_H]  = s->var_values[VAR_IH] = c->height;//ctx->inputs[0]->h;
    s->var_values[VAR_A]     = (float) c->width / c->height;//(float) link->w / link->h;
    //s->var_values[VAR_SAR]   = link->sample_aspect_ratio.num ? av_q2d(link->sample_aspect_ratio) : 1;
    s->var_values[VAR_SAR]   = c->sample_aspect_ratio.num ? av_q2d(c->sample_aspect_ratio) : 1;
    s->var_values[VAR_DAR]   = s->var_values[VAR_A] * s->var_values[VAR_SAR];
    s->var_values[VAR_HSUB]  = 1<<pix_desc->log2_chroma_w;
    s->var_values[VAR_VSUB]  = 1<<pix_desc->log2_chroma_h;
    s->var_values[VAR_X]     = NAN;
    s->var_values[VAR_Y]     = NAN;
    s->var_values[VAR_OUT_W] = s->var_values[VAR_OW] = NAN;
    s->var_values[VAR_OUT_H] = s->var_values[VAR_OH] = NAN;
    s->var_values[VAR_N]     = 0;
    s->var_values[VAR_T]     = NAN;
    s->var_values[VAR_POS]   = NAN;

    av_image_fill_max_pixsteps(s->max_step, NULL, pix_desc);
    s->hsub = pix_desc->log2_chroma_w;
    s->vsub = pix_desc->log2_chroma_h;

    if ((ret = av_expr_parse_and_eval(&res, (expr = s->w_expr),
                                      var_names, s->var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, c)) < 0)
        goto fail_expr;
    s->var_values[VAR_OUT_W] = s->var_values[VAR_OW] = res;
    if ((ret = av_expr_parse_and_eval(&res, (expr = s->h_expr),
                                      var_names, s->var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, c)) < 0)
        goto fail_expr;
    s->var_values[VAR_OUT_H] = s->var_values[VAR_OH] = res;
    /* evaluate again ow as it may depend on oh */
    if ((ret = av_expr_parse_and_eval(&res, (expr = s->w_expr),
                                      var_names, s->var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, c)) < 0)
        goto fail_expr;

    s->var_values[VAR_OUT_W] = s->var_values[VAR_OW] = res;
    if (normalize_double(&s->w, s->var_values[VAR_OUT_W]) < 0 ||
        normalize_double(&s->h, s->var_values[VAR_OUT_H]) < 0) {
        av_log(c, AV_LOG_ERROR,
               "Too big value or invalid expression for out_w/ow or out_h/oh. "
               "Maybe the expression for out_w:'%s' or for out_h:'%s' is self-referencing.\n",
               s->w_expr, s->h_expr);
        return AVERROR(EINVAL);
    }

    if (!s->exact) {
        s->w &= ~((1 << s->hsub) - 1);
        s->h &= ~((1 << s->vsub) - 1);
    }

    av_expr_free(s->x_pexpr);
    av_expr_free(s->y_pexpr);
    s->x_pexpr = s->y_pexpr = NULL;
    if ((ret = av_expr_parse(&s->x_pexpr, s->x_expr, var_names,
                             NULL, NULL, NULL, NULL, 0, c)) < 0 ||
        (ret = av_expr_parse(&s->y_pexpr, s->y_expr, var_names,
                             NULL, NULL, NULL, NULL, 0, c)) < 0)
        return AVERROR(EINVAL);

    dar = av_mul_q(c->sample_aspect_ratio,
            (AVRational){ c->width, c->height }); //storm
    if (s->keep_aspect) {
        av_reduce(&s->out_sar.num, &s->out_sar.den,
                  dar.num * s->h, dar.den * s->w, INT_MAX);
    } else
        s->out_sar = c->sample_aspect_ratio;

    av_log(c, AV_LOG_INFO, "w:%d h:%d dar:%d/%d sar:%d/%d -> w:%d h:%d sar:%d/%d\n",
           c->width, c->height, dar.num, dar.den,c->sample_aspect_ratio.num, c->sample_aspect_ratio.den,
           s->w, s->h, s->out_sar.num, s->out_sar.den);

    if (s->w <= 0 || s->h <= 0 ||
        s->w > c->width || s->h > c->height) {
        av_log(c, AV_LOG_ERROR,
               "Invalid too big or non positive size for width '%d' or height '%d'\n",
               s->w, s->h);
        return AVERROR(EINVAL);
    }

    /* set default, required in the case the first computed value for x/y is NAN */
    s->x = (c->width - s->w) / 2;
    s->y = (c->height - s->h) / 2;
    if (!s->exact) {
        s->x &= ~((1 << s->hsub) - 1);
        s->y &= ~((1 << s->vsub) - 1);
    }
    return 0;

fail_expr:
    av_log(NULL, AV_LOG_ERROR, "Error when evaluating the expression '%s'\n", expr);
    return ret;
}
void crop_filter_frame(AVCodecContext *c, CropContext* s, AVFrame* frame)
{
    const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(c->pix_fmt);
    int i;
    double time = av_gettime_relative() / 1000.0;

    frame->width  = s->w;
    frame->height = s->h;

    //link -> c;  link->h -> c->height; link->w -> c->width
    s->var_values[VAR_N] = 1;//link->frame_count_out;
    s->var_values[VAR_T] = frame->pts == AV_NOPTS_VALUE ?
        NAN : frame->pts * av_q2d(c->time_base);
    s->var_values[VAR_POS] = av_frame_get_pkt_pos(frame) == -1 ?
        NAN : av_frame_get_pkt_pos(frame);
    s->var_values[VAR_X] = av_expr_eval(s->x_pexpr, s->var_values, NULL);
    s->var_values[VAR_Y] = av_expr_eval(s->y_pexpr, s->var_values, NULL);
    s->var_values[VAR_X] = av_expr_eval(s->x_pexpr, s->var_values, NULL);

    normalize_double(&s->x, s->var_values[VAR_X]);
    normalize_double(&s->y, s->var_values[VAR_Y]);

    if (s->x < 0)
        s->x = 0;
    if (s->y < 0)
        s->y = 0;
    if ((unsigned)s->x + (unsigned)s->w > c->width)
        s->x = c->width - s->w;
    if ((unsigned)s->y + (unsigned)s->h > c->height)
        s->y = c->height - s->h;
    if (!s->exact) {
        s->x &= ~((1 << s->hsub) - 1);
        s->y &= ~((1 << s->vsub) - 1);
    }

    av_log(c, AV_LOG_INFO, "n:%d t:%f pos:%f x:%d y:%d x+w:%d y+h:%d\n",
            (int)s->var_values[VAR_N], s->var_values[VAR_T], s->var_values[VAR_POS],
            s->x, s->y, s->x+s->w, s->y+s->h);

    frame->data[0] += s->y * frame->linesize[0];
    frame->data[0] += s->x * s->max_step[0];

    if (!(desc->flags & AV_PIX_FMT_FLAG_PAL || desc->flags & AV_PIX_FMT_FLAG_PSEUDOPAL)) {
        for (i = 1; i < 3; i ++) {
            if (frame->data[i]) {
                frame->data[i] += (s->y >> s->vsub) * frame->linesize[i];
                frame->data[i] += (s->x * s->max_step[i]) >> s->hsub;
            }
        }
    }

    /* alpha plane */
    if (frame->data[3]) {
        frame->data[3] += s->y * frame->linesize[3];
        frame->data[3] += s->x * s->max_step[3];
    }
    av_log(NULL, AV_LOG_INFO, "crop_filter_frame waste_time: %f\n", av_gettime_relative() / 1000.0-time);
    return;
}
static av_cold void crop_uninit(CropContext* s)
{
    av_expr_free(s->x_pexpr);
    s->x_pexpr = NULL;
    av_expr_free(s->y_pexpr);
    s->y_pexpr = NULL;
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

void displayFrame(AVFrame *frame)
{
#if 1
    AVFrame *pFrameRGB=NULL;
    uint8_t *out_buffer_rgb=NULL; //解码后的rgb数据
#endif
    struct SwsContext *img_convert_ctx;  //用于解码后的视频格式转换
    AVCodecContext *pCodecCtx = dec_ctx;
    static int idx=0;
    idx++;

    pFrameRGB = av_frame_alloc();
    //将解码后的YUV数据转换成RGB32
    img_convert_ctx = sws_getContext(frame->width, frame->height,
            pCodecCtx->pix_fmt, frame->width, frame->height,
            AV_PIX_FMT_RGBA, sws_flags, NULL, NULL, NULL);

    int numBytes = avpicture_get_size(AV_PIX_FMT_RGBA, frame->width,frame->height);

    if(!out_buffer_rgb)
        out_buffer_rgb = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    qDebug()<<"frame2RGBA new out_buffer_rgb: "<<out_buffer_rgb<<" size: "<<(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, out_buffer_rgb, AV_PIX_FMT_RGBA,
            frame->width, frame->height);

    sws_scale(img_convert_ctx,
            (uint8_t const * const *) frame->data,
            frame->linesize, 0, frame->height, pFrameRGB->data,
            pFrameRGB->linesize);
    //把这个RGB数据 用QImage加载
    //QImage tmpImg((uchar *)out_buffer_rgb,frame->width,frame->height,QImage::Format_RGB32);
    ////QImage tmpImg((uchar *)out_buffer_rgb,frame->width,frame->height,QImage::Format_RGB32);
    QImage tmpImg((uchar *)out_buffer_rgb,frame->width,frame->height,QImage::Format_RGBA8888);
    tmpImg.save(QString("images/%1.jpg").arg(idx));

    qDebug()<<"frame2RGBA free out_buffer_rgb: "<<out_buffer_rgb;
    av_free(out_buffer_rgb); 
    qDebug()<<"frame2RGBA free pFrameRGB: "<<pFrameRGB;
    av_frame_free(&pFrameRGB);
    ////av_free(pFrameRGB);
    sws_freeContext(img_convert_ctx);
}
int main(int argc, char *argv[])
{
    int ret;
    AVPacket packet;
    AVFrame *frame = av_frame_alloc();
    AVFrame *filt_frame = av_frame_alloc();

    //crop
    CropContext cc, *s=&cc;
    char w[]="256";//"640";
    char h[]="192";//"360";
    char x[]="0";
    char y[]="0";

    if (!frame || !filt_frame) {
        perror("Could not allocate frame");
        exit(1);
    }
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(1);
    }

    av_register_all();
    avfilter_register_all();

    if ((ret = open_input_file(argv[1])) < 0)
        goto end;

    //1, crop
    {
        memset(s, 0, sizeof(CropContext));
        s->w_expr=w;
        s->h_expr=h;
        s->x_expr=x;
        s->y_expr=y;
        s->keep_aspect=1;
        s->exact=0;
    }
    crop_config_input(dec_ctx, s);

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

                    crop_filter_frame(dec_ctx, s, frame);

                    displayFrame(frame);
                    av_frame_unref(frame);
                }
            }
        }
        av_packet_unref(&packet);
    }
end:
    crop_uninit(s);
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

    exit(0);
    return 0;
}
