TEMPLATE = lib
MODULE_INCNAME = QtAV # for mac framework. also used in install_sdk.pro
TARGET = QtAV
QT += core gui
      CONFIG *= config_opengl
        CONFIG *= config_openglwindow
CONFIG *= qtav-buildlib
INCLUDEPATH += $$[QT_INSTALL_HEADERS] # TODO: ffmpeg dir

CONFIG(debug, debug|release): DEFINES += DEBUG
#release: DEFINES += QT_NO_DEBUG_OUTPUT
#var with '_' can not pass to pri?
PROJECTROOT = $$PWD/..
include(../.qmake.cache)
#!include(libQtAV.pri): error("could not find libQtAV.pri")

NAME = QtAV
PROJECT_TARGETNAME = $$qtLibraryTarget($$NAME)

isEmpty(PROJECTROOT): PROJECTROOT = $$PWD/..
include($${PROJECTROOT}/common.pri)
CONFIG += depend_includepath #?

PROJECT_SRCPATH = $$PWD
PROJECT_LIBDIR = $$qtLongName($$BUILD_DIR/lib)
INCLUDEPATH *= $$PROJECT_SRCPATH $$PROJECT_SRCPATH/.. $$PROJECT_SRCPATH/$$NAME
DEPENDPATH *= $$PROJECT_SRCPATH

STATICLINK = 0

!CONFIG(plugin) {
    #TEMPLATE = lib
    VERSION = $$LIB_VERSION
    DESTDIR= $$PROJECT_LIBDIR
}
TARGET = $$PROJECT_TARGETNAME ##I commented out this before, why?
CONFIG *= create_prl #
DEFINES += BUILD_$$upper($$NAME)_LIB #win32-msvc*
CONFIG *= shared #shared includes dll

shared {
    !CONFIG(plugin) {
        !isEqual(DESTDIR, $$BUILD_DIR/bin): DLLDESTDIR = $$BUILD_DIR/bin #copy shared lib there
    }
    CONFIG(release, debug|release): !isEmpty(QMAKE_STRIP):!mac_framework: QMAKE_POST_LINK = -$$QMAKE_STRIP $$PROJECT_LIBDIR/$$qtSharedLib($$NAME)
}


DESTDIR = C:/qtproject/QtAV/src/../lib_win_x86_64
#CONFIG *= shared #shared includes dll
DLLDESTDIR = C:/qtproject/QtAV/src/../out/bin

# end libqtav.pri


RESOURCES += QtAV.qrc \
    shaders/shaders.qrc

RC_ICONS = QtAV.ico
QMAKE_TARGET_COMPANY = "wbsecg1@gmail.com"
QMAKE_TARGET_DESCRIPTION = "QtAV Multimedia framework. http://qtav.org"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2012-2019 WangBin, wbsecg1@gmail.com"
QMAKE_TARGET_PRODUCT = "QtAV"

OTHER_FILES += $$RC_FILE QtAV.svg
TRANSLATIONS = i18n/QtAV_zh_CN.ts i18n/QtAV.ts

win32 {
# cross build, old vc etc.
  !config_dx: INCLUDEPATH += $$PROJECTROOT/contrib/dxsdk
}

config_gl|config_opengl {
  contains(QT_CONFIG, opengl):!contains(QT_CONFIG, opengles2): CONFIG *= enable_desktopgl
}
#UINT64_C: C99 math features, need -D__STDC_CONSTANT_MACROS in CXXFLAGS
DEFINES += __STDC_CONSTANT_MACROS
win32: {
  HEADERS += output/audio/xaudio2_compat.h
  SOURCES += output/audio/AudioOutputXAudio2.cpp
  DEFINES *= QTAV_HAVE_XAUDIO2=1
  winrt {
    LIBS += -lxaudio2 #only for xbox or >=win8
  } else {
    LIBS += -lole32 #CoInitializeEx for vs2008, but can not find the symbol at runtime
  }
}
CONFIG += config_cuda
#CONFIG += config_cuda_link
config_cuda {
    DEFINES += QTAV_HAVE_CUDA=1
    HEADERS += cuda/dllapi/nv_inc.h cuda/helper_cuda.h
    SOURCES += codec/video/VideoDecoderCUDA.cpp
    #contains(QT_CONFIG, opengl) {
      HEADERS += codec/video/SurfaceInteropCUDA.h
      SOURCES += codec/video/SurfaceInteropCUDA.cpp
    #}
    INCLUDEPATH += $$PWD/cuda cuda/dllapi
    config_dllapi:config_dllapi_cuda {
        DEFINES += QTAV_HAVE_DLLAPI_CUDA=1
        INCLUDEPATH += ../depends/dllapi/src
include(../depends/dllapi/src/libdllapi.pri)
        SOURCES += cuda/dllapi/cuda.cpp cuda/dllapi/nvcuvid.cpp cuda/dllapi/cuviddec.cpp
    } else:config_cuda_link {
        DEFINES += CUDA_LINK
        INCLUDEPATH += $$(CUDA_PATH)/include
        LIBS += -L$$(CUDA_PATH)/lib
        contains(TARGET_ARCH, x86): LIBS += -L$$(CUDA_PATH)/lib/Win32
        else: LIBS += -L$$(CUDA_PATH)/lib/x64
        LIBS += -lnvcuvid -lcuda
    }
    SOURCES += cuda/cuda_api.cpp
    HEADERS += cuda/cuda_api.h
}
config_d3d11va {
  CONFIG *= d3dva c++11
  DEFINES *= QTAV_HAVE_D3D11VA=1
  SOURCES += codec/video/VideoDecoderD3D11.cpp
  HEADERS += directx/SurfaceInteropD3D11.h
  SOURCES += directx/SurfaceInteropD3D11.cpp
  HEADERS += directx/D3D11VP.h
  SOURCES += directx/D3D11VP.cpp
  enable_egl {
    SOURCES += directx/SurfaceInteropD3D11EGL.cpp
  }
  enable_desktopgl {
    SOURCES += directx/SurfaceInteropD3D11GL.cpp
  }
  winrt: LIBS *= -ld3d11
}
win32:!winrt {
  HEADERS += directx/SurfaceInteropD3D9.h
  SOURCES += directx/SurfaceInteropD3D9.cpp
  enable_egl {
    SOURCES += directx/SurfaceInteropD3D9EGL.cpp
  }
  enable_desktopgl {
    SOURCES += directx/SurfaceInteropD3D9GL.cpp
  }
}
config_dxva {
  CONFIG *= d3dva
  DEFINES *= QTAV_HAVE_DXVA=1
  SOURCES += codec/video/VideoDecoderDXVA.cpp
  LIBS += -lole32
}
d3dva {
  HEADERS += codec/video/VideoDecoderD3D.h
  SOURCES += codec/video/VideoDecoderD3D.cpp
}

config_gl|config_opengl {
  contains(QT_CONFIG, egl) {
    DEFINES *= QTAV_HAVE_QT_EGL=1
#if a platform plugin depends on egl (for example, eglfs), egl is defined
  }
  OTHER_FILES += shaders/planar.f.glsl shaders/rgb.f.glsl
  SDK_HEADERS *= \
    QtAV/Geometry.h \
    QtAV/GeometryRenderer.h \
    QtAV/GLSLFilter.h \
    QtAV/OpenGLRendererBase.h \
    QtAV/OpenGLTypes.h \
    QtAV/OpenGLVideo.h \
    QtAV/ConvolutionShader.h \
    QtAV/VideoShaderObject.h \
    QtAV/VideoShader.h
  SDK_PRIVATE_HEADERS = \
    QtAV/private/OpenGLRendererBase_p.h
  HEADERS *= \
    opengl/gl_api.h \
    opengl/OpenGLHelper.h \
    opengl/SubImagesGeometry.h \
    opengl/SubImagesRenderer.h \
    opengl/ShaderManager.h
  SOURCES *= \
    filter/GLSLFilter.cpp \
    output/video/OpenGLRendererBase.cpp \
    opengl/gl_api.cpp \
    opengl/OpenGLTypes.cpp \
    opengl/Geometry.cpp \
    opengl/GeometryRenderer.cpp \
    opengl/SubImagesGeometry.cpp \
    opengl/SubImagesRenderer.cpp \
    opengl/OpenGLVideo.cpp \
    opengl/VideoShaderObject.cpp \
    opengl/VideoShader.cpp \
    opengl/ShaderManager.cpp \
    opengl/ConvolutionShader.cpp \
    opengl/OpenGLHelper.cpp
}
config_openglwindow {
  SDK_HEADERS *= QtAV/OpenGLWindowRenderer.h
  SOURCES *= output/video/OpenGLWindowRenderer.cpp
}
# mac is -FQTDIR we need -LQTDIR
LIBS *= -L$$[QT_INSTALL_LIBS] -lavcodec -lavformat -lswscale -lavutil
win32 {
  HEADERS *= utils/DirectXHelper.h
  SOURCES *= utils/DirectXHelper.cpp
#dynamicgl: __impl__GetDC __impl_ReleaseDC __impl_GetDesktopWindow
  !winrt:LIBS += -luser32
}
# compat with old system
# use old libva.so to link against
SOURCES += \
    AVCompat.cpp \
    QtAV_Global.cpp \
    subtitle/SubImage.cpp \
    subtitle/CharsetDetector.cpp \
    subtitle/PlainText.cpp \
    subtitle/PlayerSubtitle.cpp \
    subtitle/Subtitle.cpp \
    subtitle/SubtitleProcessor.cpp \
    subtitle/SubtitleProcessorFFmpeg.cpp \
    utils/GPUMemCopy.cpp \
    utils/Logger.cpp \
    AudioThread.cpp \
    utils/internal.cpp \
    AVThread.cpp \
    AudioFormat.cpp \
    AudioFrame.cpp \
    AudioResampler.cpp \
    AudioResamplerTemplate.cpp \
    codec/audio/AudioDecoder.cpp \
    codec/audio/AudioDecoderFFmpeg.cpp \
    codec/audio/AudioEncoder.cpp \
    codec/audio/AudioEncoderFFmpeg.cpp \
    codec/AVDecoder.cpp \
    codec/AVEncoder.cpp \
    AVMuxer.cpp \
    AVDemuxer.cpp \
    AVDemuxThread.cpp \
    ColorTransform.cpp \
    Frame.cpp \
    FrameReader.cpp \
    filter/Filter.cpp \
    filter/FilterContext.cpp \
    filter/FilterManager.cpp \
    filter/LibAVFilter.cpp \
    filter/SubtitleFilter.cpp \
    filter/EncodeFilter.cpp \
    ImageConverter.cpp \
    ImageConverterFF.cpp \
    Packet.cpp \
    PacketBuffer.cpp \
    AVError.cpp \
    AVPlayer.cpp \
    AVPlayerPrivate.cpp \
    AVTranscoder.cpp \
    AVClock.cpp \
    VideoCapture.cpp \
    VideoFormat.cpp \
    VideoFrame.cpp \
    io/MediaIO.cpp \
    io/QIODeviceIO.cpp \
    output/audio/AudioOutput.cpp \
    output/audio/AudioOutputBackend.cpp \
    output/audio/AudioOutputNull.cpp \
    output/video/VideoRenderer.cpp \
    output/video/VideoOutput.cpp \
    output/video/QPainterRenderer.cpp \
    output/AVOutput.cpp \
    output/OutputSet.cpp \
    Statistics.cpp \
    codec/video/VideoDecoder.cpp \
    codec/video/VideoDecoderFFmpegBase.cpp \
    codec/video/VideoDecoderFFmpeg.cpp \
    codec/video/VideoDecoderFFmpegHW.cpp \
    codec/video/VideoEncoder.cpp \
    codec/video/VideoEncoderFFmpeg.cpp \
    VideoThread.cpp \
    VideoFrameExtractor.cpp

SDK_HEADERS *= \
    QtAV/QtAV \
    QtAV/QtAV.h \
    QtAV/dptr.h \
    QtAV/QtAV_Global.h \
    QtAV/AudioResampler.h \
    QtAV/AudioDecoder.h \
    QtAV/AudioEncoder.h \
    QtAV/AudioFormat.h \
    QtAV/AudioFrame.h \
    QtAV/AudioOutput.h \
    QtAV/AVDecoder.h \
    QtAV/AVEncoder.h \
    QtAV/AVDemuxer.h \
    QtAV/AVMuxer.h \
    QtAV/Filter.h \
    QtAV/FilterContext.h \
    QtAV/LibAVFilter.h \
    QtAV/EncodeFilter.h \
    QtAV/Frame.h \
    QtAV/FrameReader.h \
    QtAV/QPainterRenderer.h \
    QtAV/Packet.h \
    QtAV/AVError.h \
    QtAV/AVPlayer.h \
    QtAV/AVTranscoder.h \
    QtAV/VideoCapture.h \
    QtAV/VideoRenderer.h \
    QtAV/VideoOutput.h \
    QtAV/MediaIO.h \
    QtAV/AVOutput.h \
    QtAV/AVClock.h \
    QtAV/VideoDecoder.h \
    QtAV/VideoEncoder.h \
    QtAV/VideoFormat.h \
    QtAV/VideoFrame.h \
    QtAV/VideoFrameExtractor.h \
    QtAV/FactoryDefine.h \
    QtAV/Statistics.h \
    QtAV/SubImage.h \
    QtAV/Subtitle.h \
    QtAV/SubtitleFilter.h \
    QtAV/SurfaceInterop.h \
    QtAV/version.h

SDK_PRIVATE_HEADERS *= \
    QtAV/private/factory.h \
    QtAV/private/mkid.h \
    QtAV/private/prepost.h \
    QtAV/private/singleton.h \
    QtAV/private/PlayerSubtitle.h \
    QtAV/private/SubtitleProcessor.h \
    QtAV/private/AVCompat.h \
    QtAV/private/AudioOutputBackend.h \
    QtAV/private/AudioResampler_p.h \
    QtAV/private/AVDecoder_p.h \
    QtAV/private/AVEncoder_p.h \
    QtAV/private/MediaIO_p.h \
    QtAV/private/AVOutput_p.h \
    QtAV/private/Filter_p.h \
    QtAV/private/Frame_p.h \
    QtAV/private/VideoShader_p.h \
    QtAV/private/VideoRenderer_p.h \
    QtAV/private/QPainterRenderer_p.h

# QtAV/private/* may be used by developers to extend QtAV features without changing QtAV library
# headers not in QtAV/ and its subdirs are used only by QtAV internally
HEADERS *= \
    $$SDK_HEADERS \
    $$SDK_PRIVATE_HEADERS \
    AVPlayerPrivate.h \
    AVDemuxThread.h \
    AVThread.h \
    AVThread_p.h \
    AudioThread.h \
    PacketBuffer.h \
    VideoThread.h \
    ImageConverter.h \
    ImageConverter_p.h \
    codec/video/VideoDecoderFFmpegBase.h \
    codec/video/VideoDecoderFFmpegHW.h \
    codec/video/VideoDecoderFFmpegHW_p.h \
    filter/FilterManager.h \
    subtitle/CharsetDetector.h \
    subtitle/PlainText.h \
    utils/BlockingQueue.h \
    utils/GPUMemCopy.h \
    utils/Logger.h \
    utils/SharedPtr.h \
    utils/ring.h \
    utils/internal.h \
    output/OutputSet.h \
    ColorTransform.h
# from mkspecs/features/qt_module.prf

MODULE_INCNAME = QtAV
MODULE_VERSION = $$VERSION
#use Qt version. limited by qmake
# windows: Qt5AV.dll, not Qt1AV.dll
!mac_framework: MODULE_VERSION = $${QT_MAJOR_VERSION}.$${QT_MINOR_VERSION}.$${QT_PATCH_VERSION}
!contains(QMAKE_HOST.os, Windows):include($$PROJECTROOT/deploy.pri)
