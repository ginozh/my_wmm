#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QTime>
#include <QLineEdit>
#include <QComboBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStringList>
#include <QDesktopWidget>
#include <QTimer>
#include <QDebug>
#include "mainwindow.h"
#include "GLHiddenWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	QGLFormat qglFormat;
	qglFormat.setProfile(QGLFormat::CoreProfile);

	//create hidden QGLWidget
#if 0
	m_hiddenGl=new GLHiddenWidget(qglFormat, this);
	//m_hiddenGl->setVisible(false);
#else
    m_hiddenGl=new GLWidget(true, qglFormat, NULL, this);
#endif

    setWindowTitle(tr("Movie Maker"));
    QWidget* centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* lbl=NULL;
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        mainLayout->addLayout(hbox);
        {
            //playerWidget = new PlayerWidget(this, false);
            //create main view
            m_mainView=new GLWidget(false, qglFormat, m_hiddenGl, this);
            hbox->addWidget(m_mainView);
        }
    }
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        mainLayout->addLayout(hbox);
        {
            //playerWidget = new PlayerWidget(this, false);
            //create main view
            m_mainView=new GLWidget(false, qglFormat, m_hiddenGl, this);
            hbox->addWidget(m_mainView);
            m_mainView->setMinimumSize(QSize(500, 250));
#if 0
            PlayerPrivate *p=new PlayerPrivate();
            p->setGLWidget(m_mainView);
            p->start();
#endif
        }
    }
    //control
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        mainLayout->addLayout(hbox);
        {
            QPushButton* m_pbPreFrame = new QPushButton("control");
            hbox->addWidget(m_pbPreFrame);
            connect(m_pbPreFrame, &QAbstractButton::clicked, 
                    [=]()
                    {
                        m_playerprivate->m_bstart=true;
#if 0
                    int idxFbo=-1;
                    QImage image1, image2;
                    {
                        QImage& image=image1;
                        QString fileName="c:\\shareproject\\jpg\\img006.jpg";
                        image.load(fileName);
                        if (image.isNull()) {
                            qDebug()<<"error";
                        }

                        image = image.convertToFormat(QImage::Format_RGBA8888);
                    }
                    GLuint texture=0;
                    m_mainView->m_shareWidget->context()->makeCurrent();
                    texture=m_mainView->load2DTexture(image1.width(), image1.height(), image1.bits());
#if 0
                    idxFbo=m_mainView->fragRenderForOtherThreadAgain("Basic"
                            , NULL, 0, texture
                            , 1, 1, 0, true, true);
                    m_mainView->m_shareWidget->context()->doneCurrent();
                    if(idxFbo<0)
                    {
                        qInfo()<<"fragRenderForOtherThread error";
                    }
                    if(idxFbo>=0)
                    {
                        m_mainView->m_idxFbo=idxFbo;
                        m_mainView->update();
                    }
#endif
                    m_mainView->m_texture=texture;
                    m_mainView->update();
#endif
                    });
        }
    }
    centralWidget->setLayout(mainLayout);
}
void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    m_mainView->initialOpengl(glw, glh);
    m_hiddenGl->initialOpengl(glw, glh);
#if 1
    m_playerprivate=new PlayerPrivate();
    m_playerprivate->setGLWidget(m_mainView);
    m_playerprivate->start();
#endif
}
