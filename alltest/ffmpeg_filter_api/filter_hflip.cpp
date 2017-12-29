#include "filter_hflip.h"
#include <QDebug>
#define qInfo qDebug
av_cold int hflip_init(HFlipContext *s)
{
    memset(s, 0, sizeof(HFlipContext));

    return 0;
}

int hflip_config_props(HFlipContext *s, AVFrame* in)
{
    const AVPixFmtDescriptor *pix_desc = av_pix_fmt_desc_get((AVPixelFormat)in->format);
    const int hsub = pix_desc->log2_chroma_w;
    const int vsub = pix_desc->log2_chroma_h;

    av_image_fill_max_pixsteps(s->max_step, NULL, pix_desc);
    s->planewidth[0]  = s->planewidth[3]  = in->width;
    s->planewidth[1]  = s->planewidth[2]  = AV_CEIL_RSHIFT(in->width, hsub);
    s->planeheight[0] = s->planeheight[3] = in->height;
    s->planeheight[1] = s->planeheight[2] = AV_CEIL_RSHIFT(in->height, vsub);

    return 0;
}

typedef struct ThreadData {
    AVFrame *in, *out;
} ThreadData;

static int filter_slice(HFlipContext *s, ThreadData *td, int job, int nb_jobs)
{
    AVFrame *in = td->in;
    AVFrame *out = td->out;
    uint8_t *inrow, *outrow;
    int i, j, plane, step;

    for (plane = 0; plane < 4 && in->data[plane] && in->linesize[plane]; plane++) {
        const int width  = s->planewidth[plane];
        const int height = s->planeheight[plane];
        const int start = (height *  job   ) / nb_jobs;
        const int end   = (height * (job+1)) / nb_jobs;

        step = s->max_step[plane];

        outrow = out->data[plane] + start * out->linesize[plane];
        inrow  = in ->data[plane] + start * in->linesize[plane] + (width - 1) * step;
        for (i = start; i < end; i++) {
            switch (step) {
            case 1:
                for (j = 0; j < width; j++)
                    outrow[j] = inrow[-j];
            break;

            case 2:
            {
                uint16_t *outrow16 = (uint16_t *)outrow;
                uint16_t * inrow16 = (uint16_t *) inrow;
                for (j = 0; j < width; j++)
                    outrow16[j] = inrow16[-j];
            }
            break;

            case 3:
            {
                uint8_t *in  =  inrow;
                uint8_t *out = outrow;
                for (j = 0; j < width; j++, out += 3, in -= 3) {
                    int32_t v = AV_RB24(in);
                    AV_WB24(out, v);
                }
            }
            break;

            case 4:
            {
                uint32_t *outrow32 = (uint32_t *)outrow;
                uint32_t * inrow32 = (uint32_t *) inrow;
                for (j = 0; j < width; j++)
                    outrow32[j] = inrow32[-j];
            }
            break;

            default:
                for (j = 0; j < width; j++)
                    memcpy(outrow + j*step, inrow - j*step, step);
            }

            inrow  += in ->linesize[plane];
            outrow += out->linesize[plane];
        }
    }

    return 0;
}

int hflip_filter_frame(HFlipContext *s, AVFrame *in, AVFrame* &out)
{
    ThreadData td;
    /* copy palette if required */
    if (av_pix_fmt_desc_get((AVPixelFormat)in->format)->flags & AV_PIX_FMT_FLAG_PAL)
        memcpy(out->data[1], in->data[1], AVPALETTE_SIZE);

    td.in = in, td.out = out;

    filter_slice(s, &td, 0, 1);

    return 0;
}
