#QT += quick


# To make threaded gl check...
#QT += core-private gui-private
QT += core gui widgets

CONFIG += c++11
CONFIG += opengl_qt_multex
CONFIG -= app_bundle
TARGET = opengl_qt_multex

CONFIG(debug, debug|release) {
    QMAKE_CFLAGS += -DOUTPUT_WASTE
    QMAKE_CXXFLAGS += -DOUTPUT_WASTE
}

#HEADERS += logorenderer.h
SOURCES += main.cpp

#RESOURCES += textureinthread.qrc

#target.path = $$[QT_INSTALL_EXAMPLES]/quick/scenegraph/textureinthread
#INSTALLS += target
TEMPLATE = app

#OTHER_FILES += \
#    main.qml \
#    error.qml
