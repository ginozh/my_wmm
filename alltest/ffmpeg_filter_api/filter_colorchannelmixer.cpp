#include "filter_colorchannelmixer.h"

#define R 0
#define G 1
#define B 2
#define A 3

av_cold int colorchannelmixer_init(ColorChannelMixerContext *s)
{
    memset(s, 0, sizeof(ColorChannelMixerContext));
    s->rr = 1;
    s->gg = 1;
    s->bb = 1;
    s->aa = 1;
}
int colorchannelmixer_config_output(ColorChannelMixerContext *s, int format)
{
    int i, j, size, *buffer;
    ff_fill_rgba_map(s->rgba_map, (AVPixelFormat)format);

    switch ((AVPixelFormat)format) {
    case AV_PIX_FMT_RGB48:
    case AV_PIX_FMT_BGR48:
    case AV_PIX_FMT_RGBA64:
    case AV_PIX_FMT_BGRA64:
        size = 65536;
        break;
    default:
        size = 256;
    }

    s->buffer = buffer = (int*)av_malloc(16 * size * sizeof(*s->buffer));
    if (!s->buffer)
        return AVERROR(ENOMEM);

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++, buffer += size)
            s->lut[i][j] = buffer;

    for (i = 0; i < size; i++) {
        s->lut[R][R][i] = lrint(i * s->rr);
        s->lut[R][G][i] = lrint(i * s->rg);
        s->lut[R][B][i] = lrint(i * s->rb);
        s->lut[R][A][i] = lrint(i * s->ra);

        s->lut[G][R][i] = lrint(i * s->gr);
        s->lut[G][G][i] = lrint(i * s->gg);
        s->lut[G][B][i] = lrint(i * s->gb);
        s->lut[G][A][i] = lrint(i * s->ga);

        s->lut[B][R][i] = lrint(i * s->br);
        s->lut[B][G][i] = lrint(i * s->bg);
        s->lut[B][B][i] = lrint(i * s->bb);
        s->lut[B][A][i] = lrint(i * s->ba);

        s->lut[A][R][i] = lrint(i * s->ar);
        s->lut[A][G][i] = lrint(i * s->ag);
        s->lut[A][B][i] = lrint(i * s->ab);
        s->lut[A][A][i] = lrint(i * s->aa);
    }

    return 0;
}

int colorchannelmixer_set_rgb(ColorChannelMixerContext* s, double rr, double rg, double rb, double ra, double gr, double gg, double gb, double ga, double br, double bg, double bb, double ba)
{
    s->rr = rr;
    s->rg = rg;
    s->rb = rb;
    s->gr = gr;
    s->gg = gg;
    s->gb = gb;
    s->br = br;
    s->bg = bg;
    s->bb = bb;
    return 0;
}

int colorchannelmixer_filter_frame(ColorChannelMixerContext *s, AVFrame *in, AVFrame* &out)
{
    const uint8_t roffset = s->rgba_map[R];
    const uint8_t goffset = s->rgba_map[G];
    const uint8_t boffset = s->rgba_map[B];
    const uint8_t aoffset = s->rgba_map[A];
    const uint8_t *srcrow = in->data[0];
    uint8_t *dstrow;
    int i, j;

    dstrow = out->data[0];
    switch ((AVPixelFormat)in->format) {
    case AV_PIX_FMT_BGR24:
    case AV_PIX_FMT_RGB24:
        for (i = 0; i < in->height; i++) {
            const uint8_t *src = srcrow;
            uint8_t *dst = dstrow;

            for (j = 0; j < in->width * 3; j += 3) {
                const uint8_t rin = src[j + roffset];
                const uint8_t gin = src[j + goffset];
                const uint8_t bin = src[j + boffset];

                dst[j + roffset] = av_clip_uint8(s->lut[R][R][rin] +
                                                 s->lut[R][G][gin] +
                                                 s->lut[R][B][bin]);
                dst[j + goffset] = av_clip_uint8(s->lut[G][R][rin] +
                                                 s->lut[G][G][gin] +
                                                 s->lut[G][B][bin]);
                dst[j + boffset] = av_clip_uint8(s->lut[B][R][rin] +
                                                 s->lut[B][G][gin] +
                                                 s->lut[B][B][bin]);
            }

            srcrow += in->linesize[0];
            dstrow += out->linesize[0];
        }
        break;
    case AV_PIX_FMT_0BGR:
    case AV_PIX_FMT_0RGB:
    case AV_PIX_FMT_BGR0:
    case AV_PIX_FMT_RGB0:
        for (i = 0; i < in->height; i++) {
            const uint8_t *src = srcrow;
            uint8_t *dst = dstrow;

            for (j = 0; j < in->width * 4; j += 4) {
                const uint8_t rin = src[j + roffset];
                const uint8_t gin = src[j + goffset];
                const uint8_t bin = src[j + boffset];

                dst[j + roffset] = av_clip_uint8(s->lut[R][R][rin] +
                                                 s->lut[R][G][gin] +
                                                 s->lut[R][B][bin]);
                dst[j + goffset] = av_clip_uint8(s->lut[G][R][rin] +
                                                 s->lut[G][G][gin] +
                                                 s->lut[G][B][bin]);
                dst[j + boffset] = av_clip_uint8(s->lut[B][R][rin] +
                                                 s->lut[B][G][gin] +
                                                 s->lut[B][B][bin]);
                if (in != out)
                    dst[j + aoffset] = 0;
            }

            srcrow += in->linesize[0];
            dstrow += out->linesize[0];
        }
        break;
    case AV_PIX_FMT_ABGR:
    case AV_PIX_FMT_ARGB:
    case AV_PIX_FMT_BGRA:
    case AV_PIX_FMT_RGBA:
        for (i = 0; i < in->height; i++) {
            const uint8_t *src = srcrow;
            uint8_t *dst = dstrow;

            for (j = 0; j < in->width * 4; j += 4) {
                const uint8_t rin = src[j + roffset];
                const uint8_t gin = src[j + goffset];
                const uint8_t bin = src[j + boffset];
                const uint8_t ain = src[j + aoffset];

                dst[j + roffset] = av_clip_uint8(s->lut[R][R][rin] +
                                                 s->lut[R][G][gin] +
                                                 s->lut[R][B][bin] +
                                                 s->lut[R][A][ain]);
                dst[j + goffset] = av_clip_uint8(s->lut[G][R][rin] +
                                                 s->lut[G][G][gin] +
                                                 s->lut[G][B][bin] +
                                                 s->lut[G][A][ain]);
                dst[j + boffset] = av_clip_uint8(s->lut[B][R][rin] +
                                                 s->lut[B][G][gin] +
                                                 s->lut[B][B][bin] +
                                                 s->lut[B][A][ain]);
                dst[j + aoffset] = av_clip_uint8(s->lut[A][R][rin] +
                                                 s->lut[A][G][gin] +
                                                 s->lut[A][B][bin] +
                                                 s->lut[A][A][ain]);
            }

            srcrow += in->linesize[0];
            dstrow += out->linesize[0];
        }
        break;
    case AV_PIX_FMT_BGR48:
    case AV_PIX_FMT_RGB48:
        for (i = 0; i < in->height; i++) {
            const uint16_t *src = (const uint16_t *)srcrow;
            uint16_t *dst = (uint16_t *)dstrow;

            for (j = 0; j < in->width * 3; j += 3) {
                const uint16_t rin = src[j + roffset];
                const uint16_t gin = src[j + goffset];
                const uint16_t bin = src[j + boffset];

                dst[j + roffset] = av_clip_uint16(s->lut[R][R][rin] +
                                                  s->lut[R][G][gin] +
                                                  s->lut[R][B][bin]);
                dst[j + goffset] = av_clip_uint16(s->lut[G][R][rin] +
                                                  s->lut[G][G][gin] +
                                                  s->lut[G][B][bin]);
                dst[j + boffset] = av_clip_uint16(s->lut[B][R][rin] +
                                                  s->lut[B][G][gin] +
                                                  s->lut[B][B][bin]);
            }

            srcrow += in->linesize[0];
            dstrow += out->linesize[0];
        }
        break;
    case AV_PIX_FMT_BGRA64:
    case AV_PIX_FMT_RGBA64:
        for (i = 0; i < in->height; i++) {
            const uint16_t *src = (const uint16_t *)srcrow;
            uint16_t *dst = (uint16_t *)dstrow;

            for (j = 0; j < in->width * 4; j += 4) {
                const uint16_t rin = src[j + roffset];
                const uint16_t gin = src[j + goffset];
                const uint16_t bin = src[j + boffset];
                const uint16_t ain = src[j + aoffset];

                dst[j + roffset] = av_clip_uint16(s->lut[R][R][rin] +
                                                  s->lut[R][G][gin] +
                                                  s->lut[R][B][bin] +
                                                  s->lut[R][A][ain]);
                dst[j + goffset] = av_clip_uint16(s->lut[G][R][rin] +
                                                  s->lut[G][G][gin] +
                                                  s->lut[G][B][bin] +
                                                  s->lut[G][A][ain]);
                dst[j + boffset] = av_clip_uint16(s->lut[B][R][rin] +
                                                  s->lut[B][G][gin] +
                                                  s->lut[B][B][bin] +
                                                  s->lut[B][A][ain]);
                dst[j + aoffset] = av_clip_uint16(s->lut[A][R][rin] +
                                                  s->lut[A][G][gin] +
                                                  s->lut[A][B][bin] +
                                                  s->lut[A][A][ain]);
            }

            srcrow += in->linesize[0];
            dstrow += out->linesize[0];
        }
    }
    return 0;
}

av_cold void colorchannelmixer_uninit(ColorChannelMixerContext *s)
{
    av_freep(&s->buffer);
}
