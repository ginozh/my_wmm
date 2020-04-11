TARGET = myobs
TEMPLATE = app
QT += core gui widgets
QT += opengl
CONFIG -= app_bundle
CONFIG += c++11

#INCLUDEPATH += /usr/local/include/SDL2/ 
INCLUDEPATH += .
INCLUDEPATH += ../../libobs 

QMAKE_CFLAGS += -fobjc-arc
QMAKE_CXXFLAGS += -fobjc-arc

#QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-c++11-narrowing -Wno-reserved-user-defined-literal
#QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-c++11-narrowing -Wno-reserved-user-defined-literal
DEFINES += __STDC_FORMAT_MACROS

########Release 模式下可以调试#############
#QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
CONFIG+=debug_and_release 
CONFIG(debug, debug|release) {
} else {
    QMAKE_CFLAGS += -DQT_NO_DEBUG_OUTPUT
    QMAKE_CXXFLAGS += -DQT_NO_DEBUG_OUTPUT
}

#LIBS += -L../../build/rundir/DEBUG/bin/ -lobs.0
LIBS += ../../build/rundir/DEBUG/bin/libobs.0.dylib

SOURCES += main.cpp \
    mainwindow.cpp \
    RecordDialog.cpp \
    qt-display.cpp \
    qt-wrappers.cpp \


HEADERS  += \
    mainwindow.h \
    platform.hpp \
    RecordDialog.hpp \
    display-helpers.hpp \
    qt-display.hpp \
    qt-wrappers.hpp \


