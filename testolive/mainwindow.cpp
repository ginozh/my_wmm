#include "mainwindow.h"

#include "io/config.h"
#include "io/path.h"

#include "project/footage.h"
#include "project/sequence.h"
#include "project/clip.h"
#include "project/undo.h"
#include "project/media.h"
#include "project/projectfilter.h"
#include "project/effectloaders.h"
#include "project/effect.h"

#include "ui/sourcetable.h"
#include "ui/viewerwidget.h"
#include "ui/sourceiconview.h"
#include "ui/timelineheader.h"
#include "ui/viewercontainer.h"

#include "panels/panels.h"
#include "panels/project.h"
#include "panels/effectcontrols.h"
#include "panels/viewer.h"
#include "panels/timeline.h"
#include "panels/grapheditor.h"

#include "dialogs/aboutdialog.h"
#include "dialogs/newsequencedialog.h"
#include "dialogs/exportdialog.h"
#include "dialogs/preferencesdialog.h"
#include "dialogs/demonotice.h"
#include "dialogs/speeddialog.h"
#include "dialogs/actionsearch.h"
#include "dialogs/debugdialog.h"
#include "io/loadthread.h"

#include "playback/audio.h"
#include "playback/playback.h"

#include "debug.h"

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
#if 0
                    m_Viewer = new Viewer(this);
                    vbox->addWidget(m_Viewer);
                    viewer_widget = m_Viewer->viewer_widget;
#elif 0
                    m_viewer_widget = new ViewerWidget(NULL);
                    vbox->addWidget(m_viewer_widget);
                    viewer_widget = m_viewer_widget;
#else
                    viewer_container = new ViewerContainer(NULL);
                    viewer_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                    vbox->addWidget(viewer_container);
                    viewer_widget = viewer_container->child;
#endif
                }
                {
                    QPushButton* m_pbmedia = new QPushButton("load");
                    vbox->addWidget(m_pbmedia);
                    connect(m_pbmedia, &QAbstractButton::clicked, [=]() {
                            QString autorecovery="C:/Users/user/AppData/Roaming/olive-editor/autorecovery.ove";
                            init_effects();
                            LoadThread* lt = new LoadThread(NULL, true); 
                            //QObject::connect(lt, SIGNAL(success()), this, SLOT(thread_done()));
                            //QObject::connect(lt, SIGNAL(error()), this, SLOT(die()));
                            //QObject::connect(lt, SIGNAL(report_progress(int)), bar, SLOT(setValue(int)));
                            lt->start();
                            });
                }
                {
                    QPushButton* m_pbmedia = new QPushButton("update");
                    vbox->addWidget(m_pbmedia);
                    connect(m_pbmedia, &QAbstractButton::clicked, [=]() {
#if 0
                            m_Viewer->set_main_sequence();
                            m_Viewer->update_viewer();
#elif 0
                            //loadthread=>success_func()=> set_sequence(open_seq);
                            ///set_sequence(true, sequence); //project\sequence.cpp
                            viewer_widget->frame_update();
#else
                            viewer_container->adjust();
                            viewer_widget->frame_update();
#endif
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

