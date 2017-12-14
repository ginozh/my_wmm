#ifndef FILTER_BLEND_H
#define FILTER_BLEND_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavfilter/blend.h>
}
typedef struct BlendContext {
    const AVClass *class1;
    ////FFDualInputContext dinput;
    int hsub, vsub;             ///< chroma subsampling values
    int nb_planes;
    const char *all_expr;
    enum BlendMode all_mode;
    double all_opacity;

    FilterParams params[4];
    int tblend;
    AVFrame *prev_frame;        /* only used with tblend */
} BlendContext;

av_cold int blend_init(BlendContext *s, AVFrame* top, AVFrame* bottom, const char* all_expr);
AVFrame *blend_frame(BlendContext *s, AVFrame *top_buf,
                            const AVFrame *bottom_buf, AVFrame *&dst_buf, 
                            int64_t frame_count_out , double pts);
av_cold void blend_uninit(BlendContext *s);
#endif
