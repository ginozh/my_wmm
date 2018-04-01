#-------------------------------------------------
#
# Project created by QtCreator 2018-02-22T17:16:53
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES  = resource/qglwidget-multithread-example.qrc

LIBS += -lglew32 -lglfw3 -lopengl32

SOURCES += \
    src/opengl_quad.cpp \
    src/opengl_thread.cpp \
    src/opengl_widget.cpp \
    src/main.cpp \


HEADERS += \
    src/opengl.h \
    src/opengl_quad.h \
    src/opengl_thread.h \
    src/opengl_widget.h \

