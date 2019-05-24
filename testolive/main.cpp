#include "mainwindow.h"
#include <QApplication>

#include "debug.h"
#include "project/effect.h"

extern "C" {
	#include <libavformat/avformat.h>
	#include <libavfilter/avfilter.h>
}

//storm
extern "C" {
#include "libavutil/log.h"
}
#include <QFile>
#include <QMutex>
#include <QDebug>
#include <QDateTime>
#define LOG_FILE "log.log"
QFile outFile(LOG_FILE);
void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & str);
void my_log_callback(void *ptr, int level, const char *fmt, va_list vargs)
{
    vprintf(fmt, vargs);
    ///if(level == AV_LOG_ERROR)
        qInfo()<<"my_log_callback fmt: "<<fmt<<vargs;
}
//end storm

int main(int argc, char *argv[]) {
	//if (use_internal_logger) 
    {
		//qInstallMessageHandler(debug_message_handler); //storm
        outFile.open(QIODevice::WriteOnly );
        QTextStream ts(&outFile);
        //ts << txt << endl;
        ts << "start" << endl;
        outFile.close();

        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        qInstallMessageHandler(myMessageHandler);
	}

	// init ffmpeg subsystem
	av_register_all();
	avfilter_register_all();

	QApplication a(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);//storm

    
	//MainWindow w(nullptr, appName);
	MainWindow w(nullptr);

		//w.showFullScreen();
		w.showMaximized();
    //storm
    //av_log_set_level(AV_LOG_ERROR);
    //av_log_set_callback(my_log_callback);
    //end storm

	return a.exec();
}

QMutex mutexLog;
void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & str)
{
#if 1
    QMutexLocker locker(&mutexLog);
    if(outFile.exists())
    {
        printf("exist\n");
    }
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
