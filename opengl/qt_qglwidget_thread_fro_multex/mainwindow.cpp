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
	m_hiddenGl=new GLHiddenWidget(qglFormat);
	m_hiddenGl->setVisible(false);

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
            m_mainView=new GLWidget(qglFormat, m_hiddenGl);
            PlayerPrivate *p=new PlayerPrivate();
            p->setGLWidget(m_mainView);
            p->start();
            m_mainView->setMinimumSize(QSize(500, 250));
            hbox->addWidget(m_mainView);
        }
    }
    centralWidget->setLayout(mainLayout);
}
void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    return ;
#if 0
    PlayerPrivate *p=new PlayerPrivate();
    p->setGLWidget(m_mainView);
    p->start();
#endif
}
