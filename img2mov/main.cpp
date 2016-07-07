#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    MainWindow mainWindow;
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mainWindow);
    mainWindow.resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
    mainWindow.show();

    return app.exec();
}
