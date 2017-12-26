#ifndef FILTER_VFLIP_H
#define FILTER_VFLIP_H
extern "C" {
#include "libavutil/internal.h"
#include "libavutil/opt.h"
#include "libavutil/pixdesc.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/internal.h"
}
typedef struct VFlipContext {
    const AVClass *class1;
    int vsub;   ///< vertical chroma subsampling
} VFlipContext;

av_cold int vflip_init(VFlipContext *s);
int vflip_config(VFlipContext *flip, AVFrame* in);
int vflip_filter_frame(VFlipContext *flip, AVFrame *in, AVFrame* &out);
#endif
