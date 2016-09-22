#include "tabwidget.h"
#include <QLayout>


//tmp
#include <QTableWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QComboBox>
#include <QApplication>
#include <QScrollArea>
#include "flowlayout.h"
#include "animation.h"

TabWidget::TabWidget(QWidget *parent, QWidget *elementsEdit)
    : QTabWidget(parent)
    , m_iconSize(100, 100)
    , m_elementsEdit(elementsEdit)
{
    QWidget *tabNull = new QWidget;
    addTab(tabNull, tr("   "));
    createTabHome();
    createTabAnimations();

    setTabEnabled(0, false);
    //setCurrentWidget(contentsWidget);
    setMaximumHeight(m_iconSize.height() + 120);
}
void TabWidget::createTabHome()
{
    QWidget *tabHome = new QWidget;
#if 0
    QLabel *labeladdPhotos = new QLabel;
    labeladdPhotos->setStyleSheet("border: 2px solid gray;"
        "border-radius: 4px;"
        "background: url(addphotos.png) top center no-repeat;"
        "padding-top: 32px;");
    //labeladdPhotos->setStyleSheet("background-color: yellow");
    //labeladdPhotos->setText("<img src='addphotos.png' style='vertical-align:middle;'> Hello");
    labeladdPhotos->setText("Hello");
#endif
    //QTableWidget *tableWidget = new QTableWidget(10, 10);
    QToolButton *addPhotos = new QToolButton(this);
    addPhotos->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addPhotos->setIcon(QIcon("images/addphotos.png"));
    addPhotos->setIconSize(m_iconSize);
    addPhotos->setText("Add photos");
    //addPhotos->setMaximumHeight(m_iconSize.height() + 250);
    addPhotos->setMinimumWidth(m_iconSize.width() + 100);
    //addPhotos->setMinimumWidth(addPhotos->text().length());
    connect(addPhotos, SIGNAL(clicked()), m_elementsEdit, SLOT(load()));

    QToolButton *addMusic = new QToolButton(this);
    addMusic->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addMusic->setIcon(QIcon("images/addmusic.png"));
    addMusic->setIconSize(m_iconSize);
    addMusic->setText("Add music");
    addPhotos->setMinimumWidth(m_iconSize.width() + 100);

#if 0
    contentsWidget = new QListWidget;
    contentsWidget->setIconSize(m_iconSize);
    contentsWidget->setMinimumHeight(m_iconSize.height() + 250);
    contentsWidget->setMaximumWidth(m_iconSize.width() + 50);
    contentsWidget->setMovement(QListView::Static);

    QListWidgetItem *configButton = new QListWidgetItem();
    //configButton->setIcon(QIcon(":/images/addphotos.png"));
    //configButton->setIcon(QIcon("C:\\qtproject\\img2mov\\images\\addphotos.png"));
    configButton->setIcon(QIcon("addphotos.png"));
    configButton->setText(tr("Add photos"));
    configButton->setTextAlignment(Qt::AlignVCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    //contentsWidget->insertItem(0, configButton);
    //contentsWidget->addItem(configButton);
    contentsWidget->addItem(configButton);
#endif

    QHBoxLayout *hboxHome = new QHBoxLayout;
    hboxHome->setMargin(5);
    //hboxHome->addWidget(labeladdPhotos);
    hboxHome->addWidget(addPhotos, Qt::AlignLeft);
    hboxHome->addWidget(addMusic, Qt::AlignLeft);
    //hboxHome->addWidget(contentsWidget);
    tabHome->setLayout(hboxHome);
    tabHome->setMaximumWidth(addPhotos->width() + addMusic->width() + 10);

    addTab(tabHome, tr("&Home"));

    //setCurrentWidget(tabHome);
}
/*
-----------------------------------------------------------
|                |     |     |                |           |
|                |-----------|                |           |
|                |           |                |           |
|----------------------------|----------------------------|
|                            |                            |
|----------------------------|----------------------------|
 * */
void TabWidget::createTabAnimations()
{
    QWidget *tabAnimations = new QWidget;
    ////tabAnimations->setMinimumHeight(200);
    ////tabAnimations->setMaximumHeight(300);
    QHBoxLayout *hboxAnimations = new QHBoxLayout;
    {
        QVBoxLayout *vboxLeftTransition = new QVBoxLayout;
        {
            QHBoxLayout* hboxTopTransition = new QHBoxLayout;
            {
                QScrollArea *scrollArea = new QScrollArea;
                initialScrollArea(scrollArea);


                {
                    QWidget* animations = new QWidget();
                    {
                        FlowLayout* flowLayout = new FlowLayout();
                        //flowLayout->setContentsMargins(10, 10, 10, 10);
                        flowLayout->setContentsMargins(5, 5, 5, 5);
                        {
#define INITIAL_ANIMATION(animation_name) do {\
    Animation *animation_name=new Animation("images/"#animation_name".png", ""#animation_name"", m_iconSize) ; \
    connect(animation_name, SIGNAL(selectedAnimationSignal(const QString&)), m_elementsEdit, SLOT(selectedTransition(const QString&))); \
    flowLayout->insertWidget(0, animation_name); \
} while(0)
#if 0
                            Animation *animation=new Animation("images/bowtieh.png", "bowtieh") ;
                            connect(animation, SIGNAL(selectedAnimationSignal(const QString&)), m_elementsEdit, SLOT(selectedTransition(const QString&)));

                            flowLayout->insertWidget(0, animation);
#endif
                            INITIAL_ANIMATION(bowtieh);
                            INITIAL_ANIMATION(bowtiev);
                        }

                        animations->setLayout(flowLayout);
                    }
                    scrollArea->setWidget(animations);
                }

                hboxTopTransition->addWidget(scrollArea);
            }
            {
                QVBoxLayout *vboxTopRightTransition = new QVBoxLayout;
                {
                    QHBoxLayout *hboxTopRightTopTransition = new QHBoxLayout;
                    {
                        QLabel *lblDurationTrans = new QLabel(tr("Duration: "));

                        hboxTopRightTopTransition->addWidget(lblDurationTrans);
                    }
                    {
                        QComboBox* comboBox = new QComboBox();
                        comboBox->setEditable(true);

                        hboxTopRightTopTransition->addWidget(comboBox);
                    }

                    vboxTopRightTransition->addLayout(hboxTopRightTopTransition);
                }
                {
                    QAbstractButton *btnApplyTrans = new QPushButton(tr("Apply to all"));
                    //connect(btnApplyTrans, SIGNAL(clicked()), m_elementsEdit, SLOT(openFile()));
                    //btnApplyTrans->setAlignment(Qt::AlignCenter);

                    vboxTopRightTransition->addWidget(btnApplyTrans);
                }

                hboxTopTransition->addLayout(vboxTopRightTransition);
            }

            vboxLeftTransition->addLayout(hboxTopTransition);
        }
        {
            QLabel *lblTransition = new QLabel(tr("Transition"));
            lblTransition->setAlignment(Qt::AlignCenter);

            vboxLeftTransition->addWidget(lblTransition);
        }

        hboxAnimations->addLayout(vboxLeftTransition);
    }
    {
        appendLine(hboxAnimations);
    }
    {
        QVBoxLayout *vboxRightPanZoom = new QVBoxLayout;
        {
            QHBoxLayout* hboxTopPanZoom = new QHBoxLayout;
            {
                QScrollArea *scrollArea = new QScrollArea;
                initialScrollArea(scrollArea);


                {
                    QWidget* panzooms = new QWidget();
                    {
                    }

                    scrollArea->setWidget(panzooms);
                }

                hboxTopPanZoom->addWidget(scrollArea);
            }
            {
                QAbstractButton *btnApplyPanzoom = new QPushButton(tr("Apply to all"));
                //connect(btnApplyPanzoom, SIGNAL(clicked()), m_elementsEdit, SLOT(openFile()));
                //btnApplyPanzoom->setAlignment(Qt::AlignCenter);

                hboxTopPanZoom->addWidget(btnApplyPanzoom);
            }
            vboxRightPanZoom->addLayout(hboxTopPanZoom);
        }
        {
            QLabel *lblPanZoom = new QLabel(tr("Pan and zoom"));
            lblPanZoom->setAlignment(Qt::AlignCenter);

            vboxRightPanZoom->addWidget(lblPanZoom);
        }

        hboxAnimations->addLayout(vboxRightPanZoom);
    }
    {
        appendLine(hboxAnimations);
    }

    tabAnimations->setLayout(hboxAnimations);

    addTab(tabAnimations, tr("&Animations"));
    setCurrentWidget(tabAnimations);
}
void TabWidget::appendLine(QHBoxLayout *hboxAnimations)
{
    QFrame *line = new QFrame();
    line->setObjectName(QStringLiteral("line"));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    hboxAnimations->addWidget(line);
}
void TabWidget::initialScrollArea(QScrollArea *scrollArea)
{
    //scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setMinimumWidth(500);
    ////scrollArea->setMinimumHeight(120);
    ////scrollArea->setMaximumHeight(120);
    //scrollArea->setContentsMargins(10, 10, 10, 10);
    scrollArea->setContentsMargins(0, 0, 0, 0);
    scrollArea->setWidgetResizable (true);
}
void TabWidget::createTabText()
{
}
