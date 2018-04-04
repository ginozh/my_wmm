QT           += widgets gui
QT           += opengl
INCLUDEPATH += c:/msys32/mingw32/include/SDL2/ 

CONFIG(debug, debug|release) {
    QMAKE_CFLAGS += -DOUTPUT_WASTE
    QMAKE_CXXFLAGS += -DOUTPUT_WASTE
}else{
    #QMAKE_CFLAGS += -DQT_NO_DEBUG_OUTPUT
    #QMAKE_CXXFLAGS += -DQT_NO_DEBUG_OUTPUT
    QMAKE_CFLAGS += -DOUTPUT_WASTE
    QMAKE_CXXFLAGS += -DOUTPUT_WASTE
}

RESOURCES  = effects.qrc

HEADERS       = mainwindow.h \
                GLDisplayWidget.h \
                GLHiddenWidget.h \
                PlayerPrivate.h \
                MMComm.h \
                GLWidget.h \


SOURCES       = main.cpp \
                mainwindow.cpp \
                GLDisplayWidget.cpp \
                GLHiddenWidget.cpp \
                PlayerPrivate.cpp \
                MMComm.cpp \
                GLWidget.cpp \

