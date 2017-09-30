#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T16:38:32
#
#-------------------------------------------------

QT       -= gui

TARGET = staticlib
TEMPLATE = lib
#CONFIG += static
QMAKE_CFLAGS += -static
QMAKE_CXXFLAGS += -static

INCLUDEPATH += /c/shareproject/effect_video/lib_effectvideo/header


SOURCES += staticlib.cpp

HEADERS += staticlib.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/ -lavcodec $$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/libavformat.a -lavutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/ /c/shareproject/ffmpeg-3.3/install/lib/libavformat.a /c/shareproject/ffmpeg-3.3/install/lib/libavcodec.a /c/shareproject/ffmpeg-3.3/install/lib/libavutil.a /c/shareproject/ffmpeg-3.3/install/lib//libswscale.a -lcelt0 -lgsm -liconv -llzma -lmp3lame -lopencore-amrnb -lopencore-amrwb -lopus -lschroedinger-1.0-0 -lorc-0.4-0 -lspeex -ltheoradec -ltheoraenc -logg -lvorbis -lvorbisenc -lvpx -lwavpack -lx264 -lx265 -lcaca -lopenal -lass -lfontconfig -lexpat -lfreetype -lbz2 -lharfbuzz -lintl -lpcre -lgraphite2 -lpng16 -lfribidi -lopencv_core320 -lopencv_imgproc320 -lbluray -lxml2 -lmodplug -lrtmp -lgmp -lgnutls -lhogweed -lnettle -lidn -lp11-kit -lffi -ltasn1 -lwsock32 -lWs2_32 -lz -lSecur32
else:unix: LIBS += -L$$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/ -lavcodec $$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/libavformat.a -lavutil

INCLUDEPATH += $$PWD/../../../shareproject/effect_video/lib_effectvideo/header
DEPENDPATH += $$PWD/../../../shareproject/effect_video/lib_effectvideo/header

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/libavcodec.a $$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/libavformat.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/libavcodec.a $$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/libavformat.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/avcodec.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/avcodec.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/libavcodec.a $$PWD/../../../shareproject/effect_video/lib_effectvideo/lib/libavformat.a
