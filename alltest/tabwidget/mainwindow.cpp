#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include "mainwindow.h"
#include <QFrame>
#include <QFont>
#include <QToolButton>
#include <QMenu>
#include <QTest> 
#include <QTabBar> 
TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    //tabBar()->setExpanding(false);
    int iHeight = 230;
    int m_iCellHeight = iHeight/4;
    QSize m_iconSize(150,110);
    QWidget* m_tabHome = new QWidget(this);
    addTab(m_tabHome, tr("&Home"));

    QHBoxLayout *hbox = new QHBoxLayout;
    m_tabHome->setLayout(hbox);
    {
        QVBoxLayout *vboxAdd = new QVBoxLayout;
        vboxAdd->setGeometry(QRect(0, 0, 3*m_iCellHeight, 4*m_iCellHeight));
        hbox->addLayout(vboxAdd);
        {
            QHBoxLayout* hboxTop = new QHBoxLayout;
            vboxAdd->addLayout(hboxTop);
            {
                QToolButton *addPhotos = new QToolButton();
                hboxTop->addWidget(addPhotos, Qt::AlignLeft);

                addPhotos->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                //addPhotos->setAlignment(Qt::AlignLeft);
                addPhotos->setIcon(QIcon("images/addphotos.png"));
                addPhotos->setIconSize(m_iconSize);
                addPhotos->setText("Add photos");
                addPhotos->setFixedWidth(3*m_iCellHeight);
            }
            {
                QToolButton *addMusic = new QToolButton();
                hboxTop->addWidget(addMusic, Qt::AlignLeft);
                addMusic->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                addMusic->setIcon(QIcon("images/addmusic.png"));
                addMusic->setIconSize(m_iconSize);
                addMusic->setText("Add music");
                addMusic->setFixedWidth(3*m_iCellHeight);
            }
            {
                QVBoxLayout *vboxTopRightAdd = new QVBoxLayout;
                hboxTop->addLayout(vboxTopRightAdd);
                {
                    QToolButton *addTitle = new QToolButton();
                    vboxTopRightAdd->addWidget(addTitle, Qt::AlignLeft);

                    addTitle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                    //addTitle->setIcon(QIcon("images/addTitle.png"));
                    //addTitle->setIconSize(m_iconSize);
                    addTitle->setText("Title");
                    addTitle->setFixedWidth(3*m_iCellHeight);
                    //connect(addTitle, SIGNAL(clicked()), m_globalContext->m_elementsEdit, SLOT(addImages()));

                }
                {
                    QToolButton *addCaption = new QToolButton();
                    vboxTopRightAdd->addWidget(addCaption, Qt::AlignLeft);

                    addCaption->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                    //addCaption->setIcon(QIcon("images/addCaption.png"));
                    //addCaption->setIconSize(m_iconSize);
                    addCaption->setText("Caption");
                    addCaption->setFixedWidth(3*m_iCellHeight);

                }
                {
                    QToolButton *addCredits = new QToolButton();
                    vboxTopRightAdd->addWidget(addCredits, Qt::AlignLeft);

                    addCredits->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                    //addCredits->setIcon(QIcon("images/addCredits.png"));
                    //addCredits->setIconSize(m_iconSize);
                    addCredits->setText("Credits");
                    addCredits->setFixedWidth(3*m_iCellHeight);
                    //connect(addCredits, SIGNAL(clicked()), m_globalContext->m_elementsEdit, SLOT(addImages()));

                }
            }
        }
        {
            QLabel *lbl = new QLabel(tr("Add"));
            lbl->setAlignment(Qt::AlignCenter);
            lbl->setFixedWidth(3*m_iCellHeight);

            vboxAdd->addWidget(lbl);
        }
    }
    {
        appendLine(hbox);
    }
    {
        QVBoxLayout *vboxEditing = new QVBoxLayout;
        hbox->addLayout(vboxEditing);
        {
            QHBoxLayout* hboxTop = new QHBoxLayout;
            vboxEditing->addLayout(hboxTop);
            {
                QToolButton* m_tbRotateLeft = new QToolButton();
                hboxTop->addWidget(m_tbRotateLeft, Qt::AlignLeft);

                m_tbRotateLeft->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                m_tbRotateLeft->setIcon(QIcon("images/addphotos.png"));
                m_tbRotateLeft->setIconSize(m_iconSize);
                m_tbRotateLeft->setText("Rotate Left");
                m_tbRotateLeft->setFixedWidth(3*m_iCellHeight);
                //m_tbRotateLeft->setMaximumHeight(m_iconSize.height() + 250);
                //m_tbRotateLeft->setMinimumWidth(m_iconSize.width() + 100);
            }
            {
                QToolButton* m_tbRotateRight = new QToolButton();
                hboxTop->addWidget(m_tbRotateRight, Qt::AlignLeft);
                m_tbRotateRight->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                m_tbRotateRight->setIcon(QIcon("images/addmusic.png"));
                m_tbRotateRight->setIconSize(m_iconSize);
                m_tbRotateRight->setText("Rotate Right");
                m_tbRotateRight->setFixedWidth(3*m_iCellHeight);
            }
            {
                QVBoxLayout *vboxTopRightEditing = new QVBoxLayout;
                hboxTop->addLayout(vboxTopRightEditing);
                {
                    QToolButton *remove = new QToolButton();
                    vboxTopRightEditing->addWidget(remove, Qt::AlignLeft);

                    remove->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                    //remove->setIcon(QIcon("images/remove.png"));
                    //remove->setIconSize(m_iconSize);
                    remove->setText("Remove");
                    remove->setFixedWidth(3*m_iCellHeight);
                    //connect(remove, SIGNAL(clicked()), m_globalContext->m_elementsEdit, SLOT(addImages()));

                }
                {
                    QToolButton *selectAll = new QToolButton();
                    vboxTopRightEditing->addWidget(selectAll, Qt::AlignLeft);

                    selectAll->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                    //selectAll->setIcon(QIcon("images/selectAll.png"));
                    //selectAll->setIconSize(m_iconSize);
                    selectAll->setText("Select all");
                    selectAll->setFixedWidth(3*m_iCellHeight);
                    //selectAll->setMaximumHeight(m_iconSize.height() + 250);
                    //selectAll->setMinimumWidth(m_iconSize.width() + 100);
                    //selectAll->setMinimumWidth(selectAll->text().length());
                    //connect(selectAll, SIGNAL(clicked()), m_globalContext->m_elementsEdit, SLOT(addImages()));

                }
            }
        }
        {
            QLabel *lbl = new QLabel(tr("Editing"));
            lbl->setAlignment(Qt::AlignCenter);
            lbl->setFixedWidth(3*m_iCellHeight);

            vboxEditing->addWidget(lbl);
        }
    }
    {
        appendLine(hbox);
    }
    QWidget *tmp = new QWidget;
    tmp->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    hbox->addWidget(tmp);
}
MainWindow::MainWindow()
{
    setWindowTitle(tr("progress"));
    TabWidget* m_tabWidget = new TabWidget();

    QWidget* m_centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    m_centralWidget->setLayout(mainLayout);

    setCentralWidget(m_centralWidget);

    qApp->setStyleSheet("QTabWidget::tab-bar { alignment: left; }");
}
void TabWidget::appendLine(QHBoxLayout *hbox)
{
    QFrame *line = new QFrame();
    line->setObjectName(QStringLiteral("line"));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    hbox->addWidget(line);
}
