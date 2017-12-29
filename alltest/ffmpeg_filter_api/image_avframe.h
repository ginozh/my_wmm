#ifndef IMAGE_AVFRAME_H
#define IMAGE_AVFRAME_H
extern "C" {
//#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
#include <libavutil/time.h>
#include <libavutil/pixdesc.h>
}

int image_to_avframe(const char *src_filename, AVFormatContext *&fmt_ctx, AVCodecContext *&video_dec_ctx, AVFrame * &frame);
int image_avframe_close(AVFormatContext *&fmt_ctx, AVCodecContext *&video_dec_ctx);
#endif
