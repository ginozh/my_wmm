TARGET = ffmpeg_api
TEMPLATE = app
QT += core gui

CONFIG += c++11

TARGET = console
CONFIG += console
CONFIG -= app_bundle

QMAKE_CFLAGS += -static-libgcc -static-libstdc++ -ggdb
QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++ -ggdb

INCLUDEPATH += c:/shareproject/ffmpeg-3.3

#LIBS += -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale  -lavresample 
LIBS += -lavcodec -lavdevice c:/shareproject/ffmpeg-3.3/libavfilter/avfilter-6.dll -lavformat -lavutil -lpostproc -lswresample -lswscale  -lavresample 

SOURCES += main.cpp \
    filter_crop.cpp \
    filter_pad.cpp \


HEADERS  += filter_crop.h \
    filter_pad.h \
