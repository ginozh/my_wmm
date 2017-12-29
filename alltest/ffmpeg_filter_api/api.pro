TARGET = ffmpeg_api
TEMPLATE = app
QT += core gui

CONFIG += c++11

TARGET = console
CONFIG += console
CONFIG -= app_bundle

QMAKE_CFLAGS += -static-libgcc -static-libstdc++ -ggdb -Wno-unused-parameter -Wno-switch
QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++ -ggdb -Wno-unused-parameter -Wno-switch

INCLUDEPATH += /home/gino/ffmpeg-3.3

#LIBS += -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale  -lavresample 
LIBS += -lavcodec -lavdevice -lavformat -lavutil -lpostproc -lswresample -lswscale  -lavresample 
#LIBS += c:/shareproject/ffmpeg-3.3/libavfilter/avfilter-6.dll
LIBS += -lavfilter

SOURCES += main.cpp \
    filter_crop.cpp \
    filter_pad.cpp \
    image_avframe.cpp \
    filter_overlay.cpp \
    filter_zoompan.cpp \
    filter_blend.cpp \
    filter_colorchannelmixer.cpp \
    filter_eq.cpp \
    filter_rotate.cpp \
    filter_hflip.cpp \
    filter_vflip.cpp \


HEADERS  += filter_crop.h \
    filter_pad.h \
    image_avframe.h \
    filter_overlay.h \
    filter_zoompan.h \
    filter_blend.h \
    filter_eq.h \
    filter_rotate.h \
    filter_hflip.h \
    filter_vflip.h \


