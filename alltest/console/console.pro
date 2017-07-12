QT += core gui

CONFIG += c++11

TARGET = console
CONFIG += console
CONFIG -= app_bundle

QMAKE_CFLAGS += -static-libgcc -static-libstdc++ -ggdb
QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++ -ggdb

TEMPLATE = app

SOURCES += main.cpp
