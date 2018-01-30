#include <QApplication>
#include <QMainWindow>
#include <QtCore>
#include <QDebug>
#include <QString>
#include <QObject> 
#include <QThread> 
#include <QFile>
#include <QLabel>
#include "MMComm.h"

#define LOG_FILE "log.log"
QFile outFile(LOG_FILE);

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & str);
int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    QApplication a(argc, argv);
    outFile.open(QIODevice::WriteOnly );
    QTextStream ts(&outFile);
    //ts << txt << endl;
    ts << "start" << endl;
    outFile.close();

    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    //qInstallMessageHandler(myMessageHandler);

    //test opengl
    int glw=1280, glh=720;
    QImage image1, image2;
    {
    QImage& image=image1;
    //QString fileName="c:\\shareproject\\jpg\\4.jpg";
    QString fileName="c:\\shareproject\\jpg\\512img003.jpg";
    image.load(fileName);
    if (image.isNull()) {
        qDebug()<<"error";
        return -1;
    }
    image = image.convertToFormat(QImage::Format_RGBA8888);
    }
    {
    QImage& image=image2;
    QString fileName="c:\\shareproject\\jpg\\5.jpg";
    image.load(fileName);
    if (image.isNull()) {
        qDebug()<<"error";
        return -1;
    }
    image = image.convertToFormat(QImage::Format_RGBA8888);
    }
    QImage mask(glw, glh,QImage::Format_RGBA8888);
    unsigned char *mask_pixels = (unsigned char *) mask.bits();
    {

    QString m_effectName="Comm", m_effectName2="WS_OLD_PHOTO";
    int64_t curr_effect_number=15;
    int64_t total_effect_number=30;
    //GenericShaderContext* gs=MMGlobalContext::instance()->m_gs; //storm
    //glfwMakeContextCurrent( gs->threadWin );
    //MMGlobalContext::instance()->initialOpengl(image1.width(), image1.height());
    MMGlobalContext::instance()->initialOpengl(glw, glh);
    ////MMGlobalContext::instance()->createBindFramebufferTexture(image1.width(), image1.height());

    if(MMGlobalContext::instance()->ParseConfCreateProgram("./",m_effectName)==0
            && MMGlobalContext::instance()->ParseConfCreateProgram("./",m_effectName2)==0)
    {
        QTime startTime = QTime::currentTime();
        unsigned char *pixels = (unsigned char *) image1.bits();
        unsigned char *pixels2 = (unsigned char *) image2.bits();
        //if(!MMGlobalContext::instance()->fragRenderForOtherThread(m_effectName, pixels
        //            , curr_effect_number, total_effect_number, pixels2))
        if(!MMGlobalContext::instance()->fragRenderForOtherThread(m_effectName, pixels
                    , image1.width(), image1.height()
                    , curr_effect_number, total_effect_number))
        {
            int dt = startTime.msecsTo(QTime::currentTime());
            qDebug()<< "opengl waste: " << dt;
            glFlush();
            glReadPixels(0, 0, glw, glh, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)mask_pixels);
            mask.save(QString("%1.jpg").arg(curr_effect_number));
        }
        else
        {
            qInfo()<<"Transition::GetEffectFrame error. fragRenderForOtherThread is wrong ";
        }
    }
    else
    {
        qInfo()<<"Transition::GetEffectFrame error ParseConfCreateProgram";
    }

    }
#if 0
    QLabel *lbl;
    QPixmap *pix;
    pix = new QPixmap();
    pix->loadFromData(mask.bits(), mask.byteCount(), QImage::Format_RGBA8888);
    //pix = QPixmap::fromImage(mask);
    QMainWindow mainWindow;
    lbl = new QLabel;
    lbl->setPixmap(*pix);
    mainWindow.setCentralWidget(lbl);
    mainWindow.show();
#endif
    QLabel label;
    label.setPixmap(QPixmap::fromImage(mask));
    label.show();

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
