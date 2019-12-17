QT += quick concurrent gui widgets
QT += opengl
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

########Release 模式下可以调试#############
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
CONFIG(debug, debug|release) {
    #QMAKE_CFLAGS += -DQT_NO_DEBUG_OUTPUT
    #QMAKE_CXXFLAGS += -DQT_NO_DEBUG_OUTPUT
}else{
    QMAKE_CFLAGS += -DQT_NO_DEBUG_OUTPUT
    QMAKE_CXXFLAGS += -DQT_NO_DEBUG_OUTPUT
}

SOURCES += main.cpp \
    hwdecoder.cpp \
    hwdecoderfactory.cpp \
    videorenderer.cpp \
    framerenderer.cpp \
    videosource.cpp \
    videoframe.cpp \
    surface.cpp \
    yuv2rgb.cpp \
    openglinterop.cpp \
    fileprocessor.cpp \
    glwidget.cpp \
    mainwindow.cpp \
    logo.cpp \


win32: SOURCES += d3d9decoder.cpp \
                  d3d9interop.cpp \
                  surfaced3d9.cpp

linux-g++: SOURCES += vaapidecoder.cpp \
                       surfacevaapi.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    hwdecoder.h \
    hwdecoderfactory.h \
    videorenderer.h \
    framerenderer.h \
    videosource.h \
    videoframe.h \
    surface.h \
    yuv2rgb.h \
    openglinterop.h \
    fileprocessor.h \
    glwidget.h \
    mainwindow.h \
    logo.h \


win32: HEADERS += d3d9decoder.h \
                  d3d9interop.h \
                  surfaced3d9.h

linux-g++: HEADERS += vaapidecoder.h \
                      surfacevaapi.h

#Link with FFmpeg installed in Qt
LIBS += -lavcodec -lavdevice -lavformat -lavutil

#Link with DX libs (Windows)
win32: LIBS += -ldxgi -ldxva2 -ld3d9

#Link with libva libs (LINUX)
linux-g++: LIBS += -lX11 -lva -lva-x11
