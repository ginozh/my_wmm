#include <QVBoxLayout>
#include <QToolButton>
#include <QStackedLayout>
#include <QLabel>
#include <QMenu>
#include <QScrollArea>
#include <QDebug>
#include "mainwindow.h"
#include "videoplayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_mediasEdit(new MediasEdit(this))
    , m_themes(new Themes(this))
    , m_musics(new Musics(this))
{
    setWindowTitle(tr("Movie Maker"));

    QWidget* centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout;

    createMenu(mainLayout);

    createTabBar(mainLayout);

    m_stackedLayout = new QStackedLayout;
    mainLayout->addLayout(m_stackedLayout);
    // createStackWidget(mainLayout); // 后续调用，避免出现一启动就出现小窗口的问题

    createPageUpDown(mainLayout);
    centralWidget->setLayout(mainLayout);
}
void MainWindow::createMenu(QVBoxLayout *mainLayout)
{
    QHBoxLayout* hbox = new QHBoxLayout;
    mainLayout->addLayout(hbox);
    {
        QWidget* nullWidget = new QWidget;
        nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
        hbox->addWidget(nullWidget);
    }
    {
        QToolButton* m_boldButton = new QToolButton(this);
        m_boldButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        m_boldButton->setIcon(QIcon("images/shopper.png"));
        hbox->addWidget(m_boldButton);
        hbox->setAlignment(m_boldButton, Qt::AlignRight|Qt::AlignTop);
    }
    {
        QToolButton* m_boldButton = new QToolButton(this);
        m_boldButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        m_boldButton->setIcon(QIcon("images/menu.png"));
        hbox->addWidget(m_boldButton);
        hbox->setAlignment(m_boldButton, Qt::AlignRight|Qt::AlignTop);
    }
    {
        QToolButton* m_boldButton = new QToolButton(this);
        m_boldButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        m_boldButton->setIcon(QIcon("images/sun.png"));
        hbox->addWidget(m_boldButton);
        hbox->setAlignment(m_boldButton, Qt::AlignRight|Qt::AlignTop);
    }
}
void MainWindow::createTabBar(QVBoxLayout *mainLayout)
{
    QHBoxLayout* hhoxtabbar = new QHBoxLayout;
    mainLayout->addLayout(hhoxtabbar);
#if 0
    {
        QWidget* nullWidget = new QWidget;
        nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
        hhoxtabbar->addWidget(nullWidget);
    }
#endif
    {
        m_mediaButton = new QToolButton(this);
        m_mediaButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        m_mediaButton->setIcon(QIcon("images/media.png"));
        m_mediaButton->setText("ADD MEDIA FILE");
        connect(m_mediaButton, SIGNAL(clicked(bool)),
                this, SLOT(handleTabChange()));
        hhoxtabbar->addWidget(m_mediaButton);
    }
    {
        m_themeButton = new QToolButton(this);
        m_themeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        m_themeButton->setIcon(QIcon("images/theme.png"));
        m_themeButton->setText("SELECT THEME");
        connect(m_themeButton, SIGNAL(clicked(bool)),
                this, SLOT(handleTabChange()));
        hhoxtabbar->addWidget(m_themeButton);
    }
    {
        m_musicButton = new QToolButton(this);
        m_musicButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        m_musicButton->setIcon(QIcon("images/music.png"));
        m_musicButton->setText("SELECT MUSIC");
        connect(m_musicButton, SIGNAL(clicked(bool)),
                this, SLOT(handleTabChange()));
        hhoxtabbar->addWidget(m_musicButton);
    }
    {
        m_previewButton = new QToolButton(this);
        m_previewButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        m_previewButton->setIcon(QIcon("images/preview.png"));
        m_previewButton->setText("PREVIEW");
        connect(m_previewButton, SIGNAL(clicked(bool)),
                this, SLOT(handleTabChange()));
        hhoxtabbar->addWidget(m_previewButton);
    }
    {
        m_saveButton = new QToolButton(this);
        m_saveButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        m_saveButton->setIcon(QIcon("images/save.png"));
        m_saveButton->setText("SAVE AND SHARE");
        connect(m_saveButton, SIGNAL(clicked(bool)),
                this, SLOT(handleTabChange()));
        hhoxtabbar->addWidget(m_saveButton);
    }
#if 0
    {
        QWidget* nullWidget = new QWidget;
        nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
        hhoxtabbar->addWidget(nullWidget);
    }
#endif
}

void MainWindow::createStackWidget(QVBoxLayout *mainLayout)
{
    {
        qwidgetmedia = new QWidget;
        m_stackedLayout->addWidget(qwidgetmedia);
        {
            QVBoxLayout* vbox = new QVBoxLayout;
            qwidgetmedia->setLayout(vbox);
            {
                QToolButton* m_boldButton = new QToolButton(this);
                m_boldButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                m_boldButton->setIcon(QIcon("images/download.png"));
                m_boldButton->setText("[IMPORT]");
                m_boldButton->setPopupMode(QToolButton::MenuButtonPopup);
                QMenu* menu = new QMenu(m_boldButton);
                m_boldButton->setMenu(menu);
                menu->addAction(tr("Import Media Files..."), m_mediasEdit, SLOT(addMedias())  );
                vbox->addWidget(m_boldButton);
                vbox->setAlignment(m_boldButton, Qt::AlignHCenter|Qt::AlignTop);

            }
            {
                QScrollArea* m_scrollArea = new QScrollArea;
                vbox->addWidget(m_scrollArea);

                m_scrollArea->setBackgroundRole(QPalette::Light);
                //m_scrollArea->setMinimumWidth(500*m_globalContext->m_dFactorX);
                m_scrollArea->setWidgetResizable (true);
                m_scrollArea->setWidget(m_mediasEdit);
            }
        }
    }
    {
        qwidgettheme = new QWidget;
        m_stackedLayout->addWidget(qwidgettheme);
        {
            QVBoxLayout* vbox = new QVBoxLayout;
            qwidgettheme->setLayout(vbox);
            {
                QLabel * lbl = new QLabel(tr("STEP2:SELECT A THEME"));
                vbox->addWidget(lbl);

                lbl->setAlignment(Qt::AlignCenter);
            }
            {
                QHBoxLayout* hbox = new QHBoxLayout;
                vbox->addLayout(hbox);
                {
                    QScrollArea* m_scrollArea = new QScrollArea;
                    hbox->addWidget(m_scrollArea);

                    m_scrollArea->setBackgroundRole(QPalette::Light);
                    m_scrollArea->setWidgetResizable (true);
                    m_scrollArea->setWidget(m_themes);
                }
                {
                    VideoPlayer* m_player = new VideoPlayer(this);
                    hbox->addWidget(m_player);
                }
            }
        }
    }
    {
        qwidgetmusic = new QWidget;
        m_stackedLayout->addWidget(qwidgetmusic);
        {
            QVBoxLayout* vbox = new QVBoxLayout;
            qwidgetmusic->setLayout(vbox);
            vbox->setAlignment(Qt::AlignHCenter);
            {
                QLabel * lbl = new QLabel(tr("STEP3:SELECT A MUSIC"));
                vbox->addWidget(lbl);

                lbl->setAlignment(Qt::AlignCenter);
            }
            {
                // QVBoxLayout* vboxmusic = new QVBoxLayout;
                // vbox->addLayout(vboxmusic);
                vbox->addWidget(m_musics);
            }
        }
    }
    {
        qwidgetpreview = new QWidget;
        m_stackedLayout->addWidget(qwidgetpreview);
        {
            QHBoxLayout* hbox = new QHBoxLayout;
            qwidgetpreview->setLayout(hbox);
            {
                QLabel * lbl = new QLabel(tr("preview"));
                hbox->addWidget(lbl);
            }
        }
    }
    {
        qwidgetsave = new QWidget;
        m_stackedLayout->addWidget(qwidgetsave);
        {
            QHBoxLayout* hbox = new QHBoxLayout;
            qwidgetsave->setLayout(hbox);
            {
                QLabel * lbl = new QLabel(tr("save"));
                hbox->addWidget(lbl);
            }
        }
    }
}

void MainWindow::createPageUpDown(QVBoxLayout *mainLayout)
{
    QHBoxLayout* hboxpage = new QHBoxLayout;
    mainLayout->addLayout(hboxpage);
    {
        QWidget* nullWidget = new QWidget;
        nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
        hboxpage->addWidget(nullWidget);
    }
    {
        QToolButton* m_tbZoomPan = new QToolButton;
        hboxpage->addWidget(m_tbZoomPan);
        m_tbZoomPan->setText("Previous");
    }
    {
        QToolButton* m_tbZoomPan = new QToolButton;
        hboxpage->addWidget(m_tbZoomPan);
        m_tbZoomPan->setText("Next");
    }
}

void MainWindow::handleTabChange()
{
    QToolButton* qwidget = qobject_cast<QToolButton *>(sender());
    qDebug()<< "MainWindow::handleTabChange. qwidget: " << qwidget;
    // qwidget->raise();
    if(m_mediaButton == qwidget)
        m_stackedLayout->setCurrentWidget(qwidgetmedia);
    else if(m_themeButton == qwidget)
        m_stackedLayout->setCurrentWidget(qwidgettheme);
    else if(m_musicButton == qwidget)
        m_stackedLayout->setCurrentWidget(qwidgetmusic);
    else if(m_previewButton == qwidget)
        m_stackedLayout->setCurrentWidget(qwidgetpreview);
    else if(m_saveButton == qwidget)
        m_stackedLayout->setCurrentWidget(qwidgetsave);
}
void MainWindow::showEvent(QShowEvent *event)
{
    createStackWidget(mainLayout);
    QMainWindow::showEvent(event);
}
