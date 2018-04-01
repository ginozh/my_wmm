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
    QImage image1, image2;
    {
    QImage& image=image1;
    //QString fileName="c:\\shareproject\\jpg\\4.jpg";
    QString fileName="c:\\shareproject\\jpg\\img006.jpg";
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
    QImage mask(image1.width(), image1.height(),QImage::Format_RGBA8888);
    unsigned char *mask_pixels = (unsigned char *) mask.bits();
    {

    QString m_effectName="Comm";//"Cube";
    QString m_effectName2="WS_OLD_PHOTO";
    int64_t curr_effect_number=15;
    int64_t total_effect_number=30;
    //GenericShaderContext* gs=MMGlobalContext::instance()->m_gs; //storm
    //glfwMakeContextCurrent( gs->threadWin );
    MMGlobalContext::instance()->initialOpengl(image1.width(), image1.height());
    //MMGlobalContext::instance()->createBindFramebufferTexture(image1.width(), image1.height());

#if 0
    if(MMGlobalContext::instance()->ParseConfCreateProgram("./",m_effectName)==0
            && MMGlobalContext::instance()->ParseConfCreateProgram("./",m_effectName2)==0)
#endif
    {
        QTime startTime = QTime::currentTime();
        unsigned char *pixels = (unsigned char *) image1.bits();
        unsigned char *pixels2 = (unsigned char *) image2.bits();
#if 0
        float spos[12] = { 
            0.0f, 0.0f,
            0.0f, 720.0f,
            1280.0f, 0.0f,
            0.0f, 720.0f,
            0.0f, 0.0f,
            1280.0f, 720.0f
        };
#endif
#if 0
        float spos[12] = {
            -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f
                , -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
#endif
#if 1
        float spos[12] = { //1: leftbottom->rightbottom->lefttop; 2: lefttop->rightbottom->righttop
            0.0f, 720.0f, 1280.0f, 720.0f, 0.0f, 0.0f
                , 0.0f, 0.0f, 1280.0f, 720.0f, 1280.0f, 0.0f};
#endif
        GLuint texture=MMGlobalContext::load2DTexture(image1.width(), image1.height(), pixels);
        GLuint texture2=0;//MMGlobalContext::load2DTexture(image2.width(), image2.height(), pixels2);
#if 0
        if(!MMGlobalContext::instance()->fragRenderForOtherThread(m_effectName, spos, sizeof(spos), pixels
                    , curr_effect_number, total_effect_number, pixels2))
#endif
        if(!MMGlobalContext::instance()->fragRenderForOtherThread(m_effectName, spos, sizeof(spos), texture
                    , curr_effect_number, total_effect_number, texture2))
        {
            int dt = startTime.msecsTo(QTime::currentTime());
            qDebug()<< "opengl waste: " << dt;
            //glFlush();
            //glReadPixels(0, 0, image1.width(), image1.height(), GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)mask_pixels);
        }
        else
        {
            qInfo()<<"Transition::GetEffectFrame error. fragRenderForOtherThread is wrong ";
        }
    }
#if 0
    else
    {
        qInfo()<<"Transition::GetEffectFrame error ParseConfCreateProgram";
    }
#endif

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
    //label.setPixmap(QPixmap::fromImage(mask));
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
