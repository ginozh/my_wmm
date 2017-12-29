#include "filter_vflip.h"
#include <QDebug>
#define qInfo qDebug
av_cold int vflip_init(VFlipContext *s)
{
    memset(s, 0, sizeof(VFlipContext));

    return 0;
}

int vflip_config(VFlipContext *flip, AVFrame* in)
{
    const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get((AVPixelFormat)in->format);

    flip->vsub = desc->log2_chroma_h;

    return 0;
}

int vflip_filter_frame(VFlipContext *flip, AVFrame *in, AVFrame* &out)
{
/*
var width=src.width,height=src.height,data=src.data,out=[];
			for(var i=0;i<height;i++){
				for(var j=0;j<width;j++){
					out[(i*width+j)*4]=data[((height-1-i)*width+j*4)];
					out[(i*width+j)*4+1]=data[((height-1-i)*width+j*4)+1];
					out[(i*width+j)*4+2]=data[((height-1-i)*width+j*4)+2];
					out[(i*width+j)*4+3]=data[((height-1-i)*width+j*4)+3];
					}
				}
			}
*/
    int width=out->width;
	int height=out->height;
    //uint8_t* src=in->data[0];
    //uint8_t* dst=out->data[0];
    const uint8_t *srcori = in->data[0];
    uint8_t* dstrow = out->data[0];
    for(int i=0;i<height;i++){
        uint8_t *dst = dstrow;
        for(int j=0;j<width*4;j+=4){
#if 0
            dst[(i*width+j)*4]=src[((height-1-i)*width+j*4)];
            dst[(i*width+j)*4+1]=src[((height-1-i)*width+j*4)+1];
            dst[(i*width+j)*4+2]=src[((height-1-i)*width+j*4)+2];
            dst[(i*width+j)*4+3]=src[((height-1-i)*width+j*4)+3];
#endif
            dst[j]=srcori[(height-1-i)*width*4+j];//src[((height-1-i)*width+j*4)];
            dst[j+1]=srcori[(height-1-i)*width*4+j+1];//src[((height-1-i)*width+j*4)+1];
            dst[j+2]=srcori[(height-1-i)*width*4+j+2];//src[((height-1-i)*width+j*4)+2];
            dst[j+3]=srcori[(height-1-i)*width*4+j+3];//src[((height-1-i)*width+j*4)+3];
        }
        dstrow += out->linesize[0];
    }
#if 0

    int i;
    for (i = 0; i < 4; i ++) {
        int vsub = i == 1 || i == 2 ? flip->vsub : 0;
        int height = AV_CEIL_RSHIFT(out->height, vsub);

        if (out->data[i]) {
            out->data[i] += (height - 1) * in->linesize[i];
            out->linesize[i] = -in->linesize[i];
        }
    }
#endif

    return 0;
}
