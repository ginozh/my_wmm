#include <QVBoxLayout>
#include <QHBoxLayout>
//#include <QGLFormat>
#include <QSurfaceFormat>
#include <QLabel>
#include <QPushButton>
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

	//create hidden QGLWidget
	m_hiddenGl=new GLHiddenWidget();
	//m_hiddenGl->setVisible(false);

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
    {
        mainLayout->addWidget(m_hiddenGl);
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
                    //move the hidden widget to the new thread id
                    m_hiddenGl->doneCurrent();
                    const QOpenGLContext * c = m_hiddenGl->context();
                    qDebug()<<"hidden c: "<<c;
                    if(c)
                    {
                        m_hiddenGl->context()->moveToThread(m_processThread);
                        //m_hiddenGl->test(this);
                        m_processThread->start();
                        //QThread::msleep(10*1000);
                    }
                    c=playerWidget->context();
                    qDebug()<<"player c: "<<c;
                    playerWidget->initial();
                    //playerWidget->update();
#if 0
                    PlayerPrivate *pp = new PlayerPrivate();
                    playerWidget->context()->moveToThread(pp);
                    pp->start();
#endif

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
	while(true)
    {
        //make sure opengl context current
        //m_hiddenGl->makeCurrent();
        m_hiddenGl->test(NULL);
        //create textureid;
        //tmp

        qDebug()<<"MainWindow::process update play";
		playerWidget->update();
        QThread::msleep(10000*1000);
    }
}
