#include <QVBoxLayout>
#include <QHBoxLayout>
//#include <QGLFormat>
#include <QSurfaceFormat>
#include <QLabel>
#include <QPushButton>
#include <QDockWidget>
#include <QDebug>
#include "mainwindow.h"
#include "playerwidget.h"
#include "glhiddenwidget.h"

void TestOpenCLThread::run(){m_parent->process();}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
#if 0
	QGLFormat qglFormat;
	qglFormat.setProfile(QGLFormat::CoreProfile);

	//create hidden QGLWidget
	m_hiddenGl=new GLHiddenWidget(qglFormat);
#endif
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);


    setWindowTitle(tr("Movie Maker"));

    QWidget* centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout;
#if 0
    playerWidget = new GLWidget(qglFormat, m_hiddenGl);
#endif
    //playerWidget = new GLWidget(m_hiddenGl);
    playerWidget = GLWidget::instance();//new GLWidget(m_hiddenGl);
    {
        mainLayout->addWidget(playerWidget);
    }
	//create hidden QGLWidget
	m_hiddenGl=new GLHiddenWidget(playerWidget);
	//m_hiddenGl->setVisible(false);
    {
#if 0
        //stackedLayout = new QDockWidget(this);
        stackedLayout = new QDockWidget(playerWidget);
        {
            stackedLayout->setWidget(m_hiddenGl);
        }
        //stackedLayout->showMinimized();
        //addDockWidget(Qt::NoDockWidgetArea, stackedLayout);
#endif
    }
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        mainLayout->addLayout(hbox);
        {
            QWidget* nullWidget = new QWidget;
            nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
            hbox->addWidget(nullWidget);
        }
        {
            QLabel* lbl = new QLabel(tr("filter: "));
            hbox->addWidget(lbl);
        }
        {
            QPushButton* m_pbLeftRotate = new QPushButton("to_thread");
            hbox->addWidget(m_pbLeftRotate);

            connect(m_pbLeftRotate, &QAbstractButton::clicked, 
                    [=]()
                    {
                    //stackedLayout->setVisible(false);
                    //move the hidden widget to the new thread id
                    m_hiddenGl->doneCurrent();
                    const QOpenGLContext * c = m_hiddenGl->context();
                    qDebug()<<"hidden c: "<<c;
                    if(c)
                    {
#if 0
                        m_sharecontext = new QOpenGLContext;
                        m_sharecontext->setShareContext(playerWidget->context());
                        m_sharecontext->create();
                        m_sharecontext->moveToThread(m_processThread);
                        m_processThread->start();
#endif
#if 1
                        m_hiddenGl->context()->moveToThread(m_processThread);
                        //m_hiddenGl->context()->shareContext()->moveToThread(m_processThread);
                        //m_hiddenGl->test(this);
                        //m_hiddenGl->test(this);
                        m_processThread->start();
                        //QThread::msleep(10*1000);
#endif
#if 0
                        m_hiddenGl->test(m_sharecontext); //create textureid;
                        playerWidget->update();
#endif
                    }
                    //c=playerWidget->context();
                    //qDebug()<<"player c: "<<c;
                    //playerWidget->initial();
                    //playerWidget->update();
#if 0
                    PlayerPrivate *pp = new PlayerPrivate();
                    playerWidget->context()->moveToThread(pp);
                    pp->start();
#endif

                    });
        }
        {
            QPushButton* m_pbLeftRotate = new QPushButton("test");
            hbox->addWidget(m_pbLeftRotate);

            connect(m_pbLeftRotate, &QAbstractButton::clicked, 
                    [=]()
                    {
                    });
        }
    }

    centralWidget->setLayout(mainLayout);

	m_processThread=new TestOpenCLThread(this);
}
MainWindow::~MainWindow()
{
}
void MainWindow::process()
{
    qDebug()<<"MainWindow::process";
    int cnt=1;
	while(true)
    {
        if(cnt==1)
        {
            //make sure opengl context current
            //m_hiddenGl->makeCurrent();
            m_hiddenGl->test(m_sharecontext); //create textureid;

            qDebug()<<"MainWindow::process update play";
            playerWidget->update();
        }
        else if(cnt==2)
        {
            qDebug()<<"MainWindow::process main to sub thread opengl context";
            m_hiddenGl->main2sub(m_sharecontext); //create textureid;
        }
        QThread::msleep(10*1000);
        cnt++;
    }
}
void MainWindow::showEvent(QShowEvent *event)
{
    //stackedLayout->setVisible(false);
    QMainWindow::showEvent(event);
}
