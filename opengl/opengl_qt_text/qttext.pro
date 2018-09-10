TARGET = qttext
TEMPLATE = app
QT += widgets multimedia multimediawidgets
QT += core opengl
CONFIG -= app_bundle
CONFIG += c++11
CONFIG +=debug_and_release 

QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-switch -fopenmp
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-switch -fopenmp

#INCLUDEPATH += ../SimpleOpenGLImageLibrary/src
INCLUDEPATH += c:/msys64/mingw64/include/freetype2/ 

LIBS += -lfreetype -lfontconfig

SOURCES +=  \
    main.cpp \
    vector.c \
    utf8-utils.c \
    edtaa3func.c \
    distance-field.c \
    texture-atlas.c \
    platform.c \
    texture-font.c \
    font-manager.c \
    mat4.c \


HEADERS  +=  \
    vector.h \
    utf8-utils.h \
    edtaa3func.h \
    distance-field.h \
    vec234.h \
    texture-atlas.h \
    platform.h \
    texture-font.h \
    markup.h \
    font-manager.h \
    mat4.h \
