TARGET = img2mov
TEMPLATE = app
QT += widgets multimedia multimediawidgets
CONFIG -= app_bundle
CONFIG += c++11

#CONFIG += precompiled_header
#PRECOMPILED_HEADER = stable.h
#QMAKE_CXXFLAGS += /MP

INCLUDEPATH += C:\qtproject\ffmpeg-3.0.2
#QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS -static-libgcc -static-libstdc++ -ggdb
QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS -fpermissive
#QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS -static-libgcc -static-libstdc++ -ggdb
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS -fpermissive
DEFINES += __STDC_FORMAT_MACROS
#LIBS += -L C:\qtproject\ffmpeg-3.0.2
LIBS += -lavcodec
LIBS += -lavdevice
LIBS += -lavfilter
LIBS += -lavformat
LIBS += -lavutil
LIBS += -lpostproc
LIBS += -lswresample
LIBS += -lswscale
LIBS += -lavresample
LIBS += -lole32 #ffmpeg_dxva2.c CoTaskMemFree

SOURCES += main.cpp \
    mainwindow.cpp \
    ffmpeg.c \
    ffmpeg_filter.c \
    ffmpeg_opt.c \
    cmdutils.c \
    ffmpeg_dxva2.c \
    videoplayer.cpp \
    flowlayout.cpp \
    elementsedit.cpp \
    element.cpp \
    image.cpp \
    slider.cpp \
    menu.cpp \
    tabwidget.cpp \
    animation.cpp \
    videoscene.cpp \
    videotextitem.cpp \
    lineedit.cpp \
    videorectitem.cpp \
    musiclabel.cpp \
    combobox.cpp \
    comm.cpp \
    norwegianwoodstyle.cpp

HEADERS  += mainwindow.h \
    ffmpeg.h \
    cmdutils.h \
    cmdutils_common_opts.h \
    config.h \
    stable.h \
    videoplayer.h \
    flowlayout.h \
    elementsedit.h \
    element.h \
    image.h \
    slider.h \
    menu.h \
    tabwidget.h \
    animation.h \
    videoscene.h \
    videotextitem.h \
    lineedit.h \
    comm.h \
    videorectitem.h \
    musiclabel.h \
    combobox.h \
    norwegianwoodstyle.h
