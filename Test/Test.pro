#-------------------------------------------------
#
# Project created by QtCreator 2016-06-19T12:18:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Test
TEMPLATE = app
QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS
DEFINES += __STDC_FORMAT_MACROS
LIBS += -lavcodec
LIBS += -lavdevice
LIBS += -lavfilter
#LIBS += -lpostproc
LIBS += -lavformat

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
