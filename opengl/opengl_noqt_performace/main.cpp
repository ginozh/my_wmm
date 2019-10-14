#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QtCore>
#include <QDebug>
#include <QString>
#include <QObject> 
#include <QThread> 
#include <QFile>
#include <QLabel>
#include <QVBoxLayout>
#include <QBoxLayout>
#include <QHBoxLayout>
#include "MMComm.h"
#include "mainwindow.h"

#define LOG_FILE "log.log"
QFile outFile(LOG_FILE);

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & str);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Movie Maker"));

    QWidget* centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout;

    lblImage=new QLabel;
    {
        mainLayout->addWidget(lblImage);
        lblImage->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        glw=1280, glh=720;
        {
            QImage& image=image1;
            QString fileName="c:\\shareproject\\jpg\\4.jpg";
            //QString fileName="c:\\shareproject\\jpg\\512img003.jpg";
            image.load(fileName);
            if (image.isNull()) {
                qDebug()<<"error";
                //return -1;
            }
            image = image.convertToFormat(QImage::Format_RGBA8888);
        }
        {
            QImage& image=image2;
            QString fileName="c:\\shareproject\\jpg\\5.jpg";
            image.load(fileName);
            if (image.isNull()) {
                qDebug()<<"error";
                //return -1;
            }
            image = image.convertToFormat(QImage::Format_RGBA8888);
        }
        mask=new QImage(glw, glh,QImage::Format_RGBA8888);
        {

            //m_effectName="Comm", m_effectName2="WS_OLD_PHOTO";
            m_effectName="Cube", m_effectName2="WS_OLD_PHOTO";
            curr_effect_number=0;
            total_effect_number=30;
            MMGlobalContext::instance()->initialOpengl(glw, glh);
            MMGlobalContext::instance()->createBindFramebufferTexture(glw, glh);

            if(MMGlobalContext::instance()->ParseConfCreateProgram("./",m_effectName)==0
                    && MMGlobalContext::instance()->ParseConfCreateProgram("./",m_effectName2)==0)
            {
                Render();
            }
            else
            {
                qInfo()<<"Transition::GetEffectFrame error ParseConfCreateProgram";
            }

        }
    }
    {
        QBoxLayout *controlLayout = new QHBoxLayout;
        mainLayout->addLayout(controlLayout);
        controlLayout->setAlignment(Qt::AlignTop);
        {
            positionSlider = new MYSlider(Qt::Horizontal);
            controlLayout->addWidget(positionSlider);

            positionSlider->setRange(0, total_effect_number);
            positionSlider->setValue(curr_effect_number);
            ////connect(positionSlider, SIGNAL(sliderMoved(int)),player , SLOT(seek(int)));
            //connect(positionSlider, SIGNAL(sliderPressed(int)),player , SLOT(seek(int)));
#if 1
            connect(positionSlider, &MYSlider::sliderMoved,
                    [=](int pos)
                    {
                    qDebug()<<"Slider::sliderMoved pos: "<<pos<<" ori_position: "<<curr_effect_number;
                    if(pos!=curr_effect_number)
                    {
                        curr_effect_number=pos;
                        //player->seek(pos);
                        Render();
                    }

                    });
#endif
        }
    }
    centralWidget->setLayout(mainLayout);
}
void MainWindow::Render()
{
    unsigned char* mask_pixels = (unsigned char *) mask->bits();
    QTime startTime = QTime::currentTime();
    unsigned char *pixels = (unsigned char *) image1.bits();
    unsigned char *pixels2 = (unsigned char *) image2.bits();
#if 0
    if(!MMGlobalContext::instance()->fragRenderForOtherThread(m_effectName, pixels
                , image1.width(), image1.height()
                , curr_effect_number, total_effect_number))
#endif
    /*if(!MMGlobalContext::instance()->fragRenderForOtherThread(m_effectName, pixels
                , curr_effect_number, total_effect_number, pixels2))*/
    if(!MMGlobalContext::instance()->fragRenderForOtherThread(m_effectName2, pixels))
    {
        int dt = startTime.msecsTo(QTime::currentTime());
        qDebug()<< "opengl waste: " << dt<<" curr_effect_number: "<<curr_effect_number;
        ///glFlush();
        glFinish();
        glReadPixels(0, 0, glw, glh, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)mask_pixels);
        mask->save(QString("%1.jpg").arg(curr_effect_number));
        lblImage->setPixmap(QPixmap::fromImage(*mask));
    }
    else
    {
        qInfo()<<"Transition::GetEffectFrame error. fragRenderForOtherThread is wrong ";
    }
}

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
    qInstallMessageHandler(myMessageHandler);


    MainWindow mainWindow;
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mainWindow);
    mainWindow.resize(7*availableGeometry.width() / 8, 7*availableGeometry.height() / 8);
    //mainWindow.playerWidget->resize(6*availableGeometry.width() / 8, 6*availableGeometry.height() / 8);
    mainWindow.show();

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
