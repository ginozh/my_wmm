TARGET = qt_multi_qopenglwidget_thread
TEMPLATE = app
QT += widgets multimedia multimediawidgets qml core gui widgets
QT += opengl
CONFIG -= app_bundle
CONFIG += c++11

DEFINES += __STDC_FORMAT_MACROS

QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS

LIBS += -lwsock32 -lWs2_32 -lSecur32 -lpsapi -lOLE32 -lopengl32

SRC_PREFIX = .

SOURCES += main.cpp \
    mainwindow.cpp \
    OpenGLWidget.cpp \
    renderthread.cpp \
    Clip.cpp \
    $$SRC_PREFIX/io/math.cpp \
    $$SRC_PREFIX/ui/collapsiblewidget.cpp \
    $$SRC_PREFIX/ui/checkboxex.cpp \
    $$SRC_PREFIX/ui/comboboxex.cpp \
    $$SRC_PREFIX/ui/labelslider.cpp \
    $$SRC_PREFIX/project/effect.cpp \
    $$SRC_PREFIX/project/effectloaders.cpp \
    $$SRC_PREFIX/project/effectfield.cpp \
    $$SRC_PREFIX/project/effectrow.cpp \
    $$SRC_PREFIX/project/effectgizmo.cpp \
    $$SRC_PREFIX/project/glkeyframe.cpp \
    $$SRC_PREFIX/Effects/internal/transformeffect.cpp \


HEADERS  += \
    mainwindow.h \
    OpenGLWidget.h \
    renderthread.h \
    Clip.h \
    $$SRC_PREFIX/io/math.h \
    $$SRC_PREFIX/ui/collapsiblewidget.h \
    $$SRC_PREFIX/ui/checkboxex.h \
    $$SRC_PREFIX/ui/comboboxex.h \
    $$SRC_PREFIX/ui/labelslider.h \
    $$SRC_PREFIX/project/effect.h \
    $$SRC_PREFIX/project/effectloaders.h \
    $$SRC_PREFIX/project/effectfield.h \
    $$SRC_PREFIX/project/effectrow.h \
    $$SRC_PREFIX/project/effectgizmo.h \
    $$SRC_PREFIX/project/glkeyframe.h \
    $$SRC_PREFIX/Effects/internal/transformeffect.h \
