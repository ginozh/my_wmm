TARGET = ffmpeg_api
TEMPLATE = app
QT += core gui

CONFIG += c++11

TARGET = console
CONFIG += console
CONFIG -= app_bundle

QMAKE_CFLAGS += -static-libgcc -static-libstdc++ -ggdb
QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++ -ggdb

LIBS += -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale  -lavresample 

SOURCES += main.cpp
