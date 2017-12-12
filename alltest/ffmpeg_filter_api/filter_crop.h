#ifndef FILTER_CROP_H
#define FILTER_CROP_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/eval.h>
#include <libavutil/pixdesc.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
}
enum crop_var_name {
    CROP_VAR_IN_W,  CROP_VAR_IW,
    CROP_VAR_IN_H,  CROP_VAR_IH,
    CROP_VAR_OUT_W, CROP_VAR_OW,
    CROP_VAR_OUT_H, CROP_VAR_OH,
    CROP_VAR_A,
    CROP_VAR_SAR,
    CROP_VAR_DAR,
    CROP_VAR_HSUB,
    CROP_VAR_VSUB,
    CROP_VAR_X,
    CROP_VAR_Y,
    CROP_VAR_N,
    CROP_VAR_POS,
    CROP_VAR_T,
    CROP_VAR_VARS_NB
};
typedef struct CropContext {
    const AVClass *class1;
    int  x;             ///< x offset of the non-cropped area with respect to the input area
    int  y;             ///< y offset of the non-cropped area with respect to the input area
    int  w;             ///< width of the cropped area
    int  h;             ///< height of the cropped area

    AVRational out_sar; ///< output sample aspect ratio
    int keep_aspect;    ///< keep display aspect ratio when cropping
    int exact;          ///< exact cropping, for subsampled formats

    int max_step[4];    ///< max pixel step for each plane, expressed as a number of bytes
    int hsub, vsub;     ///< chroma subsampling
    char *x_expr, *y_expr, *w_expr, *h_expr;
    AVExpr *x_pexpr, *y_pexpr;  /* parsed expressions for x and y */
    ///double *var_values;
    double var_values[CROP_VAR_VARS_NB];
    ///double var_values[20];
} CropContext;
void crop_init(CropContext* s, char* w, char*h, char* x, char* y, int keep_aspect, int exact);
int crop_config_input(AVCodecContext *c, CropContext* s);
void crop_filter_frame(AVCodecContext *c, CropContext* s, AVFrame* frame);
av_cold void crop_uninit(CropContext* s);
#endif
