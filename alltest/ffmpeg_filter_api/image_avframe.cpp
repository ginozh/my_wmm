#include "image_avframe.h"

static int open_codec_context(int *stream_idx,
                              AVCodecContext **dec_ctx, AVFormatContext *fmt_ctx, enum AVMediaType type)
{
    int ret, stream_index;
    AVStream *st;
    AVCodec *dec = NULL;
    AVDictionary *opts = NULL;

    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream\n",
                av_get_media_type_string(type));
        return ret;
    } else {
        stream_index = ret;
        st = fmt_ctx->streams[stream_index];

        /* find decoder for the stream */
        dec = avcodec_find_decoder(st->codecpar->codec_id);
        if (!dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }

        /* Allocate a codec context for the decoder */
        *dec_ctx = avcodec_alloc_context3(dec);
        if (!*dec_ctx) {
            fprintf(stderr, "Failed to allocate the %s codec context\n",
                    av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }

        /* Copy codec parameters from input stream to output codec context */
        if ((ret = avcodec_parameters_to_context(*dec_ctx, st->codecpar)) < 0) {
            fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
                    av_get_media_type_string(type));
            return ret;
        }

        /* Init the decoders, with or without reference counting */
        //av_dict_set(&opts, "refcounted_frames", refcount ? "1" : "0", 0);
        av_dict_set(&opts, "refcounted_frames", "1" , 0);
        if ((ret = avcodec_open2(*dec_ctx, dec, &opts)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
        *stream_idx = stream_index;
    }

    return 0;
}
static AVFormatContext *fmt_ctx = NULL;
static int video_stream_idx = -1;//, audio_stream_idx = -1;
//static AVCodecContext *video_dec_ctx = NULL;//, *audio_dec_ctx;
static AVStream *video_stream = NULL;//, *audio_stream = NULL;
int image_to_avframe(const char *src_filename, AVCodecContext *&video_dec_ctx, AVFrame * &frame)
{
    //int err, i;
    ///AVFrame *frame = NULL;
    AVPacket pkt;
    int ret = 0, got_frame;

    int width, height;
    enum AVPixelFormat pix_fmt;

    /* register all formats and codecs */
    av_register_all();
#if 0
    fmt_ctx = avformat_alloc_context();
    if (!fmt_ctx) {
        av_log(NULL, AV_LOG_FATAL, "Could not allocate context.\n");
        ret = AVERROR(ENOMEM);
        goto fail;
    }
#if 0
    //设置是否中断decode的回调, 如果没有的话,不需要avformat_alloc_context, avformat_open_input会包括
    fmt_ctx->interrupt_callback.callback = decode_interrupt_cb;
    fmt_ctx->interrupt_callback.opaque = is->global_video_state;
#endif

#endif
    /* open input file, and allocate format context */
    if (avformat_open_input(&fmt_ctx, src_filename, NULL, NULL) < 0) {
        av_log(NULL, AV_LOG_FATAL, "Could not open source file %s\n", src_filename);
        ret = -1;
        goto fail;
    }

    /* retrieve stream information */
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        av_log(NULL, AV_LOG_FATAL, "Could not find stream information\n");
        ret = -2;
        goto fail;
    }

    if (open_codec_context(&video_stream_idx, &video_dec_ctx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
        video_stream = fmt_ctx->streams[video_stream_idx];

        width = video_dec_ctx->width;
        height = video_dec_ctx->height;
        pix_fmt = video_dec_ctx->pix_fmt;
    }
    if (!video_stream) {
        av_log(NULL, AV_LOG_FATAL, "Could not find audio or video stream in the input, aborting\n");
        ret = -3;
        goto fail;
    }

    frame = av_frame_alloc();
    if (!frame) {
        av_log(NULL, AV_LOG_FATAL, "Could not allocate frame\n");
        ret = AVERROR(ENOMEM);
        goto fail;
    }

    /* initialize packet, set data to NULL, let the demuxer fill it */
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    /* read frames from the file */
    while (av_read_frame(fmt_ctx, &pkt) >= 0) {
        if (pkt.stream_index == video_stream_idx) {
            /* decode video frame */
            ret = avcodec_decode_video2(video_dec_ctx, frame, &got_frame, &pkt);
            if (ret < 0) {
                //av_log(NULL, AV_LOG_FATAL, "Error decoding video frame (%s)\n", av_err2str(ret));
                av_log(NULL, AV_LOG_FATAL, "Error decoding video frame %d\n", ret);
                goto fail;
            }
            if (got_frame) {
                if (frame->width != width || frame->height != height ||
                        frame->format != pix_fmt) {
                    /* To handle this change, one could call av_image_alloc again and
                     * decode the following frames into another rawvideo file. */
                    av_log(NULL, AV_LOG_FATAL, "Error: Width, height and pixel format have to be "
                            "constant in a rawvideo file, but the width, height or "
                            "pixel format of the input video changed:\n"
                            "old: width = %d, height = %d, format = %s\n"
                            "new: width = %d, height = %d, format = %s\n",
                            width, height, av_get_pix_fmt_name(pix_fmt),
                            frame->width, frame->height,
                            av_get_pix_fmt_name((AVPixelFormat)frame->format));
                    ret = -3;
                    goto fail;
                }
                break;
            }
        }
    }

    return 0;
fail:
    image_avframe_close(video_dec_ctx);
    if(frame)
    {
        av_frame_free(&frame);
    }
    return ret;
}
int image_avframe_close(AVCodecContext *&video_dec_ctx)
{
    if (video_dec_ctx)
    {
        avcodec_free_context(&video_dec_ctx);
        video_dec_ctx=NULL;
    }
    if(fmt_ctx)
    {
        avformat_close_input(&fmt_ctx);
        fmt_ctx=NULL;
    }
    return 0;
}
