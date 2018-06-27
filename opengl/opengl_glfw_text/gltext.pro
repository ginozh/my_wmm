TARGET = gltext
TEMPLATE = app
QT += widgets multimedia multimediawidgets
QT += core widgets
#CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11
CONFIG +=debug_and_release 

QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-switch -fopenmp
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-switch -fopenmp

#INCLUDEPATH += ../SimpleOpenGLImageLibrary/src
INCLUDEPATH += c:/msys64/mingw64/include/freetype2/ 

LIBS += -lglew32 -lglfw3 -lopengl32 -lfreetype -lfontconfig

SOURCES +=  \
    markup.cpp \
    distance-field.c \
    edtaa3func.c \
    font-manager.c \
    platform.c \
    text-buffer.c \
    texture-atlas.c \
    texture-font.c \
    utf8-utils.c \
    vector.c \
    vertex-attribute.c \
    vertex-buffer.c \
    shader.c \
    mat4.c \
    screenshot-util.c \


HEADERS  +=  \
    distance-field.h \
    edtaa3func.h \
    font-manager.h \
    freetype-gl.h \
    markup.h \
    opengl.h \
    platform.h \
    text-buffer.h \
    texture-atlas.h \
    texture-font.h \
    utf8-utils.h \
    vec234.h \
    vector.h \
    vertex-attribute.h \
    vertex-buffer.h \
    shader.h \
    mat4.h \
    screenshot-util.h \

