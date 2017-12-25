#include "filter_zoompan.h"

extern "C" {
#include "libavutil/avassert.h"
#include "libavutil/eval.h"
#include "libavutil/opt.h"
#include "libavutil/pixdesc.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/formats.h"
#include "libavfilter/internal.h"
#include "libavfilter/video.h"
#include "libswscale/swscale.h"
}
#include <QDebug>

static const char *const var_names[] = {
    "in_w",   "iw",
    "in_h",   "ih",
    "out_w",  "ow",
    "out_h",  "oh",
    "middle_w",  "mw", //storm
    "middle_h",  "mh", //storm
    "in", //
    "on",
    "duration",
    "pduration",
    "time",
    "frame",
    "zoom",
    "pzoom",
    "x", "px",
    "y", "py",
    "a",
    "sar",
    "dar",
    "hsub",
    "vsub",
    NULL
};

av_cold int zoompan_init(ZPcontext *s, const char* zoom, int w, int h, const char* x, const char* y, const char* duration /*, AVRational framerate*/)
{
    memset(s, 0, sizeof(ZPcontext));
    s->zoom_expr_str = zoom;
    s->x_expr_str=x;
    s->y_expr_str=y;
    s->duration_expr_str = duration;
    s->w=w;
    s->h=h;
    //s->framerate=framerate;
    
    s->prev_zoom = 1;
    s->ox_expr_str="0";
    s->oy_expr_str="0";
    return 0;
}

static int output_single_frame(/*AVCodecContext *ctx,*/ZPContext *s, AVFrame *in, double *var_values, int i,
                               int64_t frame_count_in, double *zoom, double *dx, double *dy, AVFrame *&out)
{
    int64_t pts = s->frame_count;
    int k, x, y, w, h, ret = 0;
    uint8_t *input[4];
    int px[4], py[4];
    qDebug()<<"output_single_frame i: "<<i<<" frame_count_in: "<<frame_count_in<<" s->x: "<<s->x
        <<" s->y: "<<s->y<<" s->prev_zoom: "<<s->prev_zoom<<" s->prev_nb_frames: "<<s->prev_nb_frames;

    var_values[ZOOMPAN_VAR_PX]    = s->x;
    var_values[ZOOMPAN_VAR_PY]    = s->y;
    var_values[ZOOMPAN_VAR_PZOOM] = s->prev_zoom;
    var_values[ZOOMPAN_VAR_PDURATION] = s->prev_nb_frames;
    //var_values[ZOOMPAN_VAR_TIME] = pts;// * av_q2d(ctx->time_base);
    var_values[ZOOMPAN_VAR_FRAME] = i;
    //var_values[ZOOMPAN_VAR_ON] = outlink->frame_count_in + 1;
    var_values[ZOOMPAN_VAR_ON] = frame_count_in + 1;
    if ((ret = av_expr_parse_and_eval(zoom, s->zoom_expr_str,
                                      var_names, var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, NULL)) < 0)
        return ret;

    *zoom = av_clipd(*zoom, 1, 10);
    var_values[ZOOMPAN_VAR_ZOOM] = *zoom;
    if(s->mwhx)
    {
        w = in->width * (1.0 / *zoom); //storm
        var_values[ZOOMPAN_VAR_MIDDLE_W] =var_values[ZOOMPAN_VAR_MW] = w = in->width - ((int)((in->width-w)/2))*2;
    }
    else
    {
        var_values[ZOOMPAN_VAR_MIDDLE_W] =var_values[ZOOMPAN_VAR_MW] = w = in->width * (1.0 / *zoom); //storm
    }
    if(s->mhhy)
    {
        h = in->height * (1.0 / *zoom); //storm
        var_values[ZOOMPAN_VAR_MIDDLE_H] =var_values[ZOOMPAN_VAR_MH] = h = in->height - ((int)((in->height-h)/2))*2;
    }
    else
    {
        var_values[ZOOMPAN_VAR_MIDDLE_H] =var_values[ZOOMPAN_VAR_MH] = h = in->height * (1.0 / *zoom); //storm
    }

    if ((ret = av_expr_parse_and_eval(dx, s->x_expr_str,
                                      var_names, var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, NULL)) < 0)
        return ret;
    x = *dx = av_clipd(*dx, 0, FFMAX(in->width - w, 0));
    var_values[ZOOMPAN_VAR_X] = *dx;
    if(!s->mwx && !s->mwhx)
        x &= ~((1 << s->desc->log2_chroma_w) - 1); //storm

    if ((ret = av_expr_parse_and_eval(dy, s->y_expr_str,
                                      var_names, var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, NULL)) < 0)
        return ret;
    y = *dy = av_clipd(*dy, 0, FFMAX(in->height - h, 0));
    var_values[ZOOMPAN_VAR_Y] = *dy;
    if(!s->mhy && !s->mhhy)
        y &= ~((1 << s->desc->log2_chroma_h) - 1); //storm

#if 0
    out = ff_get_video_buffer(outlink, outlink->w, outlink->h);
    if (!out) {
        ret = AVERROR(ENOMEM);
        return ret;
    }
#endif

    px[1] = px[2] = AV_CEIL_RSHIFT(x, s->desc->log2_chroma_w);
    px[0] = px[3] = x;

    py[1] = py[2] = AV_CEIL_RSHIFT(y, s->desc->log2_chroma_h);
    py[0] = py[3] = y;

    s->sws = sws_alloc_context();
    if (!s->sws) {
        ret = AVERROR(ENOMEM);
        goto error;
    }

    for (k = 0; in->data[k]; k++)
        input[k] = in->data[k] + py[k] * in->linesize[k] + px[k];

    av_opt_set_int(s->sws, "srcw", w, 0);
    av_opt_set_int(s->sws, "srch", h, 0);
    av_opt_set_int(s->sws, "src_format", in->format, 0);
    av_opt_set_int(s->sws, "dstw", out->width, 0);
    av_opt_set_int(s->sws, "dsth", out->height, 0);
    av_opt_set_int(s->sws, "dst_format", out->format, 0);
    av_opt_set_int(s->sws, "sws_flags", SWS_BICUBIC, 0);

    if ((ret = sws_init_context(s->sws, NULL, NULL)) < 0)
        goto error;

    sws_scale(s->sws, (const uint8_t *const *)&input, in->linesize, 0, h, out->data, out->linesize);

    out->pts = pts;
    s->frame_count++;

    ////ret = ff_filter_frame(outlink, out);
    sws_freeContext(s->sws);
    s->sws = NULL;
    s->current_frame++;
    return ret;
error:
    av_frame_free(&out);
    return ret;
}

//init
int zoompan_filter_frame(/*AVCodecContext *ctx,*/ZPContext *s, AVFrame *in, int64_t frame_count_in
        , int64_t frame_count_out)
{
    double nb_frames, ox=0, oy=0; //storm
    int ret;
    s->desc = av_pix_fmt_desc_get((AVPixelFormat)in->format);

    //av_assert0(s->in == NULL);

    s->finished = 0;
    s->var_values[ZOOMPAN_VAR_IN_W]  = s->var_values[ZOOMPAN_VAR_IW] = in->width;
    s->var_values[ZOOMPAN_VAR_IN_H]  = s->var_values[ZOOMPAN_VAR_IH] = in->height;
    s->var_values[ZOOMPAN_VAR_OUT_W] = s->var_values[ZOOMPAN_VAR_OW] = s->w;
    s->var_values[ZOOMPAN_VAR_OUT_H] = s->var_values[ZOOMPAN_VAR_OH] = s->h;
    s->var_values[ZOOMPAN_VAR_IN]    = frame_count_out + 1;
    s->var_values[ZOOMPAN_VAR_ON]    = frame_count_in + 1;
    s->var_values[ZOOMPAN_VAR_PX]    = s->x;
    s->var_values[ZOOMPAN_VAR_PY]    = s->y;
    //s->var_values[ZOOMPAN_VAR_X]     = 0; //storm
    //s->var_values[ZOOMPAN_VAR_Y]     = 0; //storm
    s->var_values[ZOOMPAN_VAR_PZOOM] = s->prev_zoom;
    s->var_values[ZOOMPAN_VAR_ZOOM]  = 1;
    s->var_values[ZOOMPAN_VAR_PDURATION] = s->prev_nb_frames;
    s->var_values[ZOOMPAN_VAR_A]     = (double) in->width / in->height;
    s->var_values[ZOOMPAN_VAR_SAR]   = in->sample_aspect_ratio.num ?
        (double) in->sample_aspect_ratio.num / in->sample_aspect_ratio.den : 1;
    s->var_values[ZOOMPAN_VAR_DAR]   = s->var_values[ZOOMPAN_VAR_A] * s->var_values[ZOOMPAN_VAR_SAR];
    s->var_values[ZOOMPAN_VAR_HSUB]  = 1 << s->desc->log2_chroma_w;
    s->var_values[ZOOMPAN_VAR_VSUB]  = 1 << s->desc->log2_chroma_h;
	//storm
    if ((ret = av_expr_parse_and_eval(&ox, s->ox_expr_str,
                                      var_names, s->var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, NULL)) < 0)
        return ret;
    if ((ret = av_expr_parse_and_eval(&oy, s->oy_expr_str,
                                      var_names, s->var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, NULL)) < 0)
        return ret;
    s->var_values[ZOOMPAN_VAR_X]     = ox;
    s->var_values[ZOOMPAN_VAR_Y]     = oy;

    if ((ret = av_expr_parse_and_eval(&nb_frames, s->duration_expr_str,
                                      var_names, s->var_values,
                                      NULL, NULL, NULL, NULL, NULL, 0, NULL)) < 0) {
        av_frame_free(&in);
        return ret;
    }

    s->var_values[ZOOMPAN_VAR_DURATION] = s->nb_frames = nb_frames;
    //s->in = in;

    return 0;
}

//poll
int request_frame(/*AVCodecContext *ctx,*/ZPContext *s, AVFrame *in, int64_t frame_count_in, AVFrame* &out)
{
    //AVFrame *in = s->in;
    double zoom=-1, dx=-1, dy=-1;
    int ret = -1;

    if (in) {
        ret = output_single_frame(s, in, s->var_values, s->current_frame,
                                  frame_count_in, &zoom, &dx, &dy, out);
        if (ret < 0)
            goto fail;
    }

    if (s->current_frame >= s->nb_frames) {
        if (dx != -1)
            s->x = dx;
        if (dy != -1)
            s->y = dy;
        if (zoom != -1)
            s->prev_zoom = zoom;
        s->prev_nb_frames = s->nb_frames;
        s->nb_frames = 0;
        s->current_frame = 0;
        //av_frame_free(&s->in);
        s->finished = 1;
        ////ret = ff_request_frame(ctx->inputs[0]);
    }

fail:
    sws_freeContext(s->sws);
    s->sws = NULL;

    return ret;
}

av_cold void zoompan_uninit(ZPContext *s)
{
    if(s && s->sws)
    {
        sws_freeContext(s->sws);
        s->sws = NULL;
    }
}
