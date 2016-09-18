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
    setMaximumHeight(m_iconSize.height() + 100);
}
#if 0
void TabWidget::createTabHome()
{
    contentsWidget = new QListWidget;
    QHBoxLayout *hboxHome = new QHBoxLayout;

    //contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(m_iconSize);
    contentsWidget->setMinimumHeight(m_iconSize.height() + 50);
    contentsWidget->setMovement(QListView::Static);
    //contentsWidget->setMaximumWidth(528);
    //contentsWidget->setMaximumHeight(100);
    //contentsWidget->setMaximumHeight(100);
    contentsWidget->setSpacing(12);

    QListWidgetItem *configButton = new QListWidgetItem();
    //configButton->setIcon(QIcon(":/images/addphotos.png"));
    //configButton->setIcon(QIcon("C:\\qtproject\\img2mov\\images\\addphotos.png"));
    configButton->setIcon(QIcon("addphotos.png"));
    configButton->setText(tr("Add photos"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    //contentsWidget->insertItem(0, configButton);
    contentsWidget->addItem(configButton);

    QListWidgetItem *updateButton = new QListWidgetItem();
    //updateButton->setIcon(QIcon(":/images/addmusic.png"));
    updateButton->setIcon(QIcon("addmusic.png"));
    updateButton->setText(tr("Add music"));
    updateButton->setTextAlignment(Qt::AlignHCenter);
    updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    //contentsWidget->insertItem(0, updateButton);
    contentsWidget->addItem(updateButton);

    //hboxHome->addWidget(contentsWidget);
    //hboxHome->addWidget(updateButton);
    //contentsWidget->setLayout(hboxHome);
    addTab(contentsWidget, tr("&Home"));
}
#endif
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
void TabWidget::createTabAnimations()
{
    //QWidget *centralWidget;
    //QTabWidget *tabWidget;
    //QWidget *tab;
    //QWidget *tabHome;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *hboxAnimations;
    QWidget *leftTransition;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *vboxLeftTransition;
    QWidget *lefttopTransition;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *hboxTopTransition;
    QWidget *lefttopleftTransition;
    QLabel *lblAnimation1;
    QLabel *lblAnimation1_2;
    QWidget *lefttoprightTransition;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *vboxTopRightTransition;
    QWidget *lefttoprighttopTransition;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *hboxTopRightTopTransition;
    QLabel *lblDurationTrans;
    QComboBox *comboBox;
    QWidget *lefttoprightbottomTransition;
    QLabel *label_3;
    QWidget *leftbottomTransition;
    QLabel *label;
    QFrame *line;
    QWidget *rightPanZoom;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *vboxRightPanZoom;
    QWidget *righttopPanZoom;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *hboxTopPanZoom;
    QWidget *righttopleftPanZoom;
    QLabel *label_2;
    QLabel *label_4;
    QWidget *righttoprightPanZoom;
    QLabel *lblApplyAllPanzoom;
    QWidget *rightbottomPanZoom;
    QLabel *lblPanZoom;
    QFrame *line_2;
    QWidget *tabAnimations;
    //QMenuBar *menuBar;
    //QToolBar *mainToolBar;
    //QStatusBar *statusBar;

    tabAnimations = new QWidget();
    tabAnimations->setObjectName(QStringLiteral("tabAnimations"));
    horizontalLayoutWidget = new QWidget(tabAnimations);
    horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
    horizontalLayoutWidget->setGeometry(QRect(0, 0, 1000, 200));
    {
        // hboxlayout
        hboxAnimations = new QHBoxLayout(horizontalLayoutWidget);
        hboxAnimations->setSpacing(6);
        hboxAnimations->setContentsMargins(11, 11, 11, 11);
        hboxAnimations->setObjectName(QStringLiteral("hboxAnimations"));
        hboxAnimations->setContentsMargins(0, 0, 0, 0);
        {
            //// QWidgetleftTransition
            leftTransition = new QWidget(horizontalLayoutWidget);
            leftTransition->setObjectName(QStringLiteral("leftTransition"));
            verticalLayoutWidget = new QWidget(leftTransition);
            verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
            verticalLayoutWidget->setGeometry(QRect(0, 0, 500, 200));
            {
                //vboxleyout vboxLeftTransition
                vboxLeftTransition = new QVBoxLayout(verticalLayoutWidget);
                vboxLeftTransition->setSpacing(6);
                vboxLeftTransition->setContentsMargins(11, 11, 11, 11);
                vboxLeftTransition->setObjectName(QStringLiteral("vboxLeftTransition"));
                vboxLeftTransition->setContentsMargins(0, 0, 0, 0);
                {
                    // QWidget lefttopTransition
                    lefttopTransition = new QWidget(verticalLayoutWidget);
                    lefttopTransition->setObjectName(QStringLiteral("lefttopTransition"));
                    horizontalLayoutWidget_2 = new QWidget(lefttopTransition);
                    horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
                    horizontalLayoutWidget_2->setGeometry(QRect(0, 0, 500, 120));
                    {
                        //// QHBoxLayout hboxTopTransition
                        hboxTopTransition = new QHBoxLayout(horizontalLayoutWidget_2);
                        hboxTopTransition->setSpacing(6);
                        hboxTopTransition->setContentsMargins(11, 11, 11, 11);
                        hboxTopTransition->setObjectName(QStringLiteral("hboxTopTransition"));
                        hboxTopTransition->setContentsMargins(0, 0, 0, 0);
                        {
                            // QWidget lefttopleftTransition
                            lefttopleftTransition = new QWidget(horizontalLayoutWidget_2);
                            lefttopleftTransition->setObjectName(QStringLiteral("lefttopleftTransition"));
#if 1
                            {
                                QScrollArea *scrollArea;
                                QWidget *scrollAreaWidgetContents;
                                QWidget *animationsWidget;
                                QVBoxLayout *verticalLayout;
                                QHBoxLayout *horizontalLayout;
                                QLabel *label;
                                QLabel *label_2;
                                QLabel *label_3;
                                QHBoxLayout *horizontalLayout_2;
                                QLabel *label_5;
                                QLabel *label_4;
                                QLabel *label_6;
                                scrollArea = new QScrollArea(lefttopleftTransition);
                                scrollArea->setObjectName(QStringLiteral("scrollArea"));
                                scrollArea->setGeometry(QRect(0, 0, 300, 120));// 10, 30, 54, 31
                                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                                scrollArea->setWidgetResizable(true);
#if 0
                                scrollAreaWidgetContents = new QWidget();
                                scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
                                scrollAreaWidgetContents->setGeometry(QRect(0, 0, 205, 250));
                                scrollAreaWidgetContents->setMinimumSize(QSize(0, 250));
                                scrollAreaWidgetContents->setBaseSize(QSize(0, 0));
                                animationsWidget = new QWidget(scrollAreaWidgetContents);
                                animationsWidget->setObjectName(QStringLiteral("animationsWidget"));
                                animationsWidget->setGeometry(QRect(10, 30, 166, 112));
                                scrollArea->setWidget(scrollAreaWidgetContents);
#else
                                animationsWidget = new QWidget();
                                animationsWidget->setObjectName(QStringLiteral("animationsWidget"));
                                animationsWidget->setGeometry(QRect(0, 0, 205, 250));
                                animationsWidget->setMinimumSize(QSize(0, 250));
                                animationsWidget->setBaseSize(QSize(0, 0));
                                scrollArea->setWidget(animationsWidget);
#endif

#if 1
                                FlowLayout* flowLayout = new FlowLayout(animationsWidget);
                                flowLayout->setSpacing(6);
                                flowLayout->setContentsMargins(11, 11, 11, 11);
                                flowLayout->setObjectName(QStringLiteral("flowLayout"));
                                flowLayout->setContentsMargins(0, 0, 0, 0);
                                label = new QLabel(animationsWidget);
                                label->setObjectName(QStringLiteral("label"));
                                label->setMaximumSize(QSize(50, 50));
                                label->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img001.jpg")));
                                label->setScaledContents(true);

                                flowLayout->insertWidget(0, label);

                                label_2 = new QLabel(animationsWidget);
                                label_2->setObjectName(QStringLiteral("label_2"));
                                label_2->setMaximumSize(QSize(50, 50));
                                label_2->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img002.jpg")));
                                label_2->setScaledContents(true);
                                flowLayout->insertWidget(0, label_2);
#else
                                verticalLayout = new QVBoxLayout(animationsWidget);
                                verticalLayout->setSpacing(6);
                                verticalLayout->setContentsMargins(11, 11, 11, 11);
                                verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
                                verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
                                verticalLayout->setContentsMargins(0, 0, 0, 0);
                                horizontalLayout = new QHBoxLayout();
                                horizontalLayout->setSpacing(6);
                                horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
                                label = new QLabel(animationsWidget);
                                label->setObjectName(QStringLiteral("label"));
                                label->setMaximumSize(QSize(50, 50));
                                label->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img001.jpg")));
                                label->setScaledContents(true);

                                horizontalLayout->addWidget(label);

                                label_2 = new QLabel(animationsWidget);
                                label_2->setObjectName(QStringLiteral("label_2"));
                                label_2->setMaximumSize(QSize(50, 50));
                                label_2->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img002.jpg")));
                                label_2->setScaledContents(true);

                                horizontalLayout->addWidget(label_2);

                                label_3 = new QLabel(animationsWidget);
                                label_3->setObjectName(QStringLiteral("label_3"));
                                label_3->setMaximumSize(QSize(50, 50));
                                label_3->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img003.jpg")));
                                label_3->setScaledContents(true);

                                horizontalLayout->addWidget(label_3);


                                verticalLayout->addLayout(horizontalLayout);

                                horizontalLayout_2 = new QHBoxLayout();
                                horizontalLayout_2->setSpacing(6);
                                horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
                                label_5 = new QLabel(animationsWidget);
                                label_5->setObjectName(QStringLiteral("label_5"));
                                label_5->setMaximumSize(QSize(50, 50));
                                label_5->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img005.jpg")));
                                label_5->setScaledContents(true);

                                horizontalLayout_2->addWidget(label_5);

                                label_4 = new QLabel(animationsWidget);
                                label_4->setObjectName(QStringLiteral("label_4"));
                                label_4->setMaximumSize(QSize(50, 50));
                                label_4->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img004.jpg")));
                                label_4->setScaledContents(true);

                                horizontalLayout_2->addWidget(label_4);

                                label_6 = new QLabel(animationsWidget);
                                label_6->setObjectName(QStringLiteral("label_6"));
                                label_6->setMaximumSize(QSize(50, 50));
                                label_6->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img006.jpg")));
                                label_6->setScaledContents(true);

                                horizontalLayout_2->addWidget(label_6);


                                verticalLayout->addLayout(horizontalLayout_2);
#endif
                            }
#endif

#if 0
                            //QWidget* flowLayoutWidget = new QWidget(lefttopleftTransition);
                            //flowLayoutWidget->setObjectName(QStringLiteral("flowLayoutWidget"));
                            //flowLayoutWidget->setGeometry(QRect(0, 0, 100, 80));
                            {
                                QScrollArea* m_scrollArea = new QScrollArea(lefttopleftTransition);
                                m_scrollArea->setBackgroundRole(QPalette::Dark);
                                m_scrollArea->setMinimumWidth(100);
                                m_scrollArea->setWidgetResizable (true);
                                m_scrollArea->setGeometry(QRect(10, 30, 121, 81));// 10, 30, 54, 31
                                QWidget* m_elementsEdit = new QWidget;
                                m_scrollArea->setWidget(m_elementsEdit);
                                //// layout
                                FlowLayout* flowLayout = new FlowLayout(m_elementsEdit);
                                flowLayout->setSpacing(6);
                                flowLayout->setContentsMargins(11, 11, 11, 11);
                                flowLayout->setObjectName(QStringLiteral("flowLayout"));
                                flowLayout->setContentsMargins(0, 0, 0, 0);
                                {
                                    QLabel *lefttopleft1Transition = new QLabel(m_elementsEdit);
                                    QPixmap image("images/reveal_left.png");
                                    const QSize maximumSize(50, 50); // Reduce in case someone has large photo images.
                                    if (image.size().width() > maximumSize.width() || image.height() > maximumSize.height())
                                    {
                                        image = image.scaled(maximumSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                                    }
                                    lefttopleft1Transition->setPixmap(image);
                                    lefttopleft1Transition->setScaledContents(true);
                                    flowLayout->insertWidget(0, lefttopleft1Transition);

                                }
                                
                            }
#endif

#if 0
                            lblAnimation1 = new QLabel(lefttopleftTransition);
                            lblAnimation1->setObjectName(QStringLiteral("lblAnimation1"));
                            lblAnimation1->setGeometry(QRect(10, 30, 54, 31));
                            lblAnimation1->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img001.jpg")));
                            lblAnimation1->setScaledContents(true);
                            lblAnimation1->setText(QString());

                            lblAnimation1_2 = new QLabel(lefttopleftTransition);
                            lblAnimation1_2->setObjectName(QStringLiteral("lblAnimation1_2"));
                            lblAnimation1_2->setGeometry(QRect(70, 30, 54, 31));
                            lblAnimation1_2->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img002.jpg")));
                            lblAnimation1_2->setScaledContents(true);
                            lblAnimation1_2->setText(QString());

#endif
                            hboxTopTransition->addWidget(lefttopleftTransition);
                        }
                        {
                            // QWidget lefttoprightTransition

                            lefttoprightTransition = new QWidget(horizontalLayoutWidget_2);
                            lefttoprightTransition->setObjectName(QStringLiteral("lefttoprightTransition"));
                            verticalLayoutWidget_2 = new QWidget(lefttoprightTransition);
                            verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
                            verticalLayoutWidget_2->setGeometry(QRect(10, 0, 111, 81));
                            {
                                //// QVBoxLayout vboxTopRightTransition
                                vboxTopRightTransition = new QVBoxLayout(verticalLayoutWidget_2);
                                vboxTopRightTransition->setSpacing(6);
                                vboxTopRightTransition->setContentsMargins(11, 11, 11, 11);
                                vboxTopRightTransition->setObjectName(QStringLiteral("vboxTopRightTransition"));
                                vboxTopRightTransition->setContentsMargins(0, 0, 0, 0);
                                {
                                    // QWidget lefttoprighttopTransition
                                    lefttoprighttopTransition = new QWidget(verticalLayoutWidget_2);
                                    lefttoprighttopTransition->setObjectName(QStringLiteral("lefttoprighttopTransition"));
                                    horizontalLayoutWidget_3 = new QWidget(lefttoprighttopTransition);
                                    horizontalLayoutWidget_3->setObjectName(QStringLiteral("horizontalLayoutWidget_3"));
                                    horizontalLayoutWidget_3->setGeometry(QRect(0, 0, 134, 41));
                                    {
                                        //// QHBoxLayout hboxTopRightTopTransition
                                        hboxTopRightTopTransition = new QHBoxLayout(horizontalLayoutWidget_3);
                                        hboxTopRightTopTransition->setSpacing(6);
                                        hboxTopRightTopTransition->setContentsMargins(11, 11, 11, 11);
                                        hboxTopRightTopTransition->setObjectName(QStringLiteral("hboxTopRightTopTransition"));
                                        hboxTopRightTopTransition->setContentsMargins(0, 0, 0, 0);
                                        lblDurationTrans = new QLabel(horizontalLayoutWidget_3);
                                        lblDurationTrans->setObjectName(QStringLiteral("lblDurationTrans"));
                                        lblDurationTrans->setText(QApplication::translate("MainWindow", "Duration:", 0));
                                        hboxTopRightTopTransition->addWidget(lblDurationTrans);

                                        comboBox = new QComboBox(horizontalLayoutWidget_3);
                                        comboBox->setObjectName(QStringLiteral("comboBox"));
                                        hboxTopRightTopTransition->addWidget(comboBox);
                                    }

                                    vboxTopRightTransition->addWidget(lefttoprighttopTransition);

                                    {
                                        // QWidget
                                        lefttoprightbottomTransition = new QWidget(verticalLayoutWidget_2);
                                        lefttoprightbottomTransition->setObjectName(QStringLiteral("lefttoprightbottomTransition"));
                                        label_3 = new QLabel(lefttoprightbottomTransition);
                                        label_3->setObjectName(QStringLiteral("label_3"));
                                        label_3->setGeometry(QRect(0, 0, 111, 31));
                                        label_3->setText(QApplication::translate("MainWindow", "Apply to all", 0));
                                    }
                                    vboxTopRightTransition->addWidget(lefttoprightbottomTransition);
                                }
                            }
                            hboxTopTransition->addWidget(lefttoprightTransition);
                        }
                    }
                    vboxLeftTransition->addWidget(lefttopTransition);
                    {
                        // QWidget leftbottomTransition

                        leftbottomTransition = new QWidget(verticalLayoutWidget);
                        leftbottomTransition->setObjectName(QStringLiteral("leftbottomTransition"));
                        label = new QLabel(leftbottomTransition);
                        label->setObjectName(QStringLiteral("label"));
                        label->setGeometry(QRect(0, 30, 241, 20));
                        label->setScaledContents(false);
                        label->setAlignment(Qt::AlignCenter);
                        label->setText(QApplication::translate("MainWindow", "Transition", 0));
                    }
                    vboxLeftTransition->addWidget(leftbottomTransition);
                }
            }
            hboxAnimations->addWidget(leftTransition);
        }

        {
            line = new QFrame(horizontalLayoutWidget);
            line->setObjectName(QStringLiteral("line"));
            line->setFrameShape(QFrame::VLine);
            line->setFrameShadow(QFrame::Sunken);

            hboxAnimations->addWidget(line);
        }

        {
            // QWidget rightPanZoom

            rightPanZoom = new QWidget(horizontalLayoutWidget);
            rightPanZoom->setObjectName(QStringLiteral("rightPanZoom"));
            verticalLayoutWidget_3 = new QWidget(rightPanZoom);
            verticalLayoutWidget_3->setObjectName(QStringLiteral("verticalLayoutWidget_3"));
            verticalLayoutWidget_3->setGeometry(QRect(0, 0, 261, 181));
            {
                //// QVBoxLayout vboxRightPanZoom
                vboxRightPanZoom = new QVBoxLayout(verticalLayoutWidget_3);
                vboxRightPanZoom->setSpacing(6);
                vboxRightPanZoom->setContentsMargins(11, 11, 11, 11);
                vboxRightPanZoom->setObjectName(QStringLiteral("vboxRightPanZoom"));
                vboxRightPanZoom->setContentsMargins(0, 0, 0, 0);
                {
                    // QWidget righttopPanZoom
                    righttopPanZoom = new QWidget(verticalLayoutWidget_3);
                    righttopPanZoom->setObjectName(QStringLiteral("righttopPanZoom"));
                    horizontalLayoutWidget_4 = new QWidget(righttopPanZoom);
                    horizontalLayoutWidget_4->setObjectName(QStringLiteral("horizontalLayoutWidget_4"));
                    horizontalLayoutWidget_4->setGeometry(QRect(-1, -1, 261, 91));
                    {
                        //// QHBoxLayout
                        hboxTopPanZoom = new QHBoxLayout(horizontalLayoutWidget_4);
                        hboxTopPanZoom->setSpacing(6);
                        hboxTopPanZoom->setContentsMargins(11, 11, 11, 11);
                        hboxTopPanZoom->setObjectName(QStringLiteral("hboxTopPanZoom"));
                        hboxTopPanZoom->setContentsMargins(0, 0, 0, 0);
                        {
                            // QWidget righttopleftPanZoom
                            righttopleftPanZoom = new QWidget(horizontalLayoutWidget_4);
                            righttopleftPanZoom->setObjectName(QStringLiteral("righttopleftPanZoom"));
                            label_2 = new QLabel(righttopleftPanZoom);
                            label_2->setObjectName(QStringLiteral("label_2"));
                            label_2->setGeometry(QRect(10, 30, 54, 12));
                            label_2->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img003.jpg")));
                            label_2->setScaledContents(true);
                            label_2->setText(QString());
                            label_4 = new QLabel(righttopleftPanZoom);
                            label_4->setObjectName(QStringLiteral("label_4"));
                            label_4->setGeometry(QRect(10, 50, 54, 12));
                            label_4->setPixmap(QPixmap(QString::fromUtf8("../../../QtProjects/qtmovie/jpg/img004.jpg")));
                            label_4->setScaledContents(true);
                            label_4->setText(QString());

                            hboxTopPanZoom->addWidget(righttopleftPanZoom);
                        }
                        {
                            // QWidget righttoprightPanZoom
                            righttoprightPanZoom = new QWidget(horizontalLayoutWidget_4);
                            righttoprightPanZoom->setObjectName(QStringLiteral("righttoprightPanZoom"));
                            lblApplyAllPanzoom = new QLabel(righttoprightPanZoom);
                            lblApplyAllPanzoom->setObjectName(QStringLiteral("lblApplyAllPanzoom"));
                            lblApplyAllPanzoom->setGeometry(QRect(50, 30, 41, 31));
                            lblApplyAllPanzoom->setWordWrap(true);
                            lblApplyAllPanzoom->setText(QApplication::translate("MainWindow", "Apply to all", 0));

                            hboxTopPanZoom->addWidget(righttoprightPanZoom);
                        }
                    }
                    vboxRightPanZoom->addWidget(righttopPanZoom);
                }
                {
                    // QWidget rightbottomPanZoom
                    rightbottomPanZoom = new QWidget(verticalLayoutWidget_3);
                    rightbottomPanZoom->setObjectName(QStringLiteral("rightbottomPanZoom"));
                    lblPanZoom = new QLabel(rightbottomPanZoom);
                    lblPanZoom->setObjectName(QStringLiteral("lblPanZoom"));
                    lblPanZoom->setGeometry(QRect(0, 40, 261, 20));
                    lblPanZoom->setAlignment(Qt::AlignCenter);
                    lblPanZoom->setText(QApplication::translate("MainWindow", "Pan and zoom", 0));

                    vboxRightPanZoom->addWidget(rightbottomPanZoom);
                }
            }
            hboxAnimations->addWidget(rightPanZoom);
        }

        {
            line_2 = new QFrame(horizontalLayoutWidget);
            line_2->setObjectName(QStringLiteral("line_2"));
            line_2->setFrameShape(QFrame::VLine);
            line_2->setFrameShadow(QFrame::Sunken);

            hboxAnimations->addWidget(line_2);
        }

    }
    addTab(tabAnimations, tr("&Animations"));
    setCurrentWidget(tabAnimations);
}
#if 0
void TabWidget::createTabAnimations()
{
    QWidget *tabAnimations = new QWidget;
    QWidget *leftTransition = new QWidget;
    QWidget *rightPanZoom = new QWidget;

    QVBoxLayout *vboxLeftTransition = new QVBoxLayout;
    QWidget *lefttopTransition = new QWidget;
    {
        QHBoxLayout *hboxTopTransition = new QHBoxLayout;
#if 0
        QListWidget *lefttopleftTransition = new QListWidget;
        {
            lefttopleftTransition->setIconSize(m_iconSize);
            lefttopleftTransition->setMinimumHeight(m_iconSize.height() + 250);
            lefttopleftTransition->setMaximumWidth(m_iconSize.width() + 50);
            lefttopleftTransition->setMovement(QListView::Static);

            QListWidgetItem *revealleftItem = new QListWidgetItem();
            QPixmap image("images/reveal_left.png");
            const QSize maximumSize(40, 40); // Reduce in case someone has large photo images.
            if (image.size().width() > maximumSize.width() || image.height() > maximumSize.height())
                image = image.scaled(maximumSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            revealleftItem->setIcon(image);
            revealleftItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            lefttopleftTransition->addItem(revealleftItem);

            connect(revealleftItem, SIGNAL(clicked()), m_elementsEdit, SLOT(selectedTransition()));
        }
        hboxTopTransition->addWidget(lefttopleftTransition);
#endif
#if 0
        QToolButton *lefttopleftTransition = new QToolButton(this);
        {
            //lefttopleftTransition->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            lefttopleftTransition->setIcon(QIcon("images/reveal_left.png"));
            lefttopleftTransition->setIconSize(m_iconSize);
            //lefttopleftTransition->setText("Add photos");
            //lefttopleftTransition->setMaximumHeight(m_iconSize.height() + 250);
            lefttopleftTransition->setMinimumWidth(m_iconSize.width());
            //lefttopleftTransition->setMinimumWidth(lefttopleftTransition->text().length());
            connect(lefttopleftTransition, SIGNAL(clicked()), m_elementsEdit, SLOT(selectedTransition()));
        }
#endif
#if 1
        QLabel *lefttopleftTransition = new QLabel(this);
        QPixmap image("images/reveal_left.png");
        const QSize maximumSize(100, 100); // Reduce in case someone has large photo images.
        if (image.size().width() > maximumSize.width() || image.height() > maximumSize.height())
            image = image.scaled(maximumSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        lefttopleftTransition->setPixmap(image);
#endif
        //hboxTopTransition->addWidget(lefttopleftTransition);
        FlowLayout* flowLayout = new FlowLayout;
        flowLayout->insertWidget(0, lefttopleftTransition);
        hboxTopTransition->addLayout(flowLayout);

        QWidget *lefttoprightTransition = new QWidget;
        {
            //QVBoxLayout *vboxTopTransition = new QVBoxLayout;
            //QWidget *lefttopTransition = new QWidget;
        }
        hboxTopTransition->setMargin(5);
        hboxTopTransition->addWidget(lefttoprightTransition);
        lefttopTransition->setLayout(hboxTopTransition);
    }
    QLabel *leftbottomTransition = new QLabel;
    leftbottomTransition->setText(tr("Transitions"));
    leftbottomTransition->setAlignment(Qt::AlignHCenter);
    vboxLeftTransition->setMargin(5);
    vboxLeftTransition->addWidget(lefttopTransition);
    vboxLeftTransition->addWidget(leftbottomTransition);
    leftTransition->setLayout(vboxLeftTransition);

    QVBoxLayout *vboxRightPanZoom = new QVBoxLayout;
    QWidget *righttopPanZoom = new QWidget;
    {
        QHBoxLayout *hboxTopPanZoom = new QHBoxLayout;
        QListWidget *righttopleftTransition = new QListWidget;
        QWidget *righttoprightTransition = new QWidget;
        hboxTopPanZoom->setMargin(5);
        hboxTopPanZoom->addWidget(righttopleftTransition);
        hboxTopPanZoom->addWidget(righttoprightTransition);
        righttopPanZoom->setLayout(hboxTopPanZoom);
    }
    QLabel *rightbottomPanZoom = new QLabel;
    rightbottomPanZoom->setText(tr("Pan and zoom"));
    rightbottomPanZoom->setAlignment(Qt::AlignHCenter);
    vboxRightPanZoom->setMargin(5);
    vboxRightPanZoom->addWidget(righttopPanZoom);
    vboxRightPanZoom->addWidget(rightbottomPanZoom);
    rightPanZoom->setLayout(vboxRightPanZoom);

    QHBoxLayout *hboxAnimations = new QHBoxLayout;
    hboxAnimations->setMargin(5);
    hboxAnimations->addWidget(leftTransition);
    hboxAnimations->addWidget(rightPanZoom);
    tabAnimations->setLayout(hboxAnimations);

    addTab(tabAnimations, tr("&Animations"));
    setCurrentWidget(tabAnimations);
}
#endif
