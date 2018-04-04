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
#include "PlayerPrivate.h"
PlayerPrivate* m_playerprivate=NULL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	QGLFormat qglFormat;
	qglFormat.setProfile(QGLFormat::CoreProfile);

	//create hidden QGLWidget
#if 1
	m_hiddenWidget=new GLHiddenWidget(qglFormat, this);
	//m_hiddenWidget->setVisible(false);
#else
    m_hiddenWidget=new GLWidget(true, qglFormat, NULL, this);
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
            m_playerWidget=new GLDisplayWidget(qglFormat, m_hiddenWidget, this);
            hbox->addWidget(m_playerWidget);
            m_playerWidget->setMinimumSize(QSize(500, 250));

#if 0
            PlayerPrivate *p=new PlayerPrivate();
            p->setGLWidget(m_playerWidget);
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
#if 1
                    if(m_playerprivate)
                        m_playerprivate->m_bstart=true;
#endif
#if 0
                    int idxFbo=-1;
                    QImage image1, image2;
                    {
                        QImage& image=image1;
                        QString fileName="1.jpg";
                        image.load(fileName);
                        if (image.isNull()) {
                            qDebug()<<"error";
                        }

                        image = image.convertToFormat(QImage::Format_RGBA8888);
                    }
                    GLuint texture=0;
                    m_playerWidget->m_shareWidget->context()->makeCurrent();
                    texture=m_playerWidget->m_shareWidget->load2DTexture(image1.width(), image1.height(), image1.bits());
#if 1
                    idxFbo=m_playerWidget->m_shareWidget->fragRenderForOtherThreadAgain("Basic"
                            , NULL, 0, texture
                            , 1, 1, 0, true, true);
                    if(idxFbo<0)
                    {
                        qInfo()<<"fragRenderForOtherThread error";
                    }
#endif
                    if(idxFbo>=0)
                    {
                        m_playerWidget->m_texture=m_playerWidget->m_shareWidget->GetTexture(idxFbo);
                        m_playerWidget->update();
                    }
                    else if(texture>0)
                    {
                        m_playerWidget->m_texture=texture;
                        m_playerWidget->update();
                    }
                    m_playerWidget->m_shareWidget->context()->doneCurrent();

                    m_playerWidget->m_texture=texture;
                    m_playerWidget->update();

#endif
                    });
        }
    }
    centralWidget->setLayout(mainLayout);
}
void MainWindow::showEvent(QShowEvent *event)
{
    qDebug()<<"MainWindow::showEvent";
    QMainWindow::showEvent(event);
    //GLHiddenWidget::initializeGL() //effect program
    //m_hiddenWidget->createBindFramebufferTexture(glw, glh); //fbo
    m_playerprivate=new PlayerPrivate();
    m_playerprivate->setGLWidget(m_playerWidget); //movethread
    m_playerWidget->setShareGLContext();
    m_playerWidget->setGLSize(glw, glh); //m_playerprivate->start 之前
    ////m_hiddenWidget->setVisible(false);
    ////m_playerWidget->initialOpengl(glw, glh);
    //m_hiddenWidget->initialOpengl(glw, glh);
    ////m_hiddenWidget->setVisible(false);
#if 1
    //m_playerprivate->setGLWidget(m_playerWidget);
    m_playerprivate->start();
#endif
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"MainWindow::closeEvent";
    m_playerprivate->stopPlayback();
    QMainWindow::closeEvent(event);
}
