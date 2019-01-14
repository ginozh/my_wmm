QT += widgets quick

# To make threaded gl check...
QT += core-private gui-private

HEADERS += threadrenderer.h \
    mainwindow.h\


SOURCES += threadrenderer.cpp main.cpp \
    mainwindow.cpp \


INCLUDEPATH += ../shared
HEADERS += ../shared/logorenderer.h
SOURCES += ../shared/logorenderer.cpp

RESOURCES += textureinthread.qrc

OTHER_FILES += \
    main.qml \
    error.qml
