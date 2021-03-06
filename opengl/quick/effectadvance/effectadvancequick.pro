QT += widgets quick opengl

# To make threaded gl check...
QT += core-private gui-private

HEADERS += threadrenderer.h \
    mainwindow.h \
    textbox.h \
    dynamicentrymodel.h \
    documenthandler.h \


SOURCES += threadrenderer.cpp main.cpp \
    mainwindow.cpp \
    textbox.cpp \
    dynamicentrymodel.cpp \
    documenthandler.cpp \


LIBS += -lopengl32

INCLUDEPATH += ../shared
HEADERS += ../shared/logorenderer.h
SOURCES += ../shared/logorenderer.cpp

RESOURCES += textureinthread.qrc

OTHER_FILES += \
    main.qml \
    error.qml
