#ifndef FILTER_ROTATE_H
#define FILTER_ROTATE_H
extern "C" {
#include "libavutil/avstring.h"
#include "libavutil/eval.h"
#include "libavutil/opt.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/parseutils.h"
#include "libavutil/pixdesc.h"

#include "libavfilter/avfilter.h"
#include "libavfilter/drawutils.h"
#include "libavfilter/internal.h"
}
#include <float.h>
enum rotate_var_name {
    ROTATE_VAR_IN_W , ROTATE_VAR_IW,
    ROTATE_VAR_IN_H , ROTATE_VAR_IH,
    ROTATE_VAR_OUT_W, ROTATE_VAR_OW,
    ROTATE_VAR_OUT_H, ROTATE_VAR_OH,
    ROTATE_VAR_HSUB, ROTATE_VAR_VSUB,
    ROTATE_VAR_N,
    ROTATE_VAR_T,
    ROTATE_VAR_VARS_NB
};

typedef struct RotContext {
    const AVClass *class1;
    double angle;
    char *angle_expr_str;   ///< expression for the angle
    AVExpr *angle_expr;     ///< parsed expression for the angle
    char *outw_expr_str, *outh_expr_str;
    int outh, outw;
    uint8_t fillcolor[4];   ///< color expressed either in YUVA or RGBA colorspace for the padding area
    char *fillcolor_str;
    int fillcolor_enable;
    int hsub, vsub;
    int nb_planes;
    int use_bilinear;
    float sinx, cosx;
    double var_values[ROTATE_VAR_VARS_NB];
    FFDrawContext draw;
    FFDrawColor color;
    uint8_t *(*interpolate_bilinear)(uint8_t *dst_color,
                                    const uint8_t *src, int src_linesize, int src_linestep,
                                    int x, int y, int max_x, int max_y);
} RotContext;
av_cold int rotate_init(RotContext* rot, char* angle, char* outw, char* outh, char* color);
int config_props(RotContext *rot, AVFrame *in);
int rotate_filter_frame(RotContext *rot, AVFrame *in,int frame_count_out, AVFrame* &out);
av_cold void rotate_uninit(RotContext *rot);
#endif
