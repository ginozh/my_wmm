#ifndef FFMPEG_UTILITIES_H
#define FFMPEG_UTILITIES_H

// Required for libavformat to build on Windows
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

// Include the FFmpeg headers
#if 0
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavresample/avresample.h>
#include <libavutil/mathematics.h>
#include <libavutil/pixfmt.h>
#include <libavutil/pixdesc.h>

    // libavutil changed folders at some point
#if LIBAVFORMAT_VERSION_MAJOR >= 53
#include <libavutil/opt.h>
#else
#include <libavcodec/opt.h>
#endif

    // channel header refactored
#if LIBAVFORMAT_VERSION_MAJOR >= 54
#include <libavutil/channel_layout.h>
#endif
}
#endif
extern "C"{
#include "libavutil/avstring.h"
    //#include "libavutil/colorspace.h" //add ffmpeg
//#include "colorspace.h" //add ffmpeg
    //#include "libavutil/display.h" //add ffmpeg
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libavresample/avresample.h" //add ffmpeg
#include "libswresample/swresample.h" //add ffmpeg
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"

#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/display.h"
#include "libswscale/swscale.h"

#include "libavutil/audio_fifo.h"
#include "config.h"
#undef av_restrict
#define av_restrict __restrict__
#include "libavutil/float_dsp.h"

//#include "cmdutils.h"

#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_types.h>
#include <SDL_name.h>
#include <SDL_main.h>
#include <SDL_config.h>
}

// This was removed from newer versions of FFmpeg (but still used in libopenshot)
#ifndef AVCODEC_MAX_AUDIO_FRAME_SIZE
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio
#endif
#ifndef AV_ERROR_MAX_STRING_SIZE
#define AV_ERROR_MAX_STRING_SIZE 64
#endif
#ifndef AUDIO_PACKET_ENCODING_SIZE
#define AUDIO_PACKET_ENCODING_SIZE 768000		// 48khz * S16 (2 bytes) * max channels (8)
#endif

// This wraps an unsafe C macro to be C++ compatible function
static const std::string av_make_error_string(int /*errnum*/) //storm
{
    char errbuf[AV_ERROR_MAX_STRING_SIZE];
    ////av_strerror(errnum, errbuf, AV_ERROR_MAX_STRING_SIZE); //storm
    std::string errstring(errbuf);
    return errstring;
}

// Redefine the C macro to use our new C++ function
#undef av_err2str
#define av_err2str(errnum) av_make_error_string(errnum).c_str()

// Define this for compatibility
#ifndef PixelFormat
#define PixelFormat AVPixelFormat
#endif
#ifndef PIX_FMT_RGBA
#define PIX_FMT_RGBA AV_PIX_FMT_RGBA
#endif
#ifndef PIX_FMT_NONE
#define PIX_FMT_NONE AV_PIX_FMT_NONE
#endif
#ifndef PIX_FMT_RGB24
#define PIX_FMT_RGB24 AV_PIX_FMT_RGB24
#endif
#ifndef PIX_FMT_YUV420P
#define PIX_FMT_YUV420P AV_PIX_FMT_YUV420P
#endif

#if LIBAVFORMAT_VERSION_MAJOR >= 55
#define AV_ALLOCATE_FRAME() av_frame_alloc()
#define AV_RESET_FRAME(av_frame) av_frame_unref(av_frame)
#define AV_FREE_FRAME(av_frame) av_frame_free(av_frame)
#define AV_FREE_PACKET(av_packet) av_packet_unref(av_packet)
#else
#define AV_ALLOCATE_FRAME() avcodec_alloc_frame()
#define AV_RESET_FRAME(av_frame) avcodec_get_frame_defaults(av_frame)
#define AV_FREE_FRAME(av_frame) avcodec_free_frame(av_frame)
#define AV_FREE_PACKET(av_packet) av_free_packet(av_packet)
#endif


#endif
