#-------------------------------------------------
#
# Project created by QtCreator 2016-06-23T21:22:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Img2Movie
TEMPLATE = app
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

SOURCES += main.cpp\
        mainwindow.cpp \
    cmdutils.c \
    ffmpeg.c \
    ffmpeg_dxva2.c \
    ffmpeg_filter.c \
    ffmpeg_opt.c

HEADERS  += mainwindow.h \
    ffmpeg.h \
    config.h \
    cmdutils_common_opts.h \
    cmdutils.h

FORMS    += mainwindow.ui
