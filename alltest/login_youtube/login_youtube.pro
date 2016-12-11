QT += widgets multimedia multimediawidgets gui testlib webkitwidgets
INCLUDEPATH += /c/qtproject/alltest/qt-oauth-lib/
LIBS += -L /c/qtproject/alltest/qt-oauth-lib/debug/
LIBS += -lqt-oauth-lib

HEADERS     = mainwindow.h
SOURCES     = mainwindow.cpp \
              main.cpp

