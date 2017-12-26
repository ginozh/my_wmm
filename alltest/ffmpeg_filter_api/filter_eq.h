#ifndef FILTER_EQ_H
#define FILTER_EQ_H
extern "C" {
#include "libavfilter/internal.h"
#include "libavutil/common.h"
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
#include "libavutil/pixdesc.h"
#include "libavutil/eval.h"
}
enum eq_var_name {
    EQ_VAR_N,
    EQ_VAR_POS,
    EQ_VAR_R,
    EQ_VAR_T,
    EQ_VAR_NB
};

typedef struct EQParameters {
    void (*adjust)(struct EQParameters *eq, uint8_t *dst, int dst_stride,
                   const uint8_t *src, int src_stride, int w, int h);

    uint8_t lut[256];

    double brightness, contrast, gamma, gamma_weight;
    int lut_clean;

} EQParameters;

typedef struct {
    const AVClass *class1;

    EQParameters param[3];

    char   *contrast_expr;
    AVExpr *contrast_pexpr;
    double  contrast;

    char   *brightness_expr;
    AVExpr *brightness_pexpr;
    double  brightness;

    char   *saturation_expr;
    AVExpr *saturation_pexpr;
    double  saturation;

    char   *gamma_expr;
    AVExpr *gamma_pexpr;
    double  gamma;

    char   *gamma_weight_expr;
    AVExpr *gamma_weight_pexpr;
    double  gamma_weight;

    char   *gamma_r_expr;
    AVExpr *gamma_r_pexpr;
    double  gamma_r;

    char   *gamma_g_expr;
    AVExpr *gamma_g_pexpr;
    double  gamma_g;

    char   *gamma_b_expr;
    AVExpr *gamma_b_pexpr;
    double  gamma_b;

    double var_values[EQ_VAR_NB];

    void (*process)(struct EQParameters *par, uint8_t *dst, int dst_stride,
                    const uint8_t *src, int src_stride, int w, int h);

    enum EvalMode { EVAL_MODE_INIT, EVAL_MODE_FRAME, EVAL_MODE_NB } eval_mode;
} EQContext;
int eq_initialize(EQContext *eq, double brightness, double contrast, double  saturation);
int eq_filter_frame(EQContext *eq, AVFrame *in, int frame_count_out, AVFrame* &out);
void eq_uninit(EQContext *eq);

//void ff_eq_init_x86(EQContext *eq);
#endif
