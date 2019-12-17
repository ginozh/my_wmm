#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFile>
#include <QMutex>
#include <QDateTime>
#include <QThread>
#include <QTextStream>

#include "glwidget.h"
#include "mainwindow.h"

#define LOG_FILE "log.log"
QFile outFile(LOG_FILE);

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & str);
int main(int argc, char *argv[])
{
    outFile.open(QIODevice::WriteOnly );
    QTextStream ts(&outFile);
    //ts << txt << endl;
    ts << "start" << endl;
    outFile.close();

    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    qInstallMessageHandler(myMessageHandler);
    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");

    QApplication app(argc, argv);
#if 0
    QCoreApplication::setApplicationName("Qt Hello GL 2 Example");
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption multipleSampleOption("multisample", "Multisampling");
    parser.addOption(multipleSampleOption);
    QCommandLineOption coreProfileOption("coreprofile", "Use core profile");
    parser.addOption(coreProfileOption);
    QCommandLineOption transparentOption("transparent", "Transparent window");
    parser.addOption(transparentOption);

    parser.process(app);
#endif
#if 0
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    if (parser.isSet(multipleSampleOption))
        fmt.setSamples(4);
    if (parser.isSet(coreProfileOption)) {
        fmt.setVersion(3, 2);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    }
    QSurfaceFormat::setDefaultFormat(fmt);
#endif
    MainWindow mainWindow;
#if 0
    GLWidget::setTransparent(parser.isSet(transparentOption));
    if (GLWidget::isTransparent()) {
        mainWindow.setAttribute(Qt::WA_TranslucentBackground);
        mainWindow.setAttribute(Qt::WA_NoSystemBackground, false);
    }
    mainWindow.resize(mainWindow.sizeHint());
    int desktopArea = QApplication::desktop()->width() *
                     QApplication::desktop()->height();
    int widgetArea = mainWindow.width() * mainWindow.height();
    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        mainWindow.show();
    else
        mainWindow.showMaximized();
#endif
    mainWindow.show();
    return app.exec();
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
    ////ts << str;
    //qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");
#if 1
    ts << "[" << QDateTime::currentDateTime().toString("yyyyMMdd h:mm:ss.zzz")<<"]"
        //<<context.function<<" "
        << (type==QtDebugMsg?"Debug ":(type==QtWarningMsg?"Warning ":(type==QtInfoMsg?"Info ":"Critical ")))
        <<QThread::currentThreadId()<<" "
        <<context.line<<" "
        <<str << endl;
#endif
    //outFile.close();
#endif
}
