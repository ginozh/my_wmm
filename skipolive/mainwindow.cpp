#include "mainwindow.h"


#include "project/effect.h"
#include "project/effectloaders.h"

#include "OpenGLWidget.h"


#include <QStyleFactory>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QCloseEvent>
#include <QMovie>
#include <QInputDialog>
#include <QRegExp>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QLayout>
#include <QApplication>
#include <QPushButton>

MainWindow* mainWindow;

bool g_bEnable1,g_bEnable2,g_bEnable5,g_bEnable6,g_bEnable8,g_bEnable9;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    g_bEnable1=g_bEnable2=g_bEnable5=g_bEnable6=g_bEnable8=g_bEnable9=false;
    g_bEnable8=true;
	mainWindow = this;
    setWindowTitle(tr("Movie Maker"));
    QWidget* centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* lbl=NULL;
    //player
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        mainLayout->addLayout(hbox);
        {
            QVBoxLayout* vbox = new QVBoxLayout;
            hbox->addLayout(vbox);
            {
                lbl = new QLabel(tr("mediaPlayer: "));
                vbox->addWidget(lbl);

                {
                    m_viewer_widget = new OpenGLWidget(NULL);
                    vbox->addWidget(m_viewer_widget);
                    m_viewer_widget->setFixedSize(1280,720);
                    //m_viewer_widget->setFixedSize(640,360);
                }
                {
                    QPushButton* m_pbmedia = new QPushButton("load");
                    vbox->addWidget(m_pbmedia);
                    connect(m_pbmedia, &QAbstractButton::clicked, [=]() {
                            ////QString autorecovery="C:/Users/user/AppData/Roaming/olive-editor/test.ove";
                            init_effects();
                            ///QThread::msleep(100); //wait for initeffect
                            ////LoadThread* lt = new LoadThread(NULL, true); 
                            //QObject::connect(lt, SIGNAL(success()), this, SLOT(thread_done()));
                            //QObject::connect(lt, SIGNAL(error()), this, SLOT(die()));
                            //QObject::connect(lt, SIGNAL(report_progress(int)), bar, SLOT(setValue(int)));
                            ///lt->start();
                            });
                }
                {
                    QPushButton* m_pbmedia = new QPushButton("startgetframe");
                    vbox->addWidget(m_pbmedia);
                    connect(m_pbmedia, &QAbstractButton::clicked, [=]() {
                            m_viewer_widget->frame_update();
                            });
                }
            }
        }
    }
    centralWidget->setLayout(mainLayout);
}

MainWindow::~MainWindow() {
	////free_panels();
	////close_debug_file();
}

