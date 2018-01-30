#include "filter_crop.h"
#include <qDebug>
#define qInfo qDebug

static const char *const var_names[] = {
    "in_w", "iw",   ///< width  of the input video
    "in_h", "ih",   ///< height of the input video
    "out_w", "ow",  ///< width  of the cropped video
    "out_h", "oh",  ///< height of the cropped video
    "a",
    "sar",
    "dar",
    "hsub",
    "vsub",
    "x",
    "y",
    "n",            ///< number of frame
    "pos",          ///< position in the file
    "t",            ///< timestamp expressed in seconds
    NULL
};


static inline int normalize_double(int *n, double d)
{
    int ret = 0;

    if (isnan(d)) {
        ret = AVERROR(EINVAL);
    } else if (d > INT_MAX || d < INT_MIN) {
        *n = d > INT_MAX ? INT_MAX : INT_MIN;
        ret = AVERROR(EINVAL);
    } else
        *n = lrint(d);

    return ret;
}
void crop_init(CropContext* s, char* w, char*h, char* x, char* y, int keep_aspect, int exact)
{
#if 1
    memset(s, 0, sizeof(CropContext));
    s->w_expr=w;
    s->h_expr=h;
    s->x_expr=x;
    s->y_expr=y;
    s->keep_aspect=keep_aspect;
    s->exact=exact;
#endif
    ///av_log(NULL, AV_LOG_INFO, "CROP_VAR_VARS_NB: %d sizeof(double*): %d\n",CROP_VAR_VARS_NB, sizeof(double*));
    ////s->var_values = (double*)av_malloc(CROP_VAR_VARS_NB*sizeof(double*));
}
void crop_init_int(CropContext* s, int w, int h, int x, int y, int keep_aspect, int exact)
{
#if 1
    memset(s, 0, sizeof(CropContext));
    s->w=w;
    s->h=h;
    s->x=x;
    s->y=y;
    s->keep_aspect=keep_aspect;
    s->exact=exact;
#endif
    ///av_log(NULL, AV_LOG_INFO, "CROP_VAR_VARS_NB: %d sizeof(double*): %d\n",CROP_VAR_VARS_NB, sizeof(double*));
    ////s->var_values = (double*)av_malloc(CROP_VAR_VARS_NB*sizeof(double*));
    qDebug()<<"crop_init_int w: "<<w<<" h: "<<h<<" x: "<<x<<" y: "<<y;
}

int crop_config_input(AVFrame* frame, CropContext* s)
{
    const AVPixFmtDescriptor *pix_desc = av_pix_fmt_desc_get((AVPixelFormat)frame->format);
    //int ret;
    //const char *expr;
    //double res;
    AVRational dar; //storm

    s->var_values[CROP_VAR_IN_W]  = s->var_values[CROP_VAR_IW] = frame->width;//ctx->inputs[0]->w;
    s->var_values[CROP_VAR_IN_H]  = s->var_values[CROP_VAR_IH] = frame->height;//ctx->inputs[0]->h;
    s->var_values[CROP_VAR_A]     = (float) frame->width / frame->height;//(float) link->w / link->h;
    //s->var_values[CROP_VAR_SAR]   = link->sample_aspect_ratio.num ? av_q2d(link->sample_aspect_ratio) : 1;
    s->var_values[CROP_VAR_SAR]   = frame->sample_aspect_ratio.num ? av_q2d(frame->sample_aspect_ratio) : 1;
    s->var_values[CROP_VAR_DAR]   = s->var_values[CROP_VAR_A] * s->var_values[CROP_VAR_SAR];
    s->var_values[CROP_VAR_HSUB]  = 1<<pix_desc->log2_chroma_w;
    s->var_values[CROP_VAR_VSUB]  = 1<<pix_desc->log2_chroma_h;
    s->var_values[CROP_VAR_X]     = NAN;
    s->var_values[CROP_VAR_Y]     = NAN;
    s->var_values[CROP_VAR_OUT_W] = s->var_values[CROP_VAR_OW] = NAN;
    s->var_values[CROP_VAR_OUT_H] = s->var_values[CROP_VAR_OH] = NAN;
    s->var_values[CROP_VAR_N]     = 0;
    s->var_values[CROP_VAR_T]     = NAN;
    s->var_values[CROP_VAR_POS]   = NAN;

    av_image_fill_max_pixsteps(s->max_step, NULL, pix_desc);
    s->hsub = pix_desc->log2_chroma_w;
    s->vsub = pix_desc->log2_chroma_h;
#if 0
    if ((ret = av_expr_parse_and_eval(&res, (expr = s->w_expr),
                                      var_names, s->var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, NULL)) < 0)
        goto fail_expr;
    s->var_values[CROP_VAR_OUT_W] = s->var_values[CROP_VAR_OW] = res;
    if ((ret = av_expr_parse_and_eval(&res, (expr = s->h_expr),
                                      var_names, s->var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, NULL)) < 0)
        goto fail_expr;
    s->var_values[CROP_VAR_OUT_H] = s->var_values[CROP_VAR_OH] = res;
    /* evaluate again ow as it may depend on oh */
    if ((ret = av_expr_parse_and_eval(&res, (expr = s->w_expr),
                                      var_names, s->var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, NULL)) < 0)
        goto fail_expr;

    s->var_values[CROP_VAR_OUT_W] = s->var_values[CROP_VAR_OW] = res;
    if (normalize_double(&s->w, s->var_values[CROP_VAR_OUT_W]) < 0 ||
        normalize_double(&s->h, s->var_values[CROP_VAR_OUT_H]) < 0) {
        av_log(NULL, AV_LOG_ERROR,
               "Too big value or invalid expression for out_w/ow or out_h/oh. "
               "Maybe the expression for out_w:'%s' or for out_h:'%s' is self-referencing.\n",
               s->w_expr, s->h_expr);
        return AVERROR(EINVAL);
    }

    av_expr_free(s->x_pexpr);
    av_expr_free(s->y_pexpr);
    s->x_pexpr = s->y_pexpr = NULL;
    if ((ret = av_expr_parse(&s->x_pexpr, s->x_expr, var_names,
                             NULL, NULL, NULL, NULL, 0, NULL)) < 0 ||
        (ret = av_expr_parse(&s->y_pexpr, s->y_expr, var_names,
                             NULL, NULL, NULL, NULL, 0, NULL)) < 0)
        return AVERROR(EINVAL);
#endif
    if (!s->exact) {
        s->w &= ~((1 << s->hsub) - 1);
        s->h &= ~((1 << s->vsub) - 1);
    }

    dar = av_mul_q(frame->sample_aspect_ratio,
            (AVRational){ frame->width, frame->height }); //storm
    if (s->keep_aspect) {
        av_reduce(&s->out_sar.num, &s->out_sar.den,
                  dar.num * s->h, dar.den * s->w, INT_MAX);
    } else
        s->out_sar = frame->sample_aspect_ratio;
#if 0
    av_log(NULL, AV_LOG_INFO, "w:%d h:%d dar:%d/%d sar:%d/%d -> w:%d h:%d sar:%d/%d\n",
           frame->width, frame->height, dar.num, dar.den,frame->sample_aspect_ratio.num, frame->sample_aspect_ratio.den,
           s->w, s->h, s->out_sar.num, s->out_sar.den);
#endif
    qDebug()<<"crop_config_input w:"<<frame->width<<" h:"<<frame->height<<" dar:"<<dar.num
        <<"/"<<dar.den<<" sar:"<<frame->sample_aspect_ratio.num<<"/"<<frame->sample_aspect_ratio.den
        <<" -> w:"<<s->w<<" h:"<<s->h<<" sar:"<<s->out_sar.num<<"/"<<s->out_sar.den<<" x: "<<s->x
        <<" y: "<<s->y;

    if (s->w <= 0 || s->h <= 0 ||
        s->w > frame->width || s->h > frame->height) {
#if 0
        av_log(NULL, AV_LOG_ERROR,
               "Invalid too big or non positive size for width '%d' or height '%d'\n",
               s->w, s->h);
#endif
        qInfo()<<"crop_config_input error Invalid too big or non positive size for width "<<s->w
            <<" or height "<<s->h;
        return AVERROR(EINVAL);
    }
#if 0
    /* set default, required in the case the first computed value for x/y is NAN */
    s->x = (frame->width - s->w) / 2;
    s->y = (frame->height - s->h) / 2;
    if (!s->exact) {
        s->x &= ~((1 << s->hsub) - 1);
        s->y &= ~((1 << s->vsub) - 1);
    }
#endif
    return 0;
#if 0
fail_expr:
    //av_log(NULL, AV_LOG_ERROR, "Error when evaluating the expression '%s'\n", expr);
    qInfo()<<"crop_config_input error when evaluating the expression";
    return ret;
#endif
}

void crop_filter_frame(CropContext* s, AVFrame* frame, int64_t frame_count_out , double pts)
{
    const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get((AVPixelFormat)frame->format);
    int i;
    double time = av_gettime_relative() / 1000.0;
    int oldwidth=frame->width;
    int oldheight=frame->height;

    frame->width  = s->w;
    frame->height = s->h;

    //link -> c;  link->h -> c->height; link->w -> c->width
    s->var_values[CROP_VAR_N] = frame_count_out;//link->frame_count_out;
    s->var_values[CROP_VAR_T] = 0;//pts;//frame->pts == AV_NOPTS_VALUE ?  NAN : frame->pts * av_q2d(c->time_base);
    s->var_values[CROP_VAR_POS] = av_frame_get_pkt_pos(frame) == -1 ?
        NAN : av_frame_get_pkt_pos(frame);
    s->var_values[CROP_VAR_X] = s->x;//av_expr_eval(s->x_pexpr, s->var_values, NULL);
    s->var_values[CROP_VAR_Y] = s->y;//av_expr_eval(s->y_pexpr, s->var_values, NULL);
    s->var_values[CROP_VAR_X] = s->x;//av_expr_eval(s->x_pexpr, s->var_values, NULL);

    normalize_double(&s->x, s->var_values[CROP_VAR_X]);
    normalize_double(&s->y, s->var_values[CROP_VAR_Y]);

    if (s->x < 0)
        s->x = 0;
    if (s->y < 0)
        s->y = 0;
    if ((unsigned)s->x + (unsigned)s->w > oldwidth)
        s->x = oldwidth - s->w;
    if ((unsigned)s->y + (unsigned)s->h > oldheight)
        s->y = oldheight - s->h;
    if (!s->exact) {
        s->x &= ~((1 << s->hsub) - 1);
        s->y &= ~((1 << s->vsub) - 1);
    }
#if 0
    av_log(NULL, AV_LOG_INFO, "n:%d t:%f pos:%f x:%d y:%d x+w:%d y+h:%d\n",
            (int)s->var_values[CROP_VAR_N], s->var_values[CROP_VAR_T], s->var_values[CROP_VAR_POS],
            s->x, s->y, s->x+s->w, s->y+s->h);
#endif
    qDebug()<<"crop_filter_frame n: "<<s->var_values[CROP_VAR_N]<<" t: "<<s->var_values[CROP_VAR_T]
        <<" pos: "<<s->var_values[CROP_VAR_POS]<<" x: "<<s->x<<" y: "<<s->y<<" w: "<<s->w<<" h: "<<s->h
        <<" x+w: " <<(s->x+s->w)<<" y+h: "<<(s->y+s->h)<<" frame_w: "<<oldwidth<<" frame_h: "
        <<oldheight;

    frame->data[0] += s->y * frame->linesize[0];
    frame->data[0] += s->x * s->max_step[0];

    if (!(desc->flags & AV_PIX_FMT_FLAG_PAL || desc->flags & AV_PIX_FMT_FLAG_PSEUDOPAL)) {
        for (i = 1; i < 3; i ++) {
            if (frame->data[i]) {
                frame->data[i] += (s->y >> s->vsub) * frame->linesize[i];
                frame->data[i] += (s->x * s->max_step[i]) >> s->hsub;
            }
        }
    }

    /* alpha plane */
    if (frame->data[3]) {
        frame->data[3] += s->y * frame->linesize[3];
        frame->data[3] += s->x * s->max_step[3];
    }
    av_log(NULL, AV_LOG_INFO, "crop_filter_frame waste_time: %f\n", av_gettime_relative() / 1000.0-time);
    return;
}

av_cold void crop_uninit(CropContext* s)
{
    if(s->x_pexpr)
    {
        av_expr_free(s->x_pexpr);
        s->x_pexpr = NULL;
    }
    if(s->y_pexpr)
    {
        av_expr_free(s->y_pexpr);
        s->y_pexpr = NULL;
    }
}
