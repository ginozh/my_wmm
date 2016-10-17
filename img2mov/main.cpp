#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QTime>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & str)
{
    const char * msg = str.toStdString().c_str();
#else
void myMessageHandler(QtMsgType type, const char *msg)
{
#endif
    QString txt;
	switch (type) {
		case QtDebugMsg:
			txt = QString("Debug: %1\n").arg(msg);
            //qDebug("%s", msg);
			break;
		case QtWarningMsg:
			txt = QString("Warning: %1\n").arg(msg);
            //qWarning("%s", msg);
			break;
		case QtInfoMsg:
			txt = QString("Info: %1\n").arg(msg);
            //qInfo("%s", msg);
			break;
		case QtCriticalMsg:
			txt = QString("Critical: %1\n").arg(msg);
            //qCritical("%s", msg);
			break;
		case QtFatalMsg:
			txt = QString("Fatal: %1\n").arg(msg);
            //qFatal("%s", msg);
			break;
	}
#if 1
    QFile outFile("log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
#endif
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
#if 1
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    qInstallMessageHandler(myMessageHandler);
#else
    qInstallMsgHandler(myMessageHandler);
#endif
#endif
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    MainWindow mainWindow;
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mainWindow);
    mainWindow.resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
    //mainWindow.show();
    mainWindow.showMaximized();

    return app.exec();
}
