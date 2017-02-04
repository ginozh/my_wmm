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
    , m_preview(new Preview(this))
{
    setWindowTitle(tr("Movie Maker"));

    QWidget* centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout;

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
#define NEW_TAB(var_name, var_png, var_text) do \
    { \
        var_name = new QToolButton(this); \
        var_name->setStyleSheet("border-style: flat;"); \
        var_name->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); \
        var_name->setIcon(QIcon(var_png)); \
        var_name->setText(var_text); \
        connect(var_name, SIGNAL(clicked(bool)), \
                this, SLOT(handleTabChange())); \
        hhoxtabbar->addWidget(var_name); \
    }while(0)

    NEW_TAB(m_mediaButton, "images/media.png", "ADD MEDIA FILE");
    NEW_TAB(m_themeButton, "images/theme.png", "SELECT THEME");
    NEW_TAB(m_musicButton, "images/music.png", "SELECT MUSIC");
    NEW_TAB(m_previewButton, "images/preview.png", "PREVIEW");
    NEW_TAB(m_saveButton, "images/save.png", "SAVE AND SHARE");
#undef NEW_TAB
#if 0
    {
        QWidget* nullWidget = new QWidget;
        nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
        hhoxtabbar->addWidget(nullWidget);
    }
#endif
}

void MainWindow::createStackWidget()
{
    {
        m_qwidgetmedia = new QWidget;
        m_stackedLayout->addWidget(m_qwidgetmedia);
        {
            QVBoxLayout* vbox = new QVBoxLayout;
            m_qwidgetmedia->setLayout(vbox);
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
        m_qwidgettheme = new QWidget;
        m_stackedLayout->addWidget(m_qwidgettheme);
        {
            QVBoxLayout* vbox = new QVBoxLayout;
            m_qwidgettheme->setLayout(vbox);
            {
                QLabel * lbl = new QLabel(tr("STEP 2:SELECT A THEME"));
                vbox->addWidget(lbl);

                lbl->setAlignment(Qt::AlignCenter);
            }
            {
                vbox->addWidget(m_themes);
            }
        }
    }
    {
        m_qwidgetmusic = new QWidget;
        m_stackedLayout->addWidget(m_qwidgetmusic);
        {
            QVBoxLayout* vbox = new QVBoxLayout;
            m_qwidgetmusic->setLayout(vbox);
            vbox->setAlignment(Qt::AlignHCenter);
            {
                QLabel * lbl = new QLabel(tr("STEP 3:SELECT A MUSIC"));
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
        m_qwidgetpreview = new QWidget;
        m_stackedLayout->addWidget(m_qwidgetpreview);
        {
            QVBoxLayout* vbox = new QVBoxLayout;
            m_qwidgetpreview->setLayout(vbox);
            vbox->setAlignment(Qt::AlignHCenter);
            {
                QLabel * lbl = new QLabel(tr("STEP 4:PREVIEW"));
                vbox->addWidget(lbl);

                lbl->setAlignment(Qt::AlignCenter);
            }
            {
                vbox->addWidget(m_preview);
            }
        }
    }
    {
        m_save = new Save;
        m_stackedLayout->addWidget(m_save);
#if 0
        {
            QHBoxLayout* hbox = new QHBoxLayout;
            m_save->setLayout(hbox);
            {
                QLabel * lbl = new QLabel(tr("save"));
                hbox->addWidget(lbl);
            }
        }
#endif
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
        m_stackedLayout->setCurrentWidget(m_qwidgetmedia);
    else if(m_themeButton == qwidget)
        m_stackedLayout->setCurrentWidget(m_qwidgettheme);
    else if(m_musicButton == qwidget)
        m_stackedLayout->setCurrentWidget(m_qwidgetmusic);
    else if(m_previewButton == qwidget)
        m_stackedLayout->setCurrentWidget(m_qwidgetpreview);
    else if(m_saveButton == qwidget)
        m_stackedLayout->setCurrentWidget(m_save);
}
void MainWindow::showEvent(QShowEvent *event)
{
    createStackWidget();
    QMainWindow::showEvent(event);
}
