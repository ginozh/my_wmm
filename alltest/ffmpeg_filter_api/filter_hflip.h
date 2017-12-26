#ifndef FILTER_HFLIP_H
#define FILTER_HFLIP_H
extern "C" {
#include "libavutil/opt.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/formats.h"
#include "libavfilter/internal.h"
#include "libavutil/pixdesc.h"
#include "libavutil/internal.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/imgutils.h"
}
#include <string.h>

typedef struct HFlipContext {
    const AVClass *class1;
    int max_step[4];    ///< max pixel step for each plane, expressed as a number of bytes
    int planewidth[4];  ///< width of each plane
    int planeheight[4]; ///< height of each plane
} HFlipContext;
av_cold int hflip_init(HFlipContext *s);
int hflip_config_props(HFlipContext *s, AVFrame* in);
int hflip_filter_frame(HFlipContext *s, AVFrame *in, AVFrame* &out);
#endif
