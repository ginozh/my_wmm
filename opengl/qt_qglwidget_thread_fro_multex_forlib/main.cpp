#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QtCore>
#include <QDebug>
#include <QString>
#include <QObject> 
#include <QThread> 
#include <QFile>
#include <QSurfaceFormat>
#include "mainwindow.h"

#undef main
#define LOG_FILE "log.log"
QFile outFile(LOG_FILE);

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & str);

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    if (QCoreApplication::arguments().contains(QStringLiteral("--multisample")))
        fmt.setSamples(4);
    if (QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"))) {
        fmt.setVersion(3, 2);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    }
    QSurfaceFormat::setDefaultFormat(fmt);

    outFile.open(QIODevice::WriteOnly );
    QTextStream ts(&outFile);
    //ts << txt << endl;
    ts << "start" << endl;
    outFile.close();

    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    qInstallMessageHandler(myMessageHandler);

    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");
	MainWindow w;
	w.showMaximized();
	//w.show();
	return a.exec();
}
QMutex mutexLog;
void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & str)
{
#if 1
    QMutexLocker locker(&mutexLog);
    ////QFile outFile(LOG_FILE);
    ////outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    //ts << txt << endl;
    //qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");
    ts << "[" << QDateTime::currentDateTime().toString("yyyyMMdd h:mm:ss.zzz")<<"]"
        //<<context.function<<" "
        << (type==QtDebugMsg?"Debug ":(type==QtWarningMsg?"Warning ":(type==QtInfoMsg?"Info ":"Critical ")))
        <<QThread::currentThreadId()<<" "
        <<context.line<<" "
        <<str << endl;
    //outFile.close();
#endif
}
