#include <QtCore/QThread>

#include <QGuiApplication>
#include <QApplication>
#include <QDesktopWidget>

#include <QtGui/private/qguiapplication_p.h>
#include <QtGui/qpa/qplatformintegration.h>

#include <QtQuick/QQuickView>

#include <QDebug>
#include <QString>
#include <QFile>
#include <QThread> 

#include "mainwindow.h"
#include "threadrenderer.h"

#define LOG_FILE "log.log"
QFile outFile(LOG_FILE);

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & str);
int main(int argc, char **argv)
{
    qputenv("QT_LOGGING_RULES", "qt.qpa.gl=true");
    //qputenv("QT_OPENGL", "angle");
    ///qputenv("QT_ANGLE_PLATFORM", "d3d9");
    ///QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    ///QGuiApplication app(argc, argv); //error
    QApplication app(argc, argv);

    outFile.open(QIODevice::WriteOnly );
    QTextStream ts(&outFile);
    //ts << txt << endl;
    ts << "start" << endl;
    outFile.close();

    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    qInstallMessageHandler(myMessageHandler);

    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");
#if 1
    if (!QGuiApplicationPrivate::platform_integration->hasCapability(QPlatformIntegration::ThreadedOpenGL)) {
        QQuickView view;
        view.setSource(QUrl("qrc:///error.qml"));
        view.show();
        return app.exec();
    }
#endif
    MainWindow mainWindow;
    //const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mainWindow);
    //mainWindow.resize(availableGeometry.width() , availableGeometry.height() );
    mainWindow.resize(640, 360);
    //mainWindow.playerWidget->resize(6*availableGeometry.width() / 8, 6*availableGeometry.height() / 8);
    //mainWindow.showMaximized();
    mainWindow.show();

    return app.exec();
#if 0

    qmlRegisterType<ThreadRenderer>("SceneGraphRendering", 1, 0, "Renderer");
    int execReturn = 0;

    {
        QQuickView view;

        // Rendering in a thread introduces a slightly more complicated cleanup
        // so we ensure that no cleanup of graphics resources happen until the
        // application is shutting down.
        view.setPersistentOpenGLContext(true);
        view.setPersistentSceneGraph(true);

        view.setResizeMode(QQuickView::SizeRootObjectToView);
        view.setSource(QUrl("qrc:///scenegraph/textureinthread/main.qml"));
        view.show();

        execReturn = app.exec();
    }


#endif
    // As the render threads make use of our QGuiApplication object
    // to clean up gracefully, wait for them to finish before
    // QGuiApp is taken off the heap.
    for (QThread *t : qAsConst(ThreadRenderer::threads)) {
        t->wait();
        delete t;
    }
    //return execReturn;
    return 0;
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
