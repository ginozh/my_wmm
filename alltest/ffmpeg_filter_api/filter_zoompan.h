#ifndef FILTER_ZOOMPAN_H
#define FILTER_ZOOMPAN_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
}

enum zoompan_var_name {
    ZOOMPAN_VAR_IN_W,   ZOOMPAN_VAR_IW,
    ZOOMPAN_VAR_IN_H,   ZOOMPAN_VAR_IH,
    ZOOMPAN_VAR_OUT_W,  ZOOMPAN_VAR_OW,
    ZOOMPAN_VAR_OUT_H,  ZOOMPAN_VAR_OH,
    ZOOMPAN_VAR_MIDDLE_W,  ZOOMPAN_VAR_MW, //storm
    ZOOMPAN_VAR_MIDDLE_H,  ZOOMPAN_VAR_MH, //storm
    ZOOMPAN_VAR_IN,
    ZOOMPAN_VAR_ON,
    ZOOMPAN_VAR_DURATION,
    ZOOMPAN_VAR_PDURATION,
    ZOOMPAN_VAR_TIME,
    ZOOMPAN_VAR_FRAME,
    ZOOMPAN_VAR_ZOOM,
    ZOOMPAN_VAR_PZOOM,
    ZOOMPAN_VAR_X, ZOOMPAN_VAR_PX,
    ZOOMPAN_VAR_Y, ZOOMPAN_VAR_PY,
    ZOOMPAN_VAR_A,
    ZOOMPAN_VAR_SAR,
    ZOOMPAN_VAR_DAR,
    ZOOMPAN_VAR_HSUB,
    ZOOMPAN_VAR_VSUB,
    ZOOMPAN_VARS_NB
};

typedef struct ZPcontext {
    const AVClass *class1;
    const char *zoom_expr_str;
    const char *x_expr_str;
    const char *y_expr_str;
    const char *duration_expr_str;
    int w, h; //out width, height
    double x, y;
    //double ox, oy;
    int64_t mwx, mhy, mwhx, mhhy;
    const char *ox_expr_str; //storm
    const char *oy_expr_str;
    double prev_zoom;
    int prev_nb_frames;
    struct SwsContext *sws;
    int64_t frame_count;
    const AVPixFmtDescriptor *desc;
    AVFrame *in;
    double var_values[ZOOMPAN_VARS_NB];
    int nb_frames;
    int current_frame;
    int finished;
    AVRational framerate;
} ZPContext;
av_cold int zoompan_init(ZPcontext *s, const char* zoom, int w, int h, const char* x, const char* y, const char* duration
        , AVRational framerate);
int zoompan_filter_frame(AVCodecContext *ctx,ZPContext *s, AVFrame *in, int64_t frame_count_in
        , int64_t frame_count_out);
int request_frame(AVCodecContext *ctx,ZPContext *s, int64_t frame_count_in, AVFrame* &out);

#endif
