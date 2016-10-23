#include "mainwidget.h"
#include "waveform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Qt Multimedia spectrum analyzer");

    //MainWidget w;
    Waveform w;
    w.show();
    //w.showMaximized();

    return app.exec();
}
