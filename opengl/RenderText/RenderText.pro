TARGET = rendertext
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
INCLUDEPATH += ../freetype-gl/windows/freetype/ ../LearnOpenGL/includes/

LIBS += -lglew32 -lglfw3 -lopengl32 -lfreetype

SOURCES += main.cpp \

HEADERS  +=  Shader.h  \

