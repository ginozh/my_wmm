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
    window-basic-main-outputs.cpp \
    audio-encoders.cpp \
    double-slider.cpp \
    slider-ignorewheel.cpp \
    spinbox-ignorewheel.cpp \
    combobox-ignorewheel.cpp \
    properties-view.cpp \
    vertical-scroll-area.cpp \


HEADERS  += \
    mainwindow.h \
    platform.hpp \
    RecordDialog.hpp \
    display-helpers.hpp \
    qt-display.hpp \
    qt-wrappers.hpp \
    window-basic-main-outputs.hpp \
    audio-encoders.hpp \
    double-slider.hpp \
    slider-ignorewheel.hpp \
    spinbox-ignorewheel.hpp \
    combobox-ignorewheel.hpp \
    properties-view.hpp \
    properties-view.moc.hpp \
    vertical-scroll-area.hpp \


