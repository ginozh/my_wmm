QT       += core gui widgets
OBJECTS_DIR = .obj
MOC_DIR = .moc
CONFIG -= app_bundle

TARGET = opengl_qtobject
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
    mainwidget.cpp \
    MMComm.cpp \


HEADERS += \
    mainwidget.h \
    MMComm.h \

