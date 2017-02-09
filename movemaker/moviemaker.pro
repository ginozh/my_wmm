TARGET = moviemaker
TEMPLATE = app
QT += widgets multimedia multimediawidgets
CONFIG -= app_bundle
CONFIG += c++11

#CONFIG += precompiled_header
#PRECOMPILED_HEADER = stable.h
#QMAKE_CXXFLAGS += /MP

INCLUDEPATH += C:\qtproject\ffmpeg-3.0.2
#QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS -static-libgcc -static-libstdc++ -ggdb
QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS
#QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS -static-libgcc -static-libstdc++ -ggdb
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS
DEFINES += __STDC_FORMAT_MACROS
#LIBS += -L C:\qtproject\ffmpeg-3.0.2
LIBS += -lavcodec
LIBS += -lavdevice
LIBS += -lavfilter
LIBS += -lavformat
LIBS += -lavutil
LIBS += -lpostproc
LIBS += -lswresample
LIBS += -lswscale
LIBS += -lavresample
LIBS += -lole32 #ffmpeg_dxva2.c CoTaskMemFree

SOURCES += main.cpp \
    mainwindow.cpp \
    ffmpeg.c \
    ffmpeg_filter.c \
    ffmpeg_opt.c \
    cmdutils.c \
    ffmpeg_dxva2.c \
    flowlayout.cpp \
    mediasedit.cpp \
    image.cpp \
    element.cpp \
    themes.cpp \
    videoplayer.cpp \
    slider.cpp \
    musics.cpp \
    preview.cpp \
    themeslist.cpp \
    save.cpp \
    format.cpp \
    device.cpp \
    comm.cpp

HEADERS  += mainwindow.h \
    ffmpeg.h \
    cmdutils.h \
    cmdutils_common_opts.h \
    config.h  \
    flowlayout.h  \
    mediasedit.h  \
    image.h \
    element.h \
    themes.h \
    videoplayer.h \
    slider.h \
    musics.h \
    preview.h \
    themeslist.h \
    save.h \
    format.h \
    device.h \
    comm.h
