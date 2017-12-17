#include "filter_overlay.h"

static const char *const var_names[] = {
    "main_w",    "W", ///< width  of the main    video
    "main_h",    "H", ///< height of the main    video
    "overlay_w", "w", ///< width  of the overlay video
    "overlay_h", "h", ///< height of the overlay video
    "hsub",
    "vsub",
    "x",
    "y",
    "n",            ///< number of frame
    "pos",          ///< position in the file
    "t",            ///< timestamp expressed in seconds
    NULL
};

enum EOFAction {
    EOF_ACTION_REPEAT,
    EOF_ACTION_ENDALL,
    EOF_ACTION_PASS
};

static const char * const eof_action_str[] = {
    "repeat", "endall", "pass"
};

#define MAIN    0
#define OVERLAY 1

#define R 0
#define G 1
#define B 2
#define A 3

#define Y 0
#define U 1
#define V 2

enum EvalMode {
    EVAL_MODE_INIT,
    EVAL_MODE_FRAME,
    EVAL_MODE_NB
};

enum OverlayFormat {
    OVERLAY_FORMAT_YUV420,
    OVERLAY_FORMAT_YUV422,
    OVERLAY_FORMAT_YUV444,
    OVERLAY_FORMAT_RGB,
    OVERLAY_FORMAT_GBRP,
    OVERLAY_FORMAT_NB
};

av_cold void overlay_uninit(OverlayContext *s)
{
    av_expr_free(s->x_pexpr); s->x_pexpr = NULL;
    av_expr_free(s->y_pexpr); s->y_pexpr = NULL;
}

static inline int normalize_xy(double d, int chroma_sub)
{
    if (isnan(d))
        return INT_MAX;
    return (int)d & ~((1 << chroma_sub) - 1);
}

static void eval_expr(OverlayContext *s)
{
    s->var_values[OVERLAY_VAR_X] = av_expr_eval(s->x_pexpr, s->var_values, NULL);
    s->var_values[OVERLAY_VAR_Y] = av_expr_eval(s->y_pexpr, s->var_values, NULL);
    s->var_values[OVERLAY_VAR_X] = av_expr_eval(s->x_pexpr, s->var_values, NULL);
    s->x = normalize_xy(s->var_values[OVERLAY_VAR_X], s->hsub);
    s->y = normalize_xy(s->var_values[OVERLAY_VAR_Y], s->vsub);
}

static int set_expr(AVExpr **pexpr, const char *expr, const char *option, void *log_ctx)
{
    int ret;
    AVExpr *old = NULL;

    if (*pexpr)
        old = *pexpr;
    ret = av_expr_parse(pexpr, expr, var_names,
                        NULL, NULL, NULL, NULL, 0, log_ctx);
    if (ret < 0) {
        av_log(log_ctx, AV_LOG_ERROR,
               "Error when evaluating the expression '%s' for %s\n",
               expr, option);
        *pexpr = old;
        return ret;
    }

    av_expr_free(old);
    return 0;
}



av_cold int overlay_init(OverlayContext *s, const char* x, const char* y)
{
    memset(s, 0, sizeof(OverlayContext));
    s->x_expr=x;
    s->y_expr=y;
    s->eof_action=EOF_ACTION_REPEAT;
    s->eval_mode=EVAL_MODE_FRAME;
    s->allow_packed_rgb=0;
    s->format=OVERLAY_FORMAT_RGB;//OVERLAY_FORMAT_YUV420;

    if (s->allow_packed_rgb) {
        av_log(NULL, AV_LOG_WARNING,
               "The rgb option is deprecated and is overriding the format option, use format instead\n");
        s->format = OVERLAY_FORMAT_RGB;
    }
    return 0;
}

static const enum AVPixelFormat alpha_pix_fmts[] = {
    AV_PIX_FMT_YUVA420P, AV_PIX_FMT_YUVA422P, AV_PIX_FMT_YUVA444P,
    AV_PIX_FMT_ARGB, AV_PIX_FMT_ABGR, AV_PIX_FMT_RGBA,
    AV_PIX_FMT_BGRA, AV_PIX_FMT_GBRAP, AV_PIX_FMT_NONE
};

int overlay_config_input_overlay(AVFrame *frame, AVFrame *overlay_frame, OverlayContext* s)
{
    int ret;
    //const AVPixFmtDescriptor *pix_desc = av_pix_fmt_desc_get(ctx->pix_fmt);
    const AVPixFmtDescriptor *pix_desc = av_pix_fmt_desc_get((AVPixelFormat)overlay_frame->format);

    av_image_fill_max_pixsteps(s->overlay_pix_step, NULL, pix_desc);

    /* Finish the configuration by evaluating the expressions
       now when both inputs are configured. */
    s->var_values[OVERLAY_VAR_MAIN_W   ] = s->var_values[OVERLAY_VAR_MW] = frame->width;//ctx->inputs[MAIN   ]->w;
    s->var_values[OVERLAY_VAR_MAIN_H   ] = s->var_values[OVERLAY_VAR_MH] = frame->height;//ctx->inputs[MAIN   ]->h;
    s->var_values[OVERLAY_VAR_OVERLAY_W] = s->var_values[OVERLAY_VAR_OW] = overlay_frame->width;//ctx->inputs[OVERLAY]->w;
    s->var_values[OVERLAY_VAR_OVERLAY_H] = s->var_values[OVERLAY_VAR_OH] = overlay_frame->height;//ctx->inputs[OVERLAY]->h;
    s->var_values[OVERLAY_VAR_HSUB]  = 1<<pix_desc->log2_chroma_w;
    s->var_values[OVERLAY_VAR_VSUB]  = 1<<pix_desc->log2_chroma_h;
    s->var_values[OVERLAY_VAR_X]     = NAN;
    s->var_values[OVERLAY_VAR_Y]     = NAN;
    s->var_values[OVERLAY_VAR_N]     = 0;
    s->var_values[OVERLAY_VAR_T]     = NAN;
    s->var_values[OVERLAY_VAR_POS]   = NAN;

    if ((ret = set_expr(&s->x_pexpr,      s->x_expr,      "x",      NULL)) < 0 ||
        (ret = set_expr(&s->y_pexpr,      s->y_expr,      "y",      NULL)) < 0)
        return ret;

    s->overlay_is_packed_rgb =
        //ff_fill_rgba_map(s->overlay_rgba_map, ctx->pix_fmt) >= 0;
        ff_fill_rgba_map(s->overlay_rgba_map, (AVPixelFormat)overlay_frame->format) >= 0;
    //s->overlay_has_alpha = ff_fmt_is_in(ctx->pix_fmt, (const int*)alpha_pix_fmts);
    s->overlay_has_alpha = ff_fmt_is_in((AVPixelFormat)overlay_frame->format, (const int*)alpha_pix_fmts);

    if (s->eval_mode == EVAL_MODE_INIT) {
        eval_expr(s);
        av_log(NULL, AV_LOG_VERBOSE, "x:%f xi:%d y:%f yi:%d\n",
               s->var_values[OVERLAY_VAR_X], s->x,
               s->var_values[OVERLAY_VAR_Y], s->y);
    }

    av_log(NULL, AV_LOG_INFO,
           "main w:%d h:%d fmt:%s overlay w:%d h:%d fmt:%s eof_action:%s\n",
           frame->width, frame->height,
           av_get_pix_fmt_name((AVPixelFormat)frame->format),
           overlay_frame->width, overlay_frame->height,
           av_get_pix_fmt_name((AVPixelFormat)overlay_frame->format),
           eof_action_str[s->eof_action]);
    return 0;
}


// divide by 255 and round to nearest
// apply a fast variant: (X+127)/255 = ((X+127)*257+257)>>16 = ((X+128)*257)>>16
#define FAST_DIV255(x) ((((x) + 128) * 257) >> 16)

// calculate the unpremultiplied alpha, applying the general equation:
// alpha = alpha_overlay / ( (alpha_main + alpha_overlay) - (alpha_main * alpha_overlay) )
// (((x) << 16) - ((x) << 9) + (x)) is a faster version of: 255 * 255 * x
// ((((x) + (y)) << 8) - ((x) + (y)) - (y) * (x)) is a faster version of: 255 * (x + y)
#define UNPREMULTIPLY_ALPHA(x, y) ((((x) << 16) - ((x) << 9) + (x)) / ((((x) + (y)) << 8) - ((x) + (y)) - (y) * (x)))

/**
 * Blend image in src to destination buffer dst at position (x, y).
 */

static void blend_image_packed_rgb(OverlayContext *s,
                                   AVFrame *dst, const AVFrame *src,
                                   int x, int y)
{
    int i, imax, j, jmax;
    const int src_w = src->width;
    const int src_h = src->height;
    const int dst_w = dst->width;
    const int dst_h = dst->height;
    uint8_t alpha;          ///< the amount of overlay to blend on to main
    const int dr = s->main_rgba_map[R];
    const int dg = s->main_rgba_map[G];
    const int db = s->main_rgba_map[B];
    const int da = s->main_rgba_map[A];
    const int dstep = s->main_pix_step[0];
    const int sr = s->overlay_rgba_map[R];
    const int sg = s->overlay_rgba_map[G];
    const int sb = s->overlay_rgba_map[B];
    const int sa = s->overlay_rgba_map[A];
    const int sstep = s->overlay_pix_step[0];
    const int main_has_alpha = s->main_has_alpha;
    uint8_t *S, *sp, *d, *dp;

    i = FFMAX(-y, 0);
    sp = src->data[0] + i     * src->linesize[0];
    dp = dst->data[0] + (y+i) * dst->linesize[0];

    for (imax = FFMIN(-y + dst_h, src_h); i < imax; i++) {
        j = FFMAX(-x, 0);
        S = sp + j     * sstep;
        d = dp + (x+j) * dstep;

        for (jmax = FFMIN(-x + dst_w, src_w); j < jmax; j++) {
            alpha = S[sa];

            // if the main channel has an alpha channel, alpha has to be calculated
            // to create an un-premultiplied (straight) alpha value
            if (main_has_alpha && alpha != 0 && alpha != 255) {
                uint8_t alpha_d = d[da];
                alpha = UNPREMULTIPLY_ALPHA(alpha, alpha_d);
            }

            switch (alpha) {
            case 0:
                break;
            case 255:
                d[dr] = S[sr];
                d[dg] = S[sg];
                d[db] = S[sb];
                break;
            default:
                // main_value = main_value * (1 - alpha) + overlay_value * alpha
                // since alpha is in the range 0-255, the result must divided by 255
                d[dr] = FAST_DIV255(d[dr] * (255 - alpha) + S[sr] * alpha);
                d[dg] = FAST_DIV255(d[dg] * (255 - alpha) + S[sg] * alpha);
                d[db] = FAST_DIV255(d[db] * (255 - alpha) + S[sb] * alpha);
            }
            if (main_has_alpha) {
                switch (alpha) {
                case 0:
                    break;
                case 255:
                    d[da] = S[sa];
                    break;
                default:
                    // apply alpha compositing: main_alpha += (1-main_alpha) * overlay_alpha
                    d[da] += FAST_DIV255((255 - d[da]) * S[sa]);
                }
            }
            d += dstep;
            S += sstep;
        }
        dp += dst->linesize[0];
        sp += src->linesize[0];
    }
}

static av_always_inline void blend_plane(AVFrame *dst, const AVFrame *src,
                                         int src_w, int src_h,
                                         int dst_w, int dst_h,
                                         int i, int hsub, int vsub,
                                         int x, int y,
                                         int main_has_alpha,
                                         int dst_plane,
                                         int dst_offset,
                                         int dst_step)
{
    int src_wp = AV_CEIL_RSHIFT(src_w, hsub);
    int src_hp = AV_CEIL_RSHIFT(src_h, vsub);
    int dst_wp = AV_CEIL_RSHIFT(dst_w, hsub);
    int dst_hp = AV_CEIL_RSHIFT(dst_h, vsub);
    int yp = y>>vsub;
    int xp = x>>hsub;
    uint8_t *s, *sp, *d, *dp, *a, *ap;
    int jmax, j, k, kmax;

    j = FFMAX(-yp, 0);
    sp = src->data[i] + j         * src->linesize[i];
    dp = dst->data[dst_plane]
                      + (yp+j)    * dst->linesize[dst_plane]
                      + dst_offset;
    ap = src->data[3] + (j<<vsub) * src->linesize[3];

    for (jmax = FFMIN(-yp + dst_hp, src_hp); j < jmax; j++) {
        k = FFMAX(-xp, 0);
        d = dp + (xp+k) * dst_step;
        s = sp + k;
        a = ap + (k<<hsub);

        for (kmax = FFMIN(-xp + dst_wp, src_wp); k < kmax; k++) {
            int alpha_v, alpha_h, alpha;

            // average alpha for color components, improve quality
            if (hsub && vsub && j+1 < src_hp && k+1 < src_wp) {
                alpha = (a[0] + a[src->linesize[3]] +
                         a[1] + a[src->linesize[3]+1]) >> 2;
            } else if (hsub || vsub) {
                alpha_h = hsub && k+1 < src_wp ?
                    (a[0] + a[1]) >> 1 : a[0];
                alpha_v = vsub && j+1 < src_hp ?
                    (a[0] + a[src->linesize[3]]) >> 1 : a[0];
                alpha = (alpha_v + alpha_h) >> 1;
            } else
                alpha = a[0];
            // if the main channel has an alpha channel, alpha has to be calculated
            // to create an un-premultiplied (straight) alpha value
            if (main_has_alpha && alpha != 0 && alpha != 255) {
                // average alpha for color components, improve quality
                uint8_t alpha_d;
                if (hsub && vsub && j+1 < src_hp && k+1 < src_wp) {
                    alpha_d = (d[0] + d[src->linesize[3]] +
                               d[1] + d[src->linesize[3]+1]) >> 2;
                } else if (hsub || vsub) {
                    alpha_h = hsub && k+1 < src_wp ?
                        (d[0] + d[1]) >> 1 : d[0];
                    alpha_v = vsub && j+1 < src_hp ?
                        (d[0] + d[src->linesize[3]]) >> 1 : d[0];
                    alpha_d = (alpha_v + alpha_h) >> 1;
                } else
                    alpha_d = d[0];
                alpha = UNPREMULTIPLY_ALPHA(alpha, alpha_d);
            }
            *d = FAST_DIV255(*d * (255 - alpha) + *s * alpha);
            s++;
            d += dst_step;
            a += 1 << hsub;
        }
        dp += dst->linesize[dst_plane];
        sp += src->linesize[i];
        ap += (1 << vsub) * src->linesize[3];
    }
}

static inline void alpha_composite(const AVFrame *src, const AVFrame *dst,
                                   int src_w, int src_h,
                                   int dst_w, int dst_h,
                                   int x, int y)
{
    uint8_t alpha;          ///< the amount of overlay to blend on to main
    uint8_t *s, *sa, *d, *da;
    int i, imax, j, jmax;

    i = FFMAX(-y, 0);
    sa = src->data[3] + i     * src->linesize[3];
    da = dst->data[3] + (y+i) * dst->linesize[3];

    for (imax = FFMIN(-y + dst_h, src_h); i < imax; i++) {
        j = FFMAX(-x, 0);
        s = sa + j;
        d = da + x+j;

        for (jmax = FFMIN(-x + dst_w, src_w); j < jmax; j++) {
            alpha = *s;
            if (alpha != 0 && alpha != 255) {
                uint8_t alpha_d = *d;
                alpha = UNPREMULTIPLY_ALPHA(alpha, alpha_d);
            }
            switch (alpha) {
            case 0:
                break;
            case 255:
                *d = *s;
                break;
            default:
                // apply alpha compositing: main_alpha += (1-main_alpha) * overlay_alpha
                *d += FAST_DIV255((255 - *d) * *s);
            }
            d += 1;
            s += 1;
        }
        da += dst->linesize[3];
        sa += src->linesize[3];
    }
}

static av_always_inline void blend_image_yuv(OverlayContext *s,
                                             AVFrame *dst, const AVFrame *src,
                                             int hsub, int vsub,
                                             int main_has_alpha,
                                             int x, int y)
{
    const int src_w = src->width;
    const int src_h = src->height;
    const int dst_w = dst->width;
    const int dst_h = dst->height;

    if (main_has_alpha)
        alpha_composite(src, dst, src_w, src_h, dst_w, dst_h, x, y);

    blend_plane(dst, src, src_w, src_h, dst_w, dst_h, 0, 0,       0, x, y, main_has_alpha,
                s->main_desc->comp[0].plane, s->main_desc->comp[0].offset, s->main_desc->comp[0].step);
    blend_plane(dst, src, src_w, src_h, dst_w, dst_h, 1, hsub, vsub, x, y, main_has_alpha,
                s->main_desc->comp[1].plane, s->main_desc->comp[1].offset, s->main_desc->comp[1].step);
    blend_plane(dst, src, src_w, src_h, dst_w, dst_h, 2, hsub, vsub, x, y, main_has_alpha,
                s->main_desc->comp[2].plane, s->main_desc->comp[2].offset, s->main_desc->comp[2].step);
}

static av_always_inline void blend_image_rgb(OverlayContext *s, 
                                             AVFrame *dst, const AVFrame *src,
                                             int hsub, int vsub,
                                             int main_has_alpha,
                                             int x, int y)
{
    const int src_w = src->width;
    const int src_h = src->height;
    const int dst_w = dst->width;
    const int dst_h = dst->height;

    if (main_has_alpha)
        alpha_composite(src, dst, src_w, src_h, dst_w, dst_h, x, y);

    blend_plane(dst, src, src_w, src_h, dst_w, dst_h, 0, 0,       0, x, y, main_has_alpha,
                s->main_desc->comp[1].plane, s->main_desc->comp[1].offset, s->main_desc->comp[1].step);
    blend_plane(dst, src, src_w, src_h, dst_w, dst_h, 1, hsub, vsub, x, y, main_has_alpha,
                s->main_desc->comp[2].plane, s->main_desc->comp[2].offset, s->main_desc->comp[2].step);
    blend_plane(dst, src, src_w, src_h, dst_w, dst_h, 2, hsub, vsub, x, y, main_has_alpha,
                s->main_desc->comp[0].plane, s->main_desc->comp[0].offset, s->main_desc->comp[0].step);
}

static void blend_image_yuv420(OverlayContext *s, AVFrame *dst, const AVFrame *src, int x, int y)
{
    blend_image_yuv(s, dst, src, 1, 1, s->main_has_alpha, x, y);
}

static void blend_image_yuv422(OverlayContext *s, AVFrame *dst, const AVFrame *src, int x, int y)
{

    blend_image_yuv(s, dst, src, 1, 0, s->main_has_alpha, x, y);
}

static void blend_image_yuv444(OverlayContext *s, AVFrame *dst, const AVFrame *src, int x, int y)
{
    blend_image_yuv(s, dst, src, 0, 0, s->main_has_alpha, x, y);
}

static void blend_image_gbrp(OverlayContext *s, AVFrame *dst, const AVFrame *src, int x, int y)
{
    blend_image_rgb(s, dst, src, 0, 0, s->main_has_alpha, x, y);
}


int overlay_config_input_main(AVFrame* frame, OverlayContext *s)
{
    const AVPixFmtDescriptor *pix_desc = av_pix_fmt_desc_get((AVPixelFormat)frame->format);

    av_image_fill_max_pixsteps(s->main_pix_step,    NULL, pix_desc);

    s->hsub = pix_desc->log2_chroma_w;
    s->vsub = pix_desc->log2_chroma_h;

    s->main_desc = pix_desc;

    s->main_is_packed_rgb =
        ff_fill_rgba_map(s->main_rgba_map, (AVPixelFormat)frame->format) >= 0;
    s->main_has_alpha = ff_fmt_is_in((AVPixelFormat)frame->format, (const int*)alpha_pix_fmts);
    switch (s->format) {
    case OVERLAY_FORMAT_YUV420:
        s->blend_image = blend_image_yuv420;
        break;
    case OVERLAY_FORMAT_YUV422:
        s->blend_image = blend_image_yuv422;
        break;
    case OVERLAY_FORMAT_YUV444:
        s->blend_image = blend_image_yuv444;
        break;
    case OVERLAY_FORMAT_RGB:
        s->blend_image = blend_image_packed_rgb;
        break;
    case OVERLAY_FORMAT_GBRP:
        s->blend_image = blend_image_gbrp;
        break;
    }
    return 0;
}

AVFrame *overlay_blend(OverlayContext* s, AVFrame *mainpic,
                         const AVFrame *second, int64_t frame_count_out, double pts)
{
    double time = av_gettime_relative() / 1000.0;
    if (s->eval_mode == EVAL_MODE_FRAME) {
        int64_t pos = av_frame_get_pkt_pos(mainpic);

        s->var_values[OVERLAY_VAR_N] = frame_count_out;//inlink->frame_count_out;
        s->var_values[OVERLAY_VAR_T] = pts;//mainpic->pts == AV_NOPTS_VALUE ?  NAN : mainpic->pts * av_q2d(ctx->time_base);
        s->var_values[OVERLAY_VAR_POS] = pos == -1 ? NAN : pos;

        s->var_values[OVERLAY_VAR_OVERLAY_W] = s->var_values[OVERLAY_VAR_OW] = second->width;
        s->var_values[OVERLAY_VAR_OVERLAY_H] = s->var_values[OVERLAY_VAR_OH] = second->height;
        s->var_values[OVERLAY_VAR_MAIN_W   ] = s->var_values[OVERLAY_VAR_MW] = mainpic->width;
        s->var_values[OVERLAY_VAR_MAIN_H   ] = s->var_values[OVERLAY_VAR_MH] = mainpic->height;

        eval_expr(s);
        av_log(NULL, AV_LOG_INFO, "n:%f t:%f pos:%f x:%f xi:%d y:%f yi:%d\n",
               s->var_values[OVERLAY_VAR_N], s->var_values[OVERLAY_VAR_T], s->var_values[OVERLAY_VAR_POS],
               s->var_values[OVERLAY_VAR_X], s->x,
               s->var_values[OVERLAY_VAR_Y], s->y);
    }

    if (s->x < mainpic->width  && s->x + second->width  >= 0 ||
        s->y < mainpic->height && s->y + second->height >= 0)
        s->blend_image(s, mainpic, second, s->x, s->y);
    av_log(NULL, AV_LOG_INFO, "overlay_blend waste_time: %f\n", av_gettime_relative() / 1000.0-time);
    return mainpic;
}

