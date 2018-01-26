QT += core gui opengl widgets

contains(QT_CONFIG, opengles.|angle|dynamicgl):error("This example requires Qt to be configured with -opengl desktop")

CONFIG += c++11

TARGET = opengl
CONFIG += opengl
CONFIG -= app_bundle

QMAKE_CFLAGS += -static-libgcc -static-libstdc++ -ggdb
QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++ -ggdb

TEMPLATE = app
SOURCES += main.cpp
