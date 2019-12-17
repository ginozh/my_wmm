#include "mainwindow.h"
#include "glwidget.h"
//#include "window.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow()
{
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
                lbl = new QLabel(tr("Player: "));
                vbox->addWidget(lbl);
//#ifdef EXPORT
                {
                    glWidget = new GLWidget(this);
                    vbox->addWidget(glWidget);
                }
            }
        }
    }
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        mainLayout->addLayout(hbox);
        {
            m_leUrl = new QLineEdit;
            //m_leUrl->setText("Z:\\Desktop\\upan\\qqrtx\\QQ\\Wildlife.wmv"); 
            m_leUrl->setText("C:\\shareproject\\OpenglEffectAdvance\\example\\Wildlife.wmv"); 
            hbox->addWidget(m_leUrl);

            //m_pbOpenFile = new QPushButton("Open File");
            //hbox->addWidget(m_pbOpenFile);

            m_pbStart = new QPushButton("Start");
            hbox->addWidget(m_pbStart);
            connect(m_pbStart, &QAbstractButton::clicked, [=]() {
                    m_url=m_leUrl->text().trimmed();
                    m_fileProcessor = new FileProcessor(this, glWidget);
                    m_fileProcessor->processMedia(m_url);
                    });
        }
    }
    centralWidget->setLayout(mainLayout);
}

void MainWindow::onAddNew()
{
    if (!centralWidget())
    {
        //setCentralWidget(new Window(this));
        setCentralWidget(new GLWidget(this));
    }
    else
        QMessageBox::information(0, tr("Cannot add new window"), tr("Already occupied. Undock first."));
}
