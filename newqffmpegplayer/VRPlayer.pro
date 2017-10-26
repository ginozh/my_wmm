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
    videothread.cpp \
    cmdutils.c \


HEADERS  += mainwindow.h \
    videothread.h \
    avversion.h \
    cmdutils.h \
    colorspace.h \

FORMS    += mainwindow.ui

LIBS += -L$$PWD/ffmpeg/lib/ -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale  -lavresample \
        -L$$PWD/gl/lib/x64/ -lfreeglut -lglew32 #-lglew32s
LIBS += -lSDL

INCLUDEPATH += $$PWD/ffmpeg/include/ \
                $$PWD/gl/include/ \
                c:/msys32/mingw32/include/SDL/

