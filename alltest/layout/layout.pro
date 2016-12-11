QT += widgets

HEADERS     = mainwindow.h \
    flowlayout.h
SOURCES     = mainwindow.cpp \
    flowlayout.cpp \
              main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/charactermap
INSTALLS += target
