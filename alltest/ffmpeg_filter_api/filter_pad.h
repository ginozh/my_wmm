#ifndef FILTER_PAD_H
#define FILTER_PAD_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libavutil/avstring.h"
#include "libavutil/common.h"
#include "libavutil/eval.h"
#include "libavutil/pixdesc.h"
//#include "libavutil/colorspace.h"
#include "libavutil/imgutils.h"
#include "libavutil/parseutils.h"
#include "libavutil/mathematics.h"
#include "libavutil/opt.h"
#include <libavfilter/drawutils.h>
#include <libavutil/time.h>
}

typedef struct PadContext {
    const AVClass *class1;
    int w, h;               ///< output dimensions, a value of 0 will result in the input size
    int x, y;               ///< offsets of the input area with respect to the padded area
    int in_w, in_h;         ///< width and height for the padded input video, which has to be aligned to the chroma values in order to avoid chroma issues
    int inlink_w, inlink_h;

    char *w_expr;           ///< width  expression string
    char *h_expr;           ///< height expression string
    char *x_expr;           ///< width  expression string
    char *y_expr;           ///< height expression string
    uint8_t rgba_color[4];  ///< color for the padding area
    FFDrawContext draw;
    FFDrawColor color;

    int eval_mode;          ///< expression evaluation mode
} PadContext;

void pad_init(PadContext* s, char* w, char*h, char* x, char* y);
int pad_config_input(AVCodecContext *ctx, PadContext *s);
int pad_filter_frame(AVCodecContext *ctx, PadContext* s, AVFrame * &in, AVFrame *&out);
void pad_uninit(PadContext* s);
#endif
