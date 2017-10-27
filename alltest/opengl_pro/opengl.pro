#QT += core gui opengl widgets
QT += core widgets

contains(QT_CONFIG, opengles.|angle|dynamicgl):error("This example requires Qt to be configured with -opengl desktop")

CONFIG += c++11

TARGET = myopengl
CONFIG += myopengl
CONFIG -= app_bundle

QMAKE_CFLAGS += -static-libgcc -static-libstdc++ -ggdb
QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++ -ggdb

TEMPLATE = app

LIBS += -lglew32
LIBS += -lglfw3

QMAKE_CFLAGS += -DOUTPUT_WASTE
QMAKE_CXXFLAGS += -DOUTPUT_WASTE

#HEADERS += glextensions.h
SOURCES += main.cpp \

    #glextensions.cpp
