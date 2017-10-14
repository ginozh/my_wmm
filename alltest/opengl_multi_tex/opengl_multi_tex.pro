QT += core widgets

contains(QT_CONFIG, opengles.|angle|dynamicgl):error("This example requires Qt to be configured with -opengl desktop")

CONFIG += c++11

TARGET = myopengl
CONFIG += myopengl
CONFIG -= app_bundle

QMAKE_CFLAGS += -static-libgcc -static-libstdc++ -ggdb
QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++ -ggdb

TEMPLATE = app

LIBS += -lglew32 -lglfw3 -lopengl32

QMAKE_CFLAGS += -DOUTPUT_WASTE
QMAKE_CXXFLAGS += -DOUTPUT_WASTE
INCLUDEPATH += c:/shareproject/MovieMakeAdvance/libeffect/src

HEADERS += c:/shareproject/MovieMakeAdvance/libeffect/src/comm.h
SOURCES += main.cpp \
    c:/shareproject/MovieMakeAdvance/libeffect/src/comm.cpp
