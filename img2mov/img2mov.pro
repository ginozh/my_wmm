TARGET = img2mov
TEMPLATE = app
QT += widgets
CONFIG -= app_bundle
CONFIG += c++11

#CONFIG += precompiled_header
#PRECOMPILED_HEADER = stable.h
#QMAKE_CXXFLAGS += /MP

INCLUDEPATH += C:\QtProjects\ffmpeg-3.0.2
QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS
DEFINES += __STDC_FORMAT_MACROS
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
    ffmpeg_dxva2.c

HEADERS  += mainwindow.h \
    ffmpeg.h \
    cmdutils.h \
    cmdutils_common_opts.h \
    config.h \
    stable.h
