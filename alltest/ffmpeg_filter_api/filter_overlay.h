#ifndef FILTER_OVERLAY_H
#define FILTER_OVERLAY_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavutil/imgutils.h>
#include <libavfilter/drawutils.h>
#include <libavfilter/internal.h>
#include <libavutil/time.h>
#include <libavutil/eval.h>
#include <libavutil/common.h>
}

enum overlay_var_name {
    OVERLAY_VAR_MAIN_W,    OVERLAY_VAR_MW,
    OVERLAY_VAR_MAIN_H,    OVERLAY_VAR_MH,
    OVERLAY_VAR_OVERLAY_W, OVERLAY_VAR_OW,
    OVERLAY_VAR_OVERLAY_H, OVERLAY_VAR_OH,
    OVERLAY_VAR_HSUB,
    OVERLAY_VAR_VSUB,
    OVERLAY_VAR_X,
    OVERLAY_VAR_Y,
    OVERLAY_VAR_N,
    OVERLAY_VAR_POS,
    OVERLAY_VAR_T,
    OVERLAY_VAR_VARS_NB
};

typedef struct OverlayContext {
    const AVClass *class1;
    int x, y;                   ///< position of overlaid picture

    int allow_packed_rgb;
    uint8_t main_is_packed_rgb;
    uint8_t main_rgba_map[4];
    uint8_t main_has_alpha;
    uint8_t overlay_is_packed_rgb;
    uint8_t overlay_rgba_map[4];
    uint8_t overlay_has_alpha;
    int format;                 ///< OverlayFormat
    int eval_mode;              ///< EvalMode

    ////FFDualInputContext dinput;

    int main_pix_step[4];       ///< steps per pixel for each plane of the main output
    int overlay_pix_step[4];    ///< steps per pixel for each plane of the overlay
    int hsub, vsub;             ///< chroma subsampling values
    const AVPixFmtDescriptor *main_desc; ///< format descriptor for main input

    double var_values[OVERLAY_VAR_VARS_NB];
    char *x_expr, *y_expr;

    int eof_action;             ///< action to take on EOF from source

    AVExpr *x_pexpr, *y_pexpr;

    void (*blend_image)(OverlayContext *s, AVFrame *dst, const AVFrame *src, int x, int y);
} OverlayContext;

av_cold int overlay_init(OverlayContext *s, char* x, char* y);
int overlay_config_input_main(AVCodecContext *c, OverlayContext *s);
int overlay_config_input_overlay(AVCodecContext *ctx, AVFrame *overlay_frame, OverlayContext* s);
AVFrame *overlay_blend(AVCodecContext *ctx, OverlayContext* s, AVFrame *mainpic,
                         const AVFrame *second, int64_t frame_count_out);
av_cold void overlay_uninit(OverlayContext *s);
#endif
