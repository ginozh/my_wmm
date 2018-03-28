TARGET = effectvideo
TEMPLATE = app
QT += widgets multimedia multimediawidgets
QT += core widgets
#CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11
CONFIG +=debug_and_release 

QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-switch -fopenmp
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-switch -fopenmp

LIBS += -lglew32 -lglfw3 -lopengl32
SOURCES += MMComm.cpp \
    main.cpp \
    slider.cpp \

HEADERS  += MMComm.h \
    mainwindow.h \
    slider.h \

