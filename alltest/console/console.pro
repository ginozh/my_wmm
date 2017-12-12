QT += core gui

CONFIG += c++11

TARGET = console
CONFIG += console
CONFIG -= app_bundle

QMAKE_CFLAGS += -static-libgcc -static-libstdc++ -ggdb
QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++ -ggdb

TEMPLATE = app

INCLUDEPATH += c:/qtproject/level/ /c/qtproject/level/
LIBS += c:/qtproject/level/leveldb/libleveldb.a c:/qtproject/level/leveldb/libmemenv.a

windows{
LIBS+= -lshlwapi
}

SOURCES += main.cpp
