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
# libQtAVd.dll.a


NAME = QtAV
PROJECT_TARGETNAME = $$qtLibraryTarget($$NAME) # important debug: libQtAVd.dll.a

#isEmpty(PROJECTROOT): PROJECTROOT = $$PWD/..
#include($${PROJECTROOT}/common.pri) #
#CONFIG += depend_includepath #?

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
TARGET = $$PROJECT_TARGETNAME ##
CONFIG *= create_prl #
DEFINES += BUILD_$$upper($$NAME)_LIB #win32-msvc* #导出函数
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

win32 {
# cross build, old vc etc.
  !config_dx: INCLUDEPATH += $$PROJECTROOT/contrib/dxsdk
}

config_gl|config_opengl {
  contains(QT_CONFIG, opengl):!contains(QT_CONFIG, opengles2): CONFIG *= enable_desktopgl
}
#UINT64_C: C99 math features, need -D__STDC_CONSTANT_MACROS in CXXFLAGS
DEFINES += __STDC_CONSTANT_MACROS

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
  HEADERS *= \
    opengl/gl_api.h \
    opengl/OpenGLHelper.h \

  SOURCES *= \
    opengl/gl_api.cpp \
    opengl/OpenGLHelper.cpp
}

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
    codec/AVDecoder.cpp \
    utils/internal.cpp \
    AVError.cpp \
    utils/Logger.cpp \
    utils/GPUMemCopy.cpp \
    Frame.cpp \
    codec/video/VideoDecoder.cpp \
    codec/video/VideoDecoderFFmpegBase.cpp \
    codec/video/VideoDecoderFFmpeg.cpp \
    codec/video/VideoDecoderFFmpegHW.cpp \
    VideoFormat.cpp \
    VideoFrame.cpp \
    ImageConverter.cpp \
    ImageConverterFF.cpp \
    Packet.cpp \
    AVDemuxer.cpp \
    io/MediaIO.cpp \
    io/QIODeviceIO.cpp \
    FrameReader.cpp \


SDK_HEADERS *= \
    QtAV/AVError.h \
    QtAV/QtAV_Global.h \
    QtAV/Frame.h \
    QtAV/VideoDecoder.h \
    QtAV/VideoFormat.h \
    QtAV/VideoFrame.h \
    QtAV/SurfaceInterop.h \
    QtAV/Packet.h \
    QtAV/FactoryDefine.h \

SDK_PRIVATE_HEADERS *= \
    QtAV/private/factory.h \
    QtAV/private/mkid.h \
    QtAV/private/singleton.h \
    QtAV/private/AVCompat.h \
    QtAV/private/Frame_p.h \
    QtAV/private/AVDecoder_p.h \
    QtAV/private/MediaIO_p.h \

HEADERS *= \
    QtAV/dptr.h \
    utils/internal.h \
    utils/Logger.h \
    utils/GPUMemCopy.h \
    codec/video/VideoDecoderFFmpegBase.h \
    codec/video/VideoDecoderFFmpegHW.h \
    codec/video/VideoDecoderFFmpegHW_p.h \
    ImageConverter.h \
    ImageConverter_p.h \
    utils/BlockingQueue.h \
    QtAV/AVDecoder.h \
    QtAV/AVDemuxer.h \
    QtAV/MediaIO.h \
    QtAV/FrameReader.h \
