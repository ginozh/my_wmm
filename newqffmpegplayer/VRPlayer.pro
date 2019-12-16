#-------------------------------------------------
#
# Project created by QtCreator 2017-06-28T15:23:07
#
#-------------------------------------------------

QT       += core gui multimedia opengl
#CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += console
CONFIG -= app_bundle

QMAKE_CFLAGS += -static-libgcc -static-libstdc++ -ggdb
QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++ -ggdb

TARGET = VRPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    FFmpegReader.cpp \
    PlayerPrivate.cpp \
    cmdutils.c \
    Timeline.cpp \
    Clip.cpp \


HEADERS  += mainwindow.h \
    FFmpegReader.h \
    PlayerPrivate.h \
    avversion.h \
    cmdutils.h \
    colorspace.h \
    Timeline.h \
    Clip.h \

FORMS    += mainwindow.ui

LIBS += -L$$PWD/ffmpeg/lib/ -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale  -lavresample \
        -L$$PWD/gl/lib/x64/ -lfreeglut -lglew32 #-lglew32s
LIBS += -lSDL
#LIBS += -lSDL2

INCLUDEPATH += $$PWD/ffmpeg/include/ \
                $$PWD/gl/include/ \
                c:/msys64/mingw64/include/SDL/

