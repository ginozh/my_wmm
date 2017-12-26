#ifndef FILTER_COLORCHANNELMIXER_H
#define FILTER_COLORCHANNELMIXER_H
extern "C" {
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>

#include <libavfilter/avfilter.h>
#include <libavfilter/drawutils.h>
#include <libavfilter/internal.h>
#include <libavfilter/formats.h>
#include <libavfilter/video.h>
}

typedef struct {
    const AVClass *class1;
    double rr, rg, rb, ra;
    double gr, gg, gb, ga;
    double br, bg, bb, ba;
    double ar, ag, ab, aa;

    int *lut[4][4];

    int *buffer;

    uint8_t rgba_map[4];
} ColorChannelMixerContext;

av_cold int colorchannelmixer_init(ColorChannelMixerContext *s);
int colorchannelmixer_set_rgb(ColorChannelMixerContext* s, double rr=0, double rg=0, double rb=0, double ra=0, double gr=0, double gg=0, double gb=0, double ga=0, double br=0, double bg=0, double bb=0, double ba=0);
int colorchannelmixer_config_output(ColorChannelMixerContext *s, int format);
int colorchannelmixer_filter_frame(ColorChannelMixerContext *s, AVFrame *in, AVFrame* &out);
av_cold void colorchannelmixer_uninit(ColorChannelMixerContext *s);

#endif
