/***********************************************************************
 * File : main.cpp
 * Brief: 
 * 
 * History
 * ---------------------------------------------------------------------
 * 2017-12-30     storm   1.0    created
 * 
 ***********************************************************************
 */
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
#include <time.h>
#include <stdio.h>
#include <wchar.h>
#include <chrono>
#include <ratio>
#include <string>
#include <sstream>
#include <mutex>
#include <util/bmem.h>
#include <util/dstr.hpp>
#include <util/platform.h>
#include <util/profiler.hpp>
#include <util/cf-parser.h>
#include <obs-config.h>
#include <obs.hpp>

#include <QGuiApplication>
#include <QProxyStyle>
#include <QScreen>
#include <QProcess>

#include "platform.hpp"

#include <fstream>


#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#include <pthread.h>
#endif

#include <iostream>

using namespace std;
#undef main

#define LOG_FILE "log.log"
QFile outFile(LOG_FILE);

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & str);
static const char *run_program_init = "run_program_init";
static const int cx = 800;
static const int cy = 600;
int main(int argc, char *argv[])
{
	profiler_start(); // ./libobs/util/profiler.c
	profile_register_root(run_program_init, 0); //./libobs/util/profiler.c
	QCoreApplication::addLibraryPath(".");

    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    //QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
    //QCoreApplication a(argc, argv);
    QApplication a(argc, argv);
#if 0
    //mac 下报错
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    if (QCoreApplication::arguments().contains(QStringLiteral("--multisample")))
        fmt.setSamples(4);
    if (QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"))) {
        fmt.setVersion(3, 2);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    }
    QSurfaceFormat::setDefaultFormat(fmt);
#endif
    outFile.open(QIODevice::WriteOnly );
    QTextStream ts(&outFile);
    //ts << txt << endl;
    ts << "start" << endl;
    outFile.close();

    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    qInstallMessageHandler(myMessageHandler);

    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");


    MainWindow mainWindow;
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mainWindow);
    mainWindow.resize(availableGeometry.width() , availableGeometry.height() );
    //mainWindow.playerWidget->resize(6*availableGeometry.width() / 8, 6*availableGeometry.height() / 8);
    mainWindow.showMaximized();
    //mainWindow.show();
#if 0
	if (!obs_startup("en", nullptr, nullptr)) // storm
		throw "Couldn't create OBS";
	struct obs_video_info ovi;
	ovi.adapter = 0;
	ovi.fps_num = 30000;
	ovi.fps_den = 1001;
	ovi.graphics_module = DL_OPENGL;
	ovi.output_format = VIDEO_FORMAT_RGBA;
	ovi.base_width = cx;
	ovi.base_height = cy;
	ovi.output_width = cx;
	ovi.output_height = cy;

	if (obs_reset_video(&ovi) != 0)
		throw "Couldn't initialize video";
#endif
    int result = a.exec();
    ////CoUninitialize();
    return result;
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
