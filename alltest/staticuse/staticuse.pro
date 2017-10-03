QT += core
QT -= gui

CONFIG += c++11

TARGET = staticuse
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../staticlib/release/ -lstaticlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../staticlib/debug/ -lstaticlib
else:unix: LIBS += -L$$PWD/../staticlib/ -lstaticlib

INCLUDEPATH += $$PWD/../staticlib
DEPENDPATH += $$PWD/../staticlib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../staticlib/release/libstaticlib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../staticlib/debug/libstaticlib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../staticlib/release/staticlib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../staticlib/debug/staticlib.lib
else:unix: PRE_TARGETDEPS += $$PWD/../staticlib/libstaticlib.a
