#include "filter_eq.h"
#include <QDebug>
#define qInfo qDebug
static const char *const var_names[] = {
    "n",   // frame count
    "pos", // frame position
    "r",   // frame rate
    "t",   // timestamp expressed in seconds
    NULL
};

static void create_lut(EQParameters *param)
{
    int i;
    double   g  = 1.0 / param->gamma;
    double   lw = 1.0 - param->gamma_weight;

    for (i = 0; i < 256; i++) {
        double v = i / 255.0;
        v = param->contrast * (v - 0.5) + 0.5 + param->brightness;

        if (v <= 0.0) {
            param->lut[i] = 0;
        } else {
            v = v * lw + pow(v, g) * param->gamma_weight;

            if (v >= 1.0)
                param->lut[i] = 255;
            else
                param->lut[i] = 256.0 * v;
        }
    }

    param->lut_clean = 1;
}

static void apply_lut(EQParameters *param, uint8_t *dst, int dst_stride,
                      const uint8_t *src, int src_stride, int w, int h)
{
    int x, y;

    if (!param->lut_clean)
        create_lut(param);

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            dst[y * dst_stride + x] = param->lut[src[y * src_stride + x]];
        }
    }
}

static void process_c(EQParameters *param, uint8_t *dst, int dst_stride,
                      const uint8_t *src, int src_stride, int w, int h)
{
    int x, y, pel;

    int contrast = (int) (param->contrast * 256 * 16);
    int brightness = ((int) (100.0 * param->brightness + 100.0) * 511) / 200 - 128 - contrast / 32;

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            pel = ((src[y * src_stride + x] * contrast) >> 12) + brightness;

            if (pel & ~255)
                pel = (-pel) >> 31;

            dst[y * dst_stride + x] = pel;
        }
    }
}

static void check_values(EQParameters *param, EQContext *eq)
{
    if (param->contrast == 1.0 && param->brightness == 0.0 && param->gamma == 1.0)
        param->adjust = NULL;
    else if (param->gamma == 1.0 && fabs(param->contrast) < 7.9)
        param->adjust = eq->process;
    else
        param->adjust = apply_lut;
}

static void set_contrast(EQContext *eq)
{
    //eq->contrast = av_clipf(av_expr_eval(eq->contrast_pexpr, eq->var_values, eq), -1000.0, 1000.0);
    eq->param[0].contrast = eq->contrast;
    eq->param[0].lut_clean = 0;
    check_values(&eq->param[0], eq);
}

static void set_brightness(EQContext *eq)
{
#if 0
    eq->brightness = av_clipf(av_expr_eval(eq->brightness_pexpr, eq->var_values, eq), -1.0, 1.0);
#endif
    eq->param[0].brightness = eq->brightness;
    eq->param[0].lut_clean = 0;
    check_values(&eq->param[0], eq);
}

static void set_gamma(EQContext *eq)
{
    int i;
#if 0
    eq->gamma        = av_clipf(av_expr_eval(eq->gamma_pexpr,        eq->var_values, eq), 0.1, 10.0);
    eq->gamma_r      = av_clipf(av_expr_eval(eq->gamma_r_pexpr,      eq->var_values, eq), 0.1, 10.0);
    eq->gamma_g      = av_clipf(av_expr_eval(eq->gamma_g_pexpr,      eq->var_values, eq), 0.1, 10.0);
    eq->gamma_b      = av_clipf(av_expr_eval(eq->gamma_b_pexpr,      eq->var_values, eq), 0.1, 10.0);
    eq->gamma_weight = av_clipf(av_expr_eval(eq->gamma_weight_pexpr, eq->var_values, eq), 0.0,  1.0);
#endif
    eq->param[0].gamma = eq->gamma * eq->gamma_g;
    eq->param[1].gamma = sqrt(eq->gamma_b / eq->gamma_g);
    eq->param[2].gamma = sqrt(eq->gamma_r / eq->gamma_g);

    for (i = 0; i < 3; i++) {
        eq->param[i].gamma_weight = eq->gamma_weight;
        eq->param[i].lut_clean = 0;
        check_values(&eq->param[i], eq);
    }
}

static void set_saturation(EQContext *eq)
{
    int i;
#if 0
    eq->saturation = av_clipf(av_expr_eval(eq->saturation_pexpr, eq->var_values, eq), 0.0, 3.0);
#endif

    for (i = 1; i < 3; i++) {
        eq->param[i].contrast = eq->saturation;
        eq->param[i].lut_clean = 0;
        check_values(&eq->param[i], eq);
    }
}

static int set_expr(AVExpr **pexpr, const char *expr, const char *option, void *log_ctx)
{
    int ret;
    AVExpr *old = NULL;

    if (*pexpr)
        old = *pexpr;
    ret = av_expr_parse(pexpr, expr, var_names, NULL, NULL, NULL, NULL, 0, log_ctx);
    if (ret < 0) {
        av_log(log_ctx, AV_LOG_ERROR,
               "Error when parsing the expression '%s' for %s\n",
               expr, option);
        *pexpr = old;
        return ret;
    }

    av_expr_free(old);
    return 0;
}

int eq_initialize(EQContext *eq, double brightness, double contrast, double  saturation)
{
    int ret;
    memset(eq, 0, sizeof(EQContext));
    qDebug()<<"eq_initialize brightness: "<<brightness<<" contrast: "<<contrast<<" saturation: "
        <<saturation;
#if 0
    double brightness=0; // -100~100 -1.0~1.0 0
    double contrast=0;   // -100~100 -2.0~2.0 1
    double saturation=0; // -100~100 0.0~3.0  1
#endif
    brightness=brightness<-100?-100:(brightness>100?100:brightness);
    contrast=contrast<-100?-100:(contrast>100?100:contrast);
    saturation=saturation<-100?-100:(saturation>100?100:saturation);
    eq->brightness=brightness/100; // -100~100 -1.0~1.0 0
    // -100~100 -2.0~2.0 1
    if(contrast<0)
    {
        eq->contrast=(double)contrast*3/100+1;
    }
    else
    {
        eq->contrast=(double)contrast/100+1;
    }
    //-100~100 0.0~3.0  1
    if(saturation<0)
    {
        eq->saturation=(double)saturation/100+1;
    }
    else
    {
        eq->saturation=(double)saturation/50+1;
    }
    qDebug()<<"eq_initialize eq_brightness: "<<eq->brightness<<" eq_contrast: "<<eq->contrast
        <<" eq_saturation: " <<eq->saturation;
    eq->gamma=1;
    eq->gamma_r=1;
    eq->gamma_g=1;
    eq->gamma_b=1;
    eq->gamma_weight=1;
    eq->process = process_c;
    eq->eval_mode = EQContext::EVAL_MODE_FRAME;
#if 0
    eq->contrast_expr=nullChar;
    eq->brightness_expr=nullChar;
    eq->saturation_expr=nullChar;
    eq->gamma_expr=nullChar;
    eq->gamma_r_expr=nullChar;
    eq->gamma_g_expr=nullChar;
    eq->gamma_b_expr=nullChar;
    eq->gamma_weight_expr=nullChar;
    if ((ret = set_expr(&eq->contrast_pexpr,     eq->contrast_expr,     "contrast",     NULL)) < 0 ||
        (ret = set_expr(&eq->brightness_pexpr,   eq->brightness_expr,   "brightness",   NULL)) < 0 ||
        (ret = set_expr(&eq->saturation_pexpr,   eq->saturation_expr,   "saturation",   NULL)) < 0 ||
        (ret = set_expr(&eq->gamma_pexpr,        eq->gamma_expr,        "gamma",        NULL)) < 0 ||
        (ret = set_expr(&eq->gamma_r_pexpr,      eq->gamma_r_expr,      "gamma_r",      NULL)) < 0 ||
        (ret = set_expr(&eq->gamma_g_pexpr,      eq->gamma_g_expr,      "gamma_g",      NULL)) < 0 ||
        (ret = set_expr(&eq->gamma_b_pexpr,      eq->gamma_b_expr,      "gamma_b",      NULL)) < 0 ||
        (ret = set_expr(&eq->gamma_weight_pexpr, eq->gamma_weight_expr, "gamma_weight", NULL)) < 0 )
        return ret;
#endif
#if 0
    if (ARCH_X86)
        ff_eq_init_x86(eq);
    if (eq->eval_mode == EQContext::EVAL_MODE_INIT) {
        //set_gamma(eq);
        set_contrast(eq);
        set_brightness(eq);
        set_saturation(eq);
    }
#endif


    return 0;
}

int eq_filter_frame(EQContext *eq, AVFrame *in, int frame_count_out, AVFrame* &out)
{
    int64_t pos = av_frame_get_pkt_pos(in);
    const AVPixFmtDescriptor *desc;
    int i;

    desc = av_pix_fmt_desc_get((AVPixelFormat)in->format);

    eq->var_values[EQ_VAR_N]   = frame_count_out;
    eq->var_values[EQ_VAR_POS] = pos == -1 ? NAN : pos;
    //eq->var_values[EQ_VAR_T]   = TS2T(in->pts, inlink->time_base);

    if (eq->eval_mode == EQContext::EVAL_MODE_FRAME) {
        set_gamma(eq);
        set_contrast(eq);
        set_brightness(eq);
        set_saturation(eq);
    }

    for (i = 0; i < desc->nb_components; i++) {
        int w = in->width;
        int h = in->height;

        if (i == 1 || i == 2) {
            w = AV_CEIL_RSHIFT(w, desc->log2_chroma_w);
            h = AV_CEIL_RSHIFT(h, desc->log2_chroma_h);
        }

        if (eq->param[i].adjust)
            eq->param[i].adjust(&eq->param[i], out->data[i], out->linesize[i],
                                 in->data[i], in->linesize[i], w, h);
        else
            av_image_copy_plane(out->data[i], out->linesize[i],
                                in->data[i], in->linesize[i], w, h);
    }

    return 0;
}

void eq_uninit(EQContext *eq)
{
#if 0
    av_expr_free(eq->contrast_pexpr);     eq->contrast_pexpr     = NULL;
    av_expr_free(eq->brightness_pexpr);   eq->brightness_pexpr   = NULL;
    av_expr_free(eq->saturation_pexpr);   eq->saturation_pexpr   = NULL;
    av_expr_free(eq->gamma_pexpr);        eq->gamma_pexpr        = NULL;
    av_expr_free(eq->gamma_weight_pexpr); eq->gamma_weight_pexpr = NULL;
    av_expr_free(eq->gamma_r_pexpr);      eq->gamma_r_pexpr      = NULL;
    av_expr_free(eq->gamma_g_pexpr);      eq->gamma_g_pexpr      = NULL;
    av_expr_free(eq->gamma_b_pexpr);      eq->gamma_b_pexpr      = NULL;
#endif
}
