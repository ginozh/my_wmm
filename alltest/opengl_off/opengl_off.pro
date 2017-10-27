#QT += quick


# To make threaded gl check...
#QT += core-private gui-private
QT += core gui widgets

CONFIG += c++11
CONFIG += opengl_off 
CONFIG -= app_bundle
TARGET = opengl_off 

HEADERS += fragrenderer.h
#HEADERS += logorenderer.h
SOURCES += fragrenderer.cpp main.cpp
#SOURCES += logorenderer.cpp main.cpp

#RESOURCES += textureinthread.qrc

#target.path = $$[QT_INSTALL_EXAMPLES]/quick/scenegraph/textureinthread
#INSTALLS += target
TEMPLATE = app

#OTHER_FILES += \
#    main.qml \
#    error.qml
