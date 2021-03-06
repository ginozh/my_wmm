#include "tabwidget.h"
#include <QLayout>
#include <QScrollArea>
#include "animation.h"
#include <QFontComboBox>
#include <QColorDialog>
#include <QTableWidget>
#include <QMessageBox>
#include "videoscene.h"
#include <QScrollBar>
#include <QToolButton>
#include "element.h"
#include "elementsedit.h"
#include <QDebug>


//tmp
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QApplication>
#include <QAction>
#include <QToolBar>
#include <QPainter>
#include <QMenu>

#ifdef Q_OS_MAC
const QString rsrcPath = "images/mac";
#else
const QString rsrcPath = "images/win";
#endif

TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
}

QSize TabBar::tabSizeHint(int idx) const
{
    GlobalContext* globalContext = GlobalContext::instance();
    if(idx==0)
    {
        //qDebug()<< "TabBar::tabSizeHint. m_iFirstTabSize w: " << globalContext->m_iFirstTabSize.width() << " h: "<<globalContext->m_iFirstTabSize.height();
        return globalContext->m_iFirstTabSize;
    }
    else
    {
        QFont font;
        QFontMetrics fontMetrics(font);
        int fw = fontMetrics.width(tabText(idx));
        int fh = fontMetrics.height();
        return QSize(fw+40*globalContext->m_dFactorX, fh);
        //qDebug()<< "TabBar::tabSizeHint. m_iOtherTabSize w: " << globalContext->m_iOtherTabSize.width() << " h: "<<globalContext->m_iOtherTabSize.height();
        //return globalContext->m_iOtherTabSize;
    }
}
TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
    , m_element(0)
{
    //m_dvStartTimeMusic = NULL;
    //m_dvStartPointMusic = NULL;
    //m_dvEndPointMusic = NULL;

    setTabBar(new TabBar);
    m_globalContext = GlobalContext::instance();
    double dFactorX = m_globalContext->m_dFactorX;
    double dFactorY = m_globalContext->m_dFactorY;
    int iHeight = 240*dFactorY;
    m_iCellHeight = iHeight/4;
    setFixedHeight(iHeight);

    m_iconSize.setWidth(1.5*m_iCellHeight);
    //m_iconSize.setHeight(100*dFactorY);
    m_iconSize.setHeight(1.5*m_iCellHeight);

    QWidget *tabNull = new QWidget;
    addTab(tabNull, tr("   "));
    createTabHome();
    createTabAnimations();
    createTabVisualEffects();
    createTabVideo();
    createTabMusic();
    createTabText();

    setTabEnabled(0, false);
    //setCurrentWidget(contentsWidget);
    setCurrentWidget(m_tabHome);
    //setFixedHeight(m_iconSize.height() + 120*dFactorY);
    //setMaximumHeight(m_iconSize.height() + 120);

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(assignTabValue()));
    connect(this, SIGNAL(elementAttrChanged(int, bool)), (const QObject*)m_globalContext->m_elementsEdit, SLOT(elementAttrChanged(int, bool)));
}
void TabWidget::assignVideoInfo()
{
    Element* element;
    if(m_globalContext && (element=m_globalContext->m_elementsEdit->currentElement()))
    {
        GlobalVideoAttr* globalVideoAttr = element->globalVideoAttr();
        if(globalVideoAttr)
        { 
            m_cbDurationVideo->setCurrentText(QString(tr("%1")).
                    arg(QString::number((float)globalVideoAttr->m_iDuration/1000, 'f', 2)));
        }
    }
}
void TabWidget::assignTabValue()
{
    QWidget * widget= currentWidget();
    qDebug()<< "TabWidget::assignTabValue. widget: "<<(int)widget<<" m_tabVideo: "<<(int)m_tabVideo
        <<" m_tabText: "<<(int)m_tabText<<" m_tabAnimations: "<<(int)m_tabAnimations
        <<" m_tabMusic: "<<(int)m_tabMusic;
    if(widget == m_tabVideo)
    {
        //QMessageBox::information(NULL, "info", QString(tr("video")));
        assignVideoInfo();
    }
    else if(widget == m_tabText)
    {
        assignTextInfo();
    }
    else if(widget == m_tabAnimations)
    {
        assignAnimationInfo();
    }
    else if(widget == m_tabMusic)
    {
        assignMusciInfo();
    }
}
void TabWidget::handleVideoAttrChange()
{
    QWidget* send = qobject_cast<QWidget *>(sender());
    bool isChange = false;
    //bool bCreateSingleVideo = false;
    int attrType = ATTR_NOTHING;
    if(m_globalContext && m_globalContext->m_elementsEdit && m_globalContext->m_elementsEdit->currentElement())
    {
        Element* element = m_globalContext->m_elementsEdit->currentElement();
        GlobalVideoAttr* globalVideoAttr = element->globalVideoAttr();
        GlobalMusicAttr* globalMusicAttr = m_globalContext->m_elementsEdit->globalMusicAttr();
        if(!globalVideoAttr || !globalMusicAttr)
        {
            qDebug()<< "error uncomplete. TabWidget::handleVideoAttrChange: !globalVideoAttr || !globalMusicAtt";
            return;
        }
        if(send == m_tbRotateLeft || send == m_tbRotateRight)
        {
            element->image()->rotate(send == m_tbRotateRight?true:false);
            attrType |= ATTR_ROTATE;
            isChange = true;
        }

#define COMPARE_ASSIGN(macrop_var, macrop_commbox, macrop_globalattr) do {\
        if(send == macrop_commbox && macrop_globalattr) \
        { \
            float i##macrop_var = macrop_commbox->currentText().toFloat(); \
            float m_i##macrop_var = QString::number((float)macrop_globalattr->m_i##macrop_var/1000, 'f', 2).toFloat(); \
            qDebug() << "handleVideoAttrChange. i" #macrop_var ": " << i##macrop_var << " m_i" #macrop_var ": "<< m_i##macrop_var; \
            if(i##macrop_var != m_i##macrop_var) \
            { \
                macrop_globalattr->m_i##macrop_var = i##macrop_var*1000; \
                if((void*)globalVideoAttr == (void*)macrop_globalattr) \
                { \
                    attrType |= ATTR_VIDEO; \
                } \
                if((void*)globalMusicAttr == (void*)macrop_globalattr) \
                { \
                    attrType |= ATTR_MUSIC; \
                } \
                isChange = true; \
            } \
        } \
        else \
        { \
            qDebug() << "TabWidget::handleVideoAttrChange.  i" #macrop_var ": " << macrop_commbox->currentText() << " m_i" #macrop_var ": "<< macrop_globalattr->m_i##macrop_var << " macrop_globalattr: "<<(int)macrop_globalattr; \
        } \
        }while(0)

        COMPARE_ASSIGN(Duration, m_cbDurationVideo, globalVideoAttr);
        //COMPARE_ASSIGN(StartTime, m_cbStartTimeMusic, globalMusicAttr);
        //COMPARE_ASSIGN(StartPoint, m_cbStartPointMusic, globalMusicAttr);
        //COMPARE_ASSIGN(EndPoint, m_cbEndPointMusic, globalMusicAttr);
#define SPINBOX_COMPARE_ASSIGN(macrop_var, macrop_commbox, macrop_globalattr) do {\
        if(send == macrop_commbox && macrop_globalattr) \
        { \
            double i##macrop_var = macrop_commbox->value(); \
            double m_i##macrop_var = QString::number((double)macrop_globalattr->m_i##macrop_var/1000, 'f', 2).toDouble(); \
            qDebug() << "TabWidget::handleVideoAttrChange. i" #macrop_var ": " << i##macrop_var << " m_i" #macrop_var ": "<< m_i##macrop_var; \
            if(i##macrop_var != m_i##macrop_var) \
            { \
                macrop_globalattr->m_i##macrop_var = i##macrop_var*1000; \
                if((void*)globalVideoAttr == (void*)macrop_globalattr) \
                { \
                    /*attrType |= ATTR_VIDEO;*/ \
                } \
                if((void*)globalMusicAttr == (void*)macrop_globalattr) \
                { \
                    attrType |= ATTR_MUSIC; \
                } \
                isChange = true; \
            } \
        } \
        }while(0)
        SPINBOX_COMPARE_ASSIGN(StartTime, m_dsbStartTimeMusic, globalMusicAttr);
        SPINBOX_COMPARE_ASSIGN(StartPoint, m_dsbStartPointMusic, globalMusicAttr);
        SPINBOX_COMPARE_ASSIGN(EndPoint, m_dsbEndPointMusic, globalMusicAttr);

    }
    else
    {
        qDebug()<< "error uncomplete. TabWidget::handleVideoAttrChange: m_globalContext && m_globalContext->m_elementsEdit";
    }
    if(isChange)
    {
        // 通知
        emit elementAttrChanged(attrType, false);
    }
}

void TabWidget::createTabHome()
{
    m_tabHome = new QWidget(this);
    QHBoxLayout *hbox = new QHBoxLayout;
    m_tabHome->setLayout(hbox);
    {
        QVBoxLayout *vboxAdd = new QVBoxLayout;
        //vboxAdd->setGeometry(QRect(0, 0, 3*m_iCellHeight, 4*m_iCellHeight));
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
                //addPhotos->setMaximumHeight(m_iconSize.height() + 250);
                //addPhotos->setMinimumWidth(m_iconSize.width() + 100);
                //addPhotos->setFixedWidth(3*m_iCellHeight);
                //addPhotos->setMinimumWidth(addPhotos->text().length());
                connect(addPhotos, SIGNAL(clicked()), (const QObject*)m_globalContext->m_elementsEdit, SLOT(addImages()));
            }
            {
                QToolButton *addMusic = new QToolButton();
                hboxTop->addWidget(addMusic, Qt::AlignLeft);
                addMusic->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                addMusic->setIcon(QIcon("images/addmusic.png"));
                addMusic->setIconSize(m_iconSize);
                addMusic->setText("Add music");
                //addMusic->setMinimumWidth(m_iconSize.width() + 100);
                //addMusic->setFixedWidth(3*m_iCellHeight);
                connect(addMusic, SIGNAL(clicked()), (const QObject*)m_globalContext->m_elementsEdit, SLOT(addMusic()));
            }
            {
                QVBoxLayout *vboxTopRightAdd = new QVBoxLayout;
                hboxTop->addLayout(vboxTopRightAdd);
                {
                    QToolButton *addTitle = new QToolButton();
                    addTitle->setEnabled(false);
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
                    //addCaption->setMaximumHeight(m_iconSize.height() + 250);
                    //addCaption->setMinimumWidth(m_iconSize.width() + 100);
                    //addCaption->setMinimumWidth(addCaption->text().length());
                    connect(addCaption, SIGNAL(clicked()), m_globalContext->m_elementsEdit, SLOT(addText()));

                }
                {
                    QToolButton *addCredits = new QToolButton();
                    addCredits->setEnabled(false);
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
            //lbl->setFixedWidth(3*m_iCellHeight);

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
                m_tbRotateLeft = new QToolButton();
                hboxTop->addWidget(m_tbRotateLeft, Qt::AlignLeft);

                m_tbRotateLeft->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                m_tbRotateLeft->setIcon(QIcon("images/addphotos.png"));
                m_tbRotateLeft->setIconSize(m_iconSize);
                m_tbRotateLeft->setText("Rotate Left");
                //m_tbRotateLeft->setMaximumHeight(m_iconSize.height() + 250);
                //m_tbRotateLeft->setMinimumWidth(m_iconSize.width() + 100);
                m_tbRotateLeft->setFixedWidth(3*m_iCellHeight);
                //m_tbRotateLeft->setMinimumWidth(m_tbRotateLeft->text().length());
                connect(m_tbRotateLeft, SIGNAL(clicked()), this, SLOT(handleVideoAttrChange()));
            }
            {
                m_tbRotateRight = new QToolButton();
                hboxTop->addWidget(m_tbRotateRight, Qt::AlignLeft);
                m_tbRotateRight->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                m_tbRotateRight->setIcon(QIcon("images/addmusic.png"));
                m_tbRotateRight->setIconSize(m_iconSize);
                m_tbRotateRight->setText("Rotate Right");
                //m_tbRotateRight->setMinimumWidth(m_iconSize.width() + 100);
                m_tbRotateRight->setFixedWidth(3*m_iCellHeight);
                connect(m_tbRotateRight, SIGNAL(clicked()), this, SLOT(handleVideoAttrChange()));
            }
            {
                QVBoxLayout *vboxTopRightEditing = new QVBoxLayout;
                hboxTop->addLayout(vboxTopRightEditing);
                {
                    QToolButton *remove = new QToolButton();
                    remove->setEnabled(false);
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
                    selectAll->setEnabled(false);
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
            //lbl->setFixedWidth(3*m_iCellHeight);

            vboxEditing->addWidget(lbl);
        }
    }
    {
        appendLine(hbox);
    }
    {
        appendExpandingWidget(hbox);
    }
#if 0
    QToolButton *addPhotos = new QToolButton(this);
    addPhotos->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addPhotos->setIcon(QIcon("images/addphotos.png"));
    addPhotos->setIconSize(m_iconSize);
    addPhotos->setText("Add photos");
    //addPhotos->setMaximumHeight(m_iconSize.height() + 250);
    addPhotos->setMinimumWidth(m_iconSize.width() + 100);
    //addPhotos->setMinimumWidth(addPhotos->text().length());
    connect(addPhotos, SIGNAL(clicked()), m_globalContext->m_elementsEdit, SLOT(addImages()));

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
    m_tabHome->setLayout(hboxHome);
    m_tabHome->setMaximumWidth(addPhotos->width() + addMusic->width() + 10);

#endif
    addTab(m_tabHome, tr("&Home"));

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
    m_tabAnimations = new QWidget;
    ////m_tabAnimations->setMinimumHeight(200);
    ////m_tabAnimations->setMaximumHeight(300);
    QHBoxLayout *hboxAnimations = new QHBoxLayout;
    {
        QVBoxLayout *vboxLeftTransition = new QVBoxLayout;
        hboxAnimations->addLayout(vboxLeftTransition);
        {
            QHBoxLayout* hboxTopTransition = new QHBoxLayout;
            vboxLeftTransition->addLayout(hboxTopTransition);
            {
                QScrollArea *scrollArea = new QScrollArea;
                hboxTopTransition->addWidget(scrollArea);
                initialScrollArea(scrollArea);
                {
                    QWidget* animations = new QWidget();
                    {
                        m_flowLayoutTransition = new FlowLayout();
                        //flowLayout->setContentsMargins(10, 10, 10, 10);
                        m_flowLayoutTransition->setContentsMargins(5*m_globalContext->m_dFactorX, 5*m_globalContext->m_dFactorX, 5*m_globalContext->m_dFactorX, 5*m_globalContext->m_dFactorX);
                        {
#define INITIAL_ANIMATION(animation_name, tips_name) do {\
    Animation *animation_name=new Animation("images/"#animation_name".png", ""#animation_name"", tips_name, QSize(1.8*m_iCellHeight, 1.8*m_iCellHeight)) ; \
    connect(animation_name, SIGNAL(selectedAnimationSignal(const QString&)), (const QObject*)m_globalContext->m_elementsEdit, SLOT(selectedTransition(const QString&))); \
    connect(animation_name, SIGNAL(selectedImageSignal(QWidget*)), (const QObject*)m_globalContext->m_elementsEdit, SLOT(selectedImage(QWidget*)));\
    m_flowLayoutTransition->insertWidget(m_flowLayoutTransition->count(), animation_name); \
} while(0)
#if 0
                            Animation *animation=new Animation("images/bowtieh.png", "bowtieh") ;
                            connect(animation, SIGNAL(selectedAnimationSignal(const QString&)), m_globalContext->m_elementsEdit, SLOT(selectedTransition(const QString&)));

                            flowLayout->insertWidget(0, animation);
#endif
                            INITIAL_ANIMATION( none, "No transition");
                            INITIAL_ANIMATION( bowtieh, "bow tie - horizontal");
                            INITIAL_ANIMATION( bowtiev, "bow tie - vertical");
                            INITIAL_ANIMATION( diagboxout, "diagonal - box out");
                            INITIAL_ANIMATION( diagcrossout, "diagonal - cross out");
                            INITIAL_ANIMATION( diagdownright, "diagonal down-right");
                            INITIAL_ANIMATION( filledvdown, "filled v down");
                            INITIAL_ANIMATION( filledvleft, "filled v left");
                            INITIAL_ANIMATION( filledvright, "filled v right");
                            INITIAL_ANIMATION( filledvup, "filled v up");
                            INITIAL_ANIMATION( barsh, "bars - horizontal");
                            INITIAL_ANIMATION( barsvertical, "bars - vertical");
                            INITIAL_ANIMATION( crossfade, "crossfade");
                            INITIAL_ANIMATION( checkerb, "checkerboard");
                            INITIAL_ANIMATION( circle, "circle");
                            INITIAL_ANIMATION( circles, "circles");
                            INITIAL_ANIMATION( diamond, "diamond");
                            INITIAL_ANIMATION( heart, "heart");
                            INITIAL_ANIMATION( rectangle, "rectangle");
                            INITIAL_ANIMATION( wheel, "wheel");
                            INITIAL_ANIMATION( insetbottoml, "inset bottom-left");
                            INITIAL_ANIMATION( insetbottomr, "inset bottom-right");
                            INITIAL_ANIMATION( insettopleft, "inset top-left");
                            INITIAL_ANIMATION( insettopr, "inset top-right");
                            INITIAL_ANIMATION( iris, "iris");
                            INITIAL_ANIMATION( revealdown, "reveal down");
                            INITIAL_ANIMATION( revealright, "reveal right");
                            INITIAL_ANIMATION( revealleft, "reveal left");
                            INITIAL_ANIMATION( roll, "roll");
                            INITIAL_ANIMATION( slide, "slide");
                            INITIAL_ANIMATION( slideupt, "slide down together");
                            //INITIAL_ANIMATION( slidedownt, "");
                            INITIAL_ANIMATION( splith, "split horizontal");
                            INITIAL_ANIMATION( splitv, "split vertical");
                            INITIAL_ANIMATION( fanin, "fan in");
                        }
#undef INITIAL_ANIMATION
                        animations->setLayout(m_flowLayoutTransition);
                    }
                    scrollArea->setWidget(animations);
                }

            }
            {
                QVBoxLayout *vboxTopRightTransition = new QVBoxLayout;
                hboxTopTransition->addLayout(vboxTopRightTransition);
                {
                    QHBoxLayout *hboxTopRightTopTransition = new QHBoxLayout;
                    vboxTopRightTransition->addLayout(hboxTopRightTopTransition);
                    {
                        QLabel *lblDurationTrans = new QLabel(tr("Duration: "));

                        hboxTopRightTopTransition->addWidget(lblDurationTrans);
                    }
                    {
                        QComboBox* comboBox = new QComboBox();
                        comboBox->setEditable(true);

                        hboxTopRightTopTransition->addWidget(comboBox);
                    }

                }
                {
                    QAbstractButton *btnApplyTrans = new QPushButton(tr("Apply to all"));
                    btnApplyTrans->setEnabled(false);
                    //connect(btnApplyTrans, SIGNAL(clicked()), m_globalContext->m_elementsEdit, SLOT(openFile()));
                    //btnApplyTrans->setAlignment(Qt::AlignCenter);

                    vboxTopRightTransition->addWidget(btnApplyTrans);
                }

            }

        }
        {
            QLabel *lbl = new QLabel(tr("Transition"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxLeftTransition->addWidget(lbl);
        }

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
                        m_flowLayoutPanZoom = new FlowLayout();
                        //m_flowLayoutPanZoom->setContentsMargins(10, 10, 10, 10);
                        m_flowLayoutPanZoom->setContentsMargins(5*m_globalContext->m_dFactorX, 5*m_globalContext->m_dFactorX, 5*m_globalContext->m_dFactorX, 5*m_globalContext->m_dFactorX);
                        {
#define INITIAL_ANIMATION(animation_name, tips_name) do {\
    Animation *animation_name=new Animation("images/"#animation_name".png", ""#animation_name"", tips_name, QSize(1.8*m_iCellHeight, 1.8*m_iCellHeight)) ; \
    connect(animation_name, SIGNAL(selectedAnimationSignal(const QString&)), (const QObject*)m_globalContext->m_elementsEdit, SLOT(selectedPanZoom(const QString&))); \
    connect(animation_name, SIGNAL(selectedImageSignal(QWidget*)), (const QObject*)m_globalContext->m_elementsEdit, SLOT(selectedImage(QWidget*)));\
    m_flowLayoutPanZoom->insertWidget(m_flowLayoutPanZoom->count(), animation_name); \
} while(0)
                            INITIAL_ANIMATION( none, "No pan and zoom");
                            INITIAL_ANIMATION( panupleft, "Pan up along left");
                            INITIAL_ANIMATION( panup, "Pan up");
                            INITIAL_ANIMATION( panupright, "Pan up along right");
                            INITIAL_ANIMATION( pandownleft, "Pan down along left");
                            INITIAL_ANIMATION( pandown, "Pan down");
                            INITIAL_ANIMATION( pandownright, "Pan down along right");
                            INITIAL_ANIMATION( panrighttop, "Pan right along top");
                            //INITIAL_ANIMATION( panright, "Pan right");
                            //INITIAL_ANIMATION( panrightbottom, "Pan right along bottom");
                            //INITIAL_ANIMATION( panlefttop, "Pan left along top");
                            //INITIAL_ANIMATION( panleft, "Pan left");
                            //INITIAL_ANIMATION( panleftbottom, "Pan left along bottom");
                            INITIAL_ANIMATION( zoomincenter, "Zoom in center");
                            INITIAL_ANIMATION( zoomincenterrotateright, "Zoom in center and rotate right");
                            INITIAL_ANIMATION( zoomincenterrotateleft, "Zoom in center and rotate left");
                            INITIAL_ANIMATION( zoomintopleft, "Zoom in top-left");
                            INITIAL_ANIMATION( zoomintop, "Zoom in top");
                            INITIAL_ANIMATION( zoomintopright, "Zoom in top-right");
                            INITIAL_ANIMATION( zoominright, "Zoom in right");
                            INITIAL_ANIMATION( zoominbottomright, "Zoom in bottom-right");
                            INITIAL_ANIMATION( zoominbottom, "Zoom in bottom");
                            INITIAL_ANIMATION( zoominbottomleft, "Zoom in bottom-left");
                            INITIAL_ANIMATION( zoominleft, "Zoom in left");
                            INITIAL_ANIMATION( zoomoutcenter, "Zoom out center");
                            INITIAL_ANIMATION( zoomouttopleft, "Zoom out top-left");
                            INITIAL_ANIMATION( zoomouttop, "Zoom out top");
                            INITIAL_ANIMATION( zoomouttopright, "Zoom out top-right");
                            INITIAL_ANIMATION( zoomoutright, "Zoom out right");
                            INITIAL_ANIMATION( zoomoutbottomright, "Zoom out bottom-right");
                            INITIAL_ANIMATION( zoomoutbottom, "Zoom out bottom");
                            INITIAL_ANIMATION( zoomoutbottomleft, "Zoom out bottom-left");
                            INITIAL_ANIMATION( zoomoutleft, "Zoom out left");
                        }
                        panzooms->setLayout(m_flowLayoutPanZoom);
                    }
#undef INITIAL_ANIMATION

                    scrollArea->setWidget(panzooms);
                }

                hboxTopPanZoom->addWidget(scrollArea);
            }
            {
                QAbstractButton *btnApplyPanzoom = new QPushButton(tr("Apply to all"));
                btnApplyPanzoom->setEnabled(false);
                //connect(btnApplyPanzoom, SIGNAL(clicked()), m_globalContext->m_elementsEdit, SLOT(openFile()));
                //btnApplyPanzoom->setAlignment(Qt::AlignCenter);

                hboxTopPanZoom->addWidget(btnApplyPanzoom);
            }
            vboxRightPanZoom->addLayout(hboxTopPanZoom);
        }
        {
            QLabel *lbl = new QLabel(tr("Pan and zoom"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxRightPanZoom->addWidget(lbl);
        }

        hboxAnimations->addLayout(vboxRightPanZoom);
    }
    {
        appendLine(hboxAnimations);
    }
    {
        //appendExpandingWidget(hboxAnimations);
    }

    m_tabAnimations->setLayout(hboxAnimations);

    addTab(m_tabAnimations, tr("&Animations"));
}
void TabWidget::createTabVisualEffects()
{
}
/*
------------------------------------ |
|          |Speed:       ComboBox  | |
|Backgroup |-----------------------| |
|Color     |Duration :   ComboBox  | |
|----------------------------------| |
|              Adjust              | |
|----------------------------------| |
*/
void TabWidget::createTabVideo()
{
    m_tabVideo = new QWidget;
    QHBoxLayout *hbox = new QHBoxLayout;
    m_tabVideo->setLayout(hbox);
    {
        QVBoxLayout *vboxAdjust = new QVBoxLayout;
        hbox->addLayout(vboxAdjust);

        {
            QHBoxLayout* hboxTop = new QHBoxLayout;
            vboxAdjust->addLayout(hboxTop);
            {
                // Backgroup color
            }

            {
                QVBoxLayout *vboxTopright = new QVBoxLayout;
                hboxTop->addLayout(vboxTopright);
                {
                    QHBoxLayout* hboxSpeed = new QHBoxLayout;
                    vboxTopright->addLayout(hboxSpeed);
                    {
                        QLabel *lbl = new QLabel(tr("Speed(s):"));
                        lbl->setEnabled(false);

                        hboxSpeed->addWidget(lbl);
                    }
                    {
                        m_cbSpeedVideo = new QComboBox();
                        m_cbSpeedVideo->setEnabled(false);

                        hboxSpeed->addWidget(m_cbSpeedVideo);
                    }

                }
                {
                    QHBoxLayout* hboxDuration = new QHBoxLayout;
                    vboxTopright->addLayout(hboxDuration);
                    {
                        QLabel *lbl = new QLabel(tr("Duration(s):"));

                        hboxDuration->addWidget(lbl);
                    }
                    {
                        m_cbDurationVideo = new ComboBox();
                        m_cbDurationVideo->setEnabled(false);
                        m_cbDurationVideo->setEditable(true);
                        m_cbDurationVideo->setCurrentText(QString(tr("2.00")));
                        m_cbDurationVideo->addItem(QString(tr("2.00")));
                        m_dvDurationVideo = new QDoubleValidator();
                        m_dvDurationVideo->setRange(0.5, 30.0, 2);
                        m_dvDurationVideo->setNotation(QDoubleValidator::StandardNotation);
                        m_cbDurationVideo->setValidator(m_dvDurationVideo); //uncomplete delete old
                        connect(m_cbDurationVideo, SIGNAL(textChangedSignal(QString)),
                                this, SLOT(handleVideoAttrChange()));

                        hboxDuration->addWidget(m_cbDurationVideo);
                    }

                }
            }
        }
        {
            QLabel *lbl = new QLabel(tr("Adjust"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxAdjust->addWidget(lbl);
        }
    }
    {
        appendLine(hbox);
    }
    {
        appendExpandingWidget(hbox);
    }

    addTab(m_tabVideo, tr("&VideoTools"));
}
/*
-------------------------------
|     |Start time:  ComboBox  | |
|     |-----------------------| |
|split|Start point: ComboBox  | |
|     |-----------------------| |
|     |End point:   ComboBox  | |
|-----------------------------| |
|           Editing           | |
|-----------------------------| |
*/
void TabWidget::createTabMusic()
{
    m_tabMusic = new QWidget;
    QHBoxLayout *hbox = new QHBoxLayout;
    m_tabMusic->setLayout(hbox);
    {
        QVBoxLayout *vboxEditing = new QVBoxLayout;
        hbox->addLayout(vboxEditing);

        {
            QHBoxLayout* hboxTop = new QHBoxLayout;
            vboxEditing->addLayout(hboxTop);
            {
                // split
            }

            {
                QVBoxLayout *vboxTimes = new QVBoxLayout;
                hboxTop->addLayout(vboxTimes);

#define INIT_SPINBOX(var_hlayout, var_spinbox) do{ \
                        var_spinbox = new SpinBox; \
                        var_spinbox->setSuffix("s"); \
                        var_spinbox->setDecimals(2); \
                        var_spinbox->setRange(0.0, 0.0); \
                        connect(var_spinbox, SIGNAL(valueChangedSignal(double)), \
                                this, SLOT(handleVideoAttrChange())); \
                        var_hlayout->addWidget(var_spinbox); \
    }while(0)
                {
                    QHBoxLayout* hboxStartTime = new QHBoxLayout;
                    vboxTimes->addLayout(hboxStartTime);
                    {
                        QLabel *lbl = new QLabel(tr("Start time(s):"));

                        hboxStartTime->addWidget(lbl);
                    }
                    {
                        INIT_SPINBOX(hboxStartTime, m_dsbStartTimeMusic);
                    }

                }
                {
                    QHBoxLayout* hboxStartPoint = new QHBoxLayout;
                    vboxTimes->addLayout(hboxStartPoint);
                    {
                        QLabel *lbl = new QLabel(tr("Start point(s):"));

                        hboxStartPoint->addWidget(lbl);
                    }
                    {
                        INIT_SPINBOX(hboxStartPoint, m_dsbStartPointMusic);
#if 0
                        m_cbStartPointMusic->setEditable(true);
                        m_cbStartPointMusic->addItem(QString(tr("0.00")));
                        m_cbStartPointMusic->setCurrentText(QString(tr("0.00")));
                        connect(m_cbStartPointMusic, SIGNAL(textChangedSignal(QString)),
                                this, SLOT(handleVideoAttrChange()));
                        hboxStartPoint->addWidget(m_cbStartPointMusic);
#endif
                    }

                }
                {
                    QHBoxLayout* hboxEndPoint = new QHBoxLayout;
                    vboxTimes->addLayout(hboxEndPoint);
                    {
                        QLabel *lbl = new QLabel(tr("End point(s):"));

                        hboxEndPoint->addWidget(lbl);
                    }
                    {
                        INIT_SPINBOX(hboxEndPoint, m_dsbEndPointMusic);
                    }

                }
            }
        }
        {
            QLabel *lbl = new QLabel(tr("Editing"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxEditing->addWidget(lbl);
        }
    }
    {
        appendLine(hbox);
    }
    {
        appendExpandingWidget(hbox);
    }

    addTab(m_tabMusic, tr("&MusicTools"));
}
/*
/c/QtProjects/QtExamples/widgets/graphicsview/diagramscene/mainwindow.cpp
--------------------------------------------------------------
|   font  | size | Transparency |starttime  |                |
|----------------|--------------|-----------|                |
|bond  color     | left center r| duration  |                |
|----------------|--------------|-----------|----------------|
|     Font       |   Paragraph  |  Adjust   |     Effects    |
|----------------|--------------|-----------|----------------|
*/
void TabWidget::createTabText()
{
    m_tabText = new QWidget;
    QHBoxLayout *hbox = new QHBoxLayout;
    {
        QVBoxLayout *vboxFont = new QVBoxLayout;
        hbox->addLayout(vboxFont);
        {
            QHBoxLayout* hboxTopFont = new QHBoxLayout;
            vboxFont->addLayout(hboxTopFont);
            {
                m_fontCombo = new QFontComboBox();
                connect(m_fontCombo, SIGNAL(currentFontChanged(QFont)),
                        this, SLOT(handleFontChange()));

                hboxTopFont->addWidget(m_fontCombo);
            }
            {
                m_fontSizeCombo = new QComboBox;
                m_fontSizeCombo->setEditable(true);
                for (int i = 8; i < 30; i = i + 2)
                    m_fontSizeCombo->addItem(QString().setNum(i));
                QIntValidator *validator = new QIntValidator(2, 64, this);
                m_fontSizeCombo->setValidator(validator);
                connect(m_fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
                        this, SLOT(handleFontChange()));

                hboxTopFont->addWidget(m_fontSizeCombo);
            }

        }
        {
            QHBoxLayout* hboxMidFont = new QHBoxLayout;
            vboxFont->addLayout(hboxMidFont);
            //hboxMidFont->setMargin(5);
            {
                m_boldButton = new QToolButton(this);
                m_boldButton->setCheckable(true);

                //m_boldButton->setChecked(true);
                m_boldButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
                m_boldButton->setIcon(QIcon("images/bold.png"));
                connect(m_boldButton, SIGNAL(clicked(bool)),
                        this, SLOT(handleFontChange()));

                hboxMidFont->addWidget(m_boldButton);
            }
            {
                m_italicButton = new QToolButton(this);
                m_italicButton->setCheckable(true);
                m_italicButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
                m_italicButton->setIcon(QIcon("images/italic.png"));
                connect(m_italicButton, SIGNAL(clicked(bool)),
                        this, SLOT(handleFontChange()));

                hboxMidFont->addWidget(m_italicButton);
            }
            {
                m_underlineButton = new QToolButton;
                m_underlineButton->setCheckable(true);
                m_underlineButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
                m_underlineButton->setIcon(QIcon("images/underline.png"));
                connect(m_underlineButton, SIGNAL(clicked(bool)),
                        this, SLOT(handleFontChange()));

                hboxMidFont->addWidget(m_underlineButton);
            }
            {
                // c:\QtProjects\QtExamples\widgets\richtext\textedit\debug
                //widgets/richtext/textedit
                // c:\QtProjects\QtExamples\widgets\dialogs\standarddialogs\debug
                //widgets/dialogs/standarddialogs
                // c:\QtProjects\QtExamples\widgets\graphicsview\boxes\debug
                //widgets/graphicsview/boxes/scene.cpp
#if 0
                m_colorEdit = new ColorEdit(Qt::white, 1);
                hboxMidFont->addWidget(m_colorEdit);
#endif
                fontColorToolButton = new QToolButton;
                hboxMidFont->addWidget(fontColorToolButton);
                fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
                fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
                textAction = fontColorToolButton->menu()->defaultAction();
                fontColorToolButton->setIcon(createColorToolButtonIcon("images/textpointer.png", Qt::black));
                fontColorToolButton->setAutoFillBackground(true);
                //connect(fontColorToolButton, SIGNAL(clicked()), this, SLOT(textButtonTriggered()));
                connect(fontColorToolButton, SIGNAL(clicked()), this, SLOT(handleFontChange()));

            }

        }
        {
            QLabel *lbl = new QLabel(tr("Font"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxFont->addWidget(lbl);
        }

    }
    {
        appendLine(hbox);
    }
    {
        // /c/QtProjects/QtExamples/widgets/richtext/textedit/textedit.cpp
        QVBoxLayout *vboxParagraph = new QVBoxLayout;
        hbox->addLayout(vboxParagraph);
        {
            //QHBoxLayout* hboxTopParagraph = new QHBoxLayout;
            QLabel *lbl = new QLabel(tr("Transparency"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxParagraph->addWidget(lbl);

            //vboxParagraph->addLayout(hboxTopParagraph);
        }
        {
            QHBoxLayout* hboxMidParagraph = new QHBoxLayout;
            vboxParagraph->addLayout(hboxMidParagraph);
            hboxMidParagraph->setMargin(5);
            {
                m_leftTextButton = new QToolButton(this);
                m_leftTextButton->setCheckable(true);
                m_leftTextButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
                const QIcon leftIcon = QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png"));
#if 0
                QAction* actionAlignLeft = new QAction(leftIcon, tr("&Left"), m_leftTextButton);
                actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
                actionAlignLeft->setCheckable(true);
                actionAlignLeft->setPriority(QAction::LowPriority);
                m_leftTextButton->addAction(actionAlignLeft);
#endif
                m_leftTextButton->setIcon(leftIcon);
                ////connect(m_leftTextButton, SIGNAL(clicked()), this , SLOT(handleFontChange()));

                hboxMidParagraph->addWidget(m_leftTextButton);
            }
            {
                m_centerTextButton = new QToolButton(this);
                m_centerTextButton->setCheckable(true);
                m_centerTextButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
                const QIcon centerIcon = QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png"));
                m_centerTextButton->setIcon(centerIcon);

                hboxMidParagraph->addWidget(m_centerTextButton);
            }
            {
                m_rightTextButton = new QToolButton(this);
                m_rightTextButton->setCheckable(true);
                m_rightTextButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
                const QIcon rightIcon = QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png"));
                m_rightTextButton->setIcon(rightIcon);

                hboxMidParagraph->addWidget(m_rightTextButton);
            }
            {
                m_paragraphTextButtonG = new QButtonGroup(hboxMidParagraph);
                m_paragraphTextButtonG->addButton(m_leftTextButton);
                m_paragraphTextButtonG->setId(m_leftTextButton, Qt::AlignLeft);
                m_paragraphTextButtonG->addButton(m_centerTextButton);
                m_paragraphTextButtonG->setId(m_centerTextButton, Qt::AlignHCenter);
                m_paragraphTextButtonG->addButton(m_rightTextButton);
                m_paragraphTextButtonG->setId(m_rightTextButton, Qt::AlignRight);

                m_centerTextButton->setChecked(true);

                //connect(m_paragraphTextButtonG, SIGNAL(buttonClicked(int)), this, SLOT(paragraphGroupClicked(int)));
                connect(m_paragraphTextButtonG, SIGNAL(buttonClicked(int)), this, SLOT(handleFontChange()));

                //hboxMidParagraph->addWidget(m_paragraphTextButtonG);
            }

        }
        {
            QLabel *lbl = new QLabel(tr("Paragraph"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxParagraph->addWidget(lbl);
        }

    }
    {
        appendLine(hbox);
    }
    {
        QVBoxLayout *vboxAdjust = new QVBoxLayout;
        hbox->addLayout(vboxAdjust);
        {
            QHBoxLayout* hboxAdjust = new QHBoxLayout;
            vboxAdjust->addLayout(hboxAdjust);
            {
                QToolButton *editextBtn = new QToolButton(this);
                editextBtn->setEnabled(false);
                editextBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                editextBtn->setIcon(QIcon("images/edittext.png"));
                //editextBtn->setIcon(QIcon("images/textpointer.png"));
                editextBtn->setIconSize(m_iconSize);
                editextBtn->setText("Edit text");
                hboxAdjust->addWidget(editextBtn, Qt::AlignLeft);
            }
            {
                QVBoxLayout *vboxTopAdjust = new QVBoxLayout;
                hboxAdjust->addLayout(vboxTopAdjust);
                {
                    QHBoxLayout* hboxTopAdjust = new QHBoxLayout;
                    {
                        QLabel *lbl = new QLabel(tr("Start time(s):"));

                        hboxTopAdjust->addWidget(lbl);
                    }
                    {
                        m_startTimeTextCombo = new QComboBox();
                        m_startTimeTextCombo->setEnabled(false);
                        m_startTimeTextCombo->setEditable(true);
                        m_startTimeTextCombo->addItem(QString(tr("2.00")));
                        m_startTimeTextCombo->setCurrentText(QString(tr("2.00")));

                        hboxTopAdjust->addWidget(m_startTimeTextCombo);
                    }

                    vboxTopAdjust->addLayout(hboxTopAdjust);
                }
                {
                    QHBoxLayout* hboxMidAdjust = new QHBoxLayout;
                    {
                        QLabel *lbl = new QLabel(tr("Text duration(s):"));

                        hboxMidAdjust->addWidget(lbl);
                    }
                    {
                        m_durationTextCombo = new QComboBox();
                        m_durationTextCombo->setEnabled(false);
                        m_durationTextCombo->setEditable(true);
                        for (int i = 2; i < 30; i = i + 2)
                            m_durationTextCombo->addItem(QString().setNum(i));

                        hboxMidAdjust->addWidget(m_durationTextCombo);
                    }

                    vboxTopAdjust->addLayout(hboxMidAdjust);
                }
            }
        }
        {
            QLabel *lbl = new QLabel(tr("Adjust"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxAdjust->addWidget(lbl);
        }
    }
    {
        appendLine(hbox);
    }
    {
        QVBoxLayout *vboxEffects = new QVBoxLayout;
        hbox->addLayout(vboxEffects);
        {
            QScrollArea *scrollArea = new QScrollArea;
            initialScrollArea(scrollArea);
            {
                QWidget* effects = new QWidget();
                {
                    FlowLayout* flowLayout = new FlowLayout();
                    flowLayout->setContentsMargins(5*m_globalContext->m_dFactorX, 5*m_globalContext->m_dFactorX, 5*m_globalContext->m_dFactorX, 5*m_globalContext->m_dFactorX);
                    {
#define INITIAL_TEXT(animation_name, tips_name) do {\
    Animation *animation_name=new Animation("images/"#animation_name".png", ""#animation_name"", tips_name, m_iconSize) ; \
    animation_name->setEnabled(false); \
    /*connect(animation_name, SIGNAL(selectedAnimationSignal(const QString&)), (const QObject*)m_globalContext->m_elementsEdit, SLOT(selectedTransition(const QString&)));*/ \
    flowLayout->insertWidget(flowLayout->count(), animation_name); \
} while(0)
                        INITIAL_TEXT(none, "none");
                        INITIAL_TEXT(fade, "fade");
                        INITIAL_TEXT(stretch, "stretch");
                        INITIAL_TEXT(spinin, "spin in");
                        INITIAL_TEXT(spinout, "spin out");
                        INITIAL_TEXT(flyinleft, "fly in -left");
                        INITIAL_TEXT(flyinright, "fly in -right");
                        INITIAL_TEXT(scroll, "scroll");
                        INITIAL_TEXT(swingdown, "swing down");
                        INITIAL_TEXT(zoominsmall, "zoom in - small");
                        INITIAL_TEXT(zoominbig, "zoom in - big");
                    }

                    effects->setLayout(flowLayout);
                }
                scrollArea->setWidget(effects);
            }

            vboxEffects->addWidget(scrollArea);
        }
        {
            QLabel *lbl = new QLabel(tr("Effects"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxEffects->addWidget(lbl);
        }
    }
    {
        appendLine(hbox);
    }
    {
        appendExpandingWidget(hbox);
    }

    m_tabText->setLayout(hbox);

    addTab(m_tabText, tr("&TextTools"));
}
void TabWidget::appendLine(QHBoxLayout *hbox)
{
    QFrame *line = new QFrame();
    line->setObjectName(QStringLiteral("line"));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    hbox->addWidget(line);
}
//增加伸缩因子的空qwidget：http://www.cnblogs.com/alleyonline/p/4631605.html
void TabWidget::appendExpandingWidget(QHBoxLayout *hbox)
{
    QWidget *nullWidget = new QWidget;
    nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hbox->addWidget(nullWidget);
}
void TabWidget::initialScrollArea(QScrollArea *scrollArea)
{
    scrollArea->setBackgroundRole(QPalette::Light);
    scrollArea->setMinimumWidth(500*GlobalContext::instance()->m_dFactorX);
    ////scrollArea->setMinimumHeight(120);
    ////scrollArea->setMaximumHeight(120);
    //scrollArea->setContentsMargins(10, 10, 10, 10);
    scrollArea->setContentsMargins(0, 0, 0, 0);
    scrollArea->setWidgetResizable (true);
    QScrollBar *scrollBar = scrollArea->verticalScrollBar();
    scrollBar->setSingleStep(100);
}
QIcon TabWidget::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 60);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}
QMenu *TabWidget::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}
QIcon TabWidget::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
void TabWidget::activeTabVideo(void* element, GlobalVideoAttr* globalVideoAttr)
{
    m_element = element;
    setCurrentWidget(m_tabVideo);
}
void TabWidget::activeTabText(void* velement)
{
    setCurrentWidget(m_tabText);
    assignTextInfo();
}
#if 0
void TabWidget::activeTabText(void* element)
{
    m_element = element;
    //激活tabtext
    setCurrentWidget(m_tabText);
    stTextAttr *textItem;
    if(m_mapText.contains(element))
    {
        textItem=m_mapText[element];
    }
    else
    {
        textItem = new stTextAttr;
        m_mapText.insert(element, textItem);
    }
    //赋值. 将element对应的保存的text值填充到各个控件中
    assignTabWidget(textItem);
}
#endif
void TabWidget::activeTabMusic(GlobalMusicAttr* musicAttr)
{
    setCurrentWidget(m_tabMusic);
    if(!musicAttr)
    {
        qDebug()<< "TabWidget::activeTabMusic. musicAttr is null";
        return;
    }
    assignMusciInfo();
}
void TabWidget::assignAnimationInfo()
{
    int idx;
    if((idx=m_globalContext->m_elementsEdit->currentElementIdx()) <= 0)
    {
        for (int i = 0; i < m_flowLayoutTransition->count(); ++i)
        {
            QWidget *element = m_flowLayoutTransition->itemAt(i)->widget();
            if (!element)
            {
                //uncomplete
                continue;
            }
            element->setVisible(false);
        }
        for (int i = 0; i < m_flowLayoutPanZoom->count(); ++i)
        {
            QWidget *element = m_flowLayoutPanZoom->itemAt(i)->widget();
            if (!element)
            {
                //uncomplete
                continue;
            }
            element->setVisible(idx == 0? true:false);
        }
        return;
    }
    else
    {
        for (int i = 0; i < m_flowLayoutTransition->count(); ++i)
        {
            QWidget *element = m_flowLayoutTransition->itemAt(i)->widget();
            if (!element)
            {
                //uncomplete
                continue;
            }
            element->setVisible(true);
        }
        for (int i = 0; i < m_flowLayoutPanZoom->count(); ++i)
        {
            QWidget *element = m_flowLayoutPanZoom->itemAt(i)->widget();
            if (!element)
            {
                //uncomplete
                continue;
            }
            element->setVisible(true);
        }
    }
    Element* element;
    if(m_globalContext && (element=m_globalContext->m_elementsEdit->currentElement()))
    {
        GlobalAnimationAttr* globalAnimationAttr = element->globalAnimationAttr();
        if(globalAnimationAttr)
        {
            //uncomplete
        }
        else
        {
            //uncomplete
        }
    }
    else
    {
    }
}
void TabWidget::assignMusciInfo()
{
    GlobalMusicAttr* globalMusicAttr = m_globalContext->m_elementsEdit->globalMusicAttr();
    if(!globalMusicAttr)
    {
        qDebug()<< "error uncomplete. TabWidget::handleVideoAttrChange: !globalVideoAttr || !globalMusicAtt";
        return;
    }
    double fStartTime =(double)globalMusicAttr->m_iStartTime/1000;
    double fStartPoint = (double)globalMusicAttr->m_iStartPoint/1000;
    double fEndPoint = (double)globalMusicAttr->m_iEndPoint/1000;
    double fMusicDuration = (double)globalMusicAttr->m_iMusicDuration/1000;
    double fVideoDuration =(double)m_globalContext->m_elementsEdit->totalVideoDuration()/1000;
    //widgets/widgets/validators
    //focusOutEvent
#if 0
    if(m_dvStartTimeMusic)
    {
        delete m_dvStartTimeMusic;
    }
    m_dvStartTimeMusic = new QDoubleValidator();
    m_dvStartTimeMusic->setRange(0.0, fStartTime, 2);
    m_dvStartTimeMusic->setNotation(QDoubleValidator::StandardNotation);
    m_cbStartTimeMusic->setValidator(m_dvStartTimeMusic); //uncomplete delete old
#endif
    //1, validate
    m_dsbStartTimeMusic->setRange(0.0, fVideoDuration);
    m_dsbStartPointMusic->setRange(0.0, fMusicDuration);
    m_dsbEndPointMusic->setRange(0.0, fMusicDuration);
    //m_dsbStartTimeMusic->setRange(0.0, 8.00);
#if 0
    if(m_dvStartPointMusic)
    {
        delete m_dvStartPointMusic;
    }
    m_dvStartPointMusic = new QDoubleValidator();
    m_dvStartPointMusic->setRange(0.0, fEndPoint, 2);
    m_dvStartPointMusic->setNotation(QDoubleValidator::StandardNotation);
    m_cbStartPointMusic->setValidator(m_dvStartPointMusic); //uncomplete delete old

    if(m_dvEndPointMusic)
    {
        delete m_dvEndPointMusic;
    }
    m_dvEndPointMusic = new QDoubleValidator();
    m_dvEndPointMusic->setRange(0.0, fEndPoint, 2);
    m_dvEndPointMusic->setNotation(QDoubleValidator::StandardNotation);
    m_cbEndPointMusic->setValidator(m_dvEndPointMusic); //uncomplete delete old
#endif
    //m_cbStartTimeMusic->setValidator(new QDoubleValidator(0.0, fStartTime, 2, m_cbStartTimeMusic)); //uncomplete delete old
    //m_cbStartPointMusic->setValidator(new QDoubleValidator(0.0, fEndPoint, 2, m_cbStartPointMusic)); //uncomplete delete old
    //m_cbEndPointMusic->setValidator(new QDoubleValidator(0.0, fEndPoint, 2, m_cbEndPointMusic)); //uncomplete delete old

    //2, assgin
    m_dsbStartTimeMusic->setValue(fStartTime);
    m_dsbStartPointMusic->setValue(fStartPoint);
    if(fEndPoint == 0)
    {
        fEndPoint = fMusicDuration;
    }
    m_dsbEndPointMusic->setValue(fEndPoint);

    qDebug()<< "TabWidget::assignMusciInfo. fStartTime: "<<fStartTime<<" fStartPoint: "<<fStartPoint
        <<" fEndPoint: "<<fEndPoint<<" fMusicDuration: "<<fMusicDuration<<" fVideoDuration:"<<fVideoDuration;
}
void TabWidget::assignTextInfo()
{
    Element* element;
    if(m_globalContext && (element=m_globalContext->m_elementsEdit->currentElement()))
    {
        GlobalTextAttr* globalTextAttr = element->globalTextAttr();
        if(globalTextAttr)
        {
            m_fontCombo->setFont(globalTextAttr->m_qfont);
            m_fontSizeCombo->setCurrentText(QString::number(globalTextAttr->m_qfont.pointSize()));
            m_boldButton->setChecked(globalTextAttr->m_qfont.weight()==QFont::Bold?true:false);
            m_italicButton->setChecked(globalTextAttr->m_qfont.italic()?true:false);
            m_underlineButton->setChecked(globalTextAttr->m_qfont.underline()?true:false);
#if 0
            m_fontSizeCombo->setCurrentText(globalTextAttr->m_fontSize);
            m_boldButton->setChecked(globalTextAttr->m_isBoldChecked);
            m_italicButton->setChecked(globalTextAttr->m_isItalicChecked);
            m_underlineButton->setChecked(globalTextAttr->m_isUnderlineChecked);
            //m_colorEdit->setColor(globalTextAttr->m_fontColor.rgb());
#endif
            m_leftTextButton->setChecked(globalTextAttr->m_textAlign == Qt::AlignLeft);
            m_centerTextButton->setChecked(globalTextAttr->m_textAlign == Qt::AlignHCenter);
            m_rightTextButton->setChecked(globalTextAttr->m_textAlign == Qt::AlignRight);

            m_startTimeTextCombo->setCurrentText(QString(tr("%1")).
                    arg(QString::number((float)globalTextAttr->m_iStartTimeText/1000, 'f', 2)));
            m_durationTextCombo->setCurrentText(QString(tr("%1")).
                    arg(QString::number((float)globalTextAttr->m_iDurationText/1000, 'f', 2)));

            //m_startTimeTextCombo->setCurrentText(globalTextAttr->m_qsStartTimeText);
            //m_durationTextCombo->setCurrentText(globalTextAttr->m_qsDurationText);
        }
        else
        {
            qDebug()<< "error uncomplete. TabWidget::assignTextInfo: globalTextAttr";
        }
    }
    else
    {
        qDebug()<< "error uncomplete. TabWidget::assignTextInfo: m_globalContext";
    }
}
#if 0
void TabWidget::assignTabWidget(const stTextAttr *textItem)
{
    if(!textItem)
    {
        return;
    }
    m_fontCombo->setFont(textItem->m_qfont);
    m_fontSizeCombo->setCurrentText(textItem->m_fontSize);
    m_boldButton->setChecked(textItem->m_isBoldChecked);
    m_italicButton->setChecked(textItem->m_isItalicChecked);
    m_underlineButton->setChecked(textItem->m_isUnderlineChecked);
    //m_colorEdit->setColor(textItem->m_fontColor.rgb());

    m_leftTextButton->setChecked(textItem->m_textAlign == Qt::AlignLeft);
    m_centerTextButton->setChecked(textItem->m_textAlign == Qt::AlignHCenter);
    m_rightTextButton->setChecked(textItem->m_textAlign == Qt::AlignRight);

    m_startTimeTextCombo->setCurrentText(textItem->m_qsStartTimeText);
    m_durationTextCombo->setCurrentText(textItem->m_qsDurationText);
}
#endif
#if 0
void TabWidget::currentFontChanged()
{
    handleFontChange();
}
void TabWidget::fontSizeChanged(const QString &)
{
    handleFontChange();
}
void TabWidget::currentBoldChanged(bool)
{
    handleFontChange();
}
#endif
void TabWidget::handleFontChange()
{
    Element* element;
    if(m_globalContext && m_globalContext->m_elementsEdit && 
            (element=m_globalContext->m_elementsEdit->currentElement()))
    {
        GlobalTextAttr* globalTextAttr = element->globalTextAttr();
        if(globalTextAttr)
        {
            QFont font = m_fontCombo->currentFont();
            font.setPointSize(m_fontSizeCombo->currentText().toInt());
            //font.setPixelSize(m_fontSizeCombo->currentText().toInt());
            font.setWeight(m_boldButton->isChecked() ? QFont::Bold : QFont::Normal);
            font.setItalic(m_italicButton->isChecked());
            font.setUnderline(m_underlineButton->isChecked());
            globalTextAttr->m_textAlign = m_paragraphTextButtonG->checkedId();
            globalTextAttr->m_qfont = font;
            globalTextAttr->m_fontColor = qvariant_cast<QColor>(textAction->data());

            m_globalContext->m_scene->setVideoTextAttr(element, globalTextAttr);

        }
        else
        {
            qDebug()<< "error uncomplete. TabWidget::handleFontChange: globalTextAttr";
        }
    }
    else
    {
        qDebug()<< "error uncomplete. TabWidget::handleFontChange: m_globalContext";

    }
}
#if 0
void TabWidget::handleFontChange()
{
    if(!m_element || !m_mapText.contains(m_element))
        return;

    //QMessageBox::information(this, "Error Opening Picture", QString(tr("x: %1 y: %2")).arg(frameGeometry().x()).arg(frameGeometry().y()));
    stTextAttr *textItem=m_mapText[m_element];

    QFont font = m_fontCombo->currentFont();
    //font.setPointSize(m_fontSizeCombo->currentText().toInt());
    font.setPixelSize(m_fontSizeCombo->currentText().toInt());
    font.setWeight(m_boldButton->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(m_italicButton->isChecked());
    font.setUnderline(m_underlineButton->isChecked());

    textItem->m_textAlign = m_paragraphTextButtonG->checkedId();

    textItem->m_qfont = font;

    textItem->m_fontColor = qvariant_cast<QColor>(textAction->data());
    //scene->setTextColor(qvariant_cast<QColor>(textAction->data()));

    //m_globalContext->m_scene->setFont(m_element, font);
    m_globalContext->m_scene->setTextAttr(m_element, textItem);

}
#endif
void TabWidget::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    fontColorToolButton->setAutoFillBackground(true);
    //textButtonTriggered();
    handleFontChange();
}
//============================================================================//
//                                  ColorEdit                                 //
//============================================================================//

ColorEdit::ColorEdit(QRgb initialColor, int id)
    : m_color(initialColor), m_id(id)
{
    //QHBoxLayout *layout = new QHBoxLayout;
    //setLayout(layout);
    //layout->setContentsMargins(0, 0, 0, 0);
    //m_button = new QFrame(this);
    m_button = new QFrame(this);
    QPalette palette = m_button->palette();
    palette.setColor(QPalette::Window, QColor(m_color));
    m_button->setPalette(palette);
    m_button->setAutoFillBackground(true);
    m_button->setMinimumSize(32, 0);
    m_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    m_button->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    //layout->addWidget(m_button);

    //connect(m_lineEdit, SIGNAL(editingFinished()), this, SLOT(editDone()));
    setMinimumSize(50, 50);
    setMaximumSize(50, 50);
    //setAlignment(Qt::AlignVCenter);
}
void ColorEdit::editDone()
{
}
void ColorEdit::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QColor color(m_color);
        QColorDialog dialog(color, 0);
        dialog.setOption(QColorDialog::ShowAlphaChannel, true);
// The ifdef block is a workaround for the beta, TODO: remove when bug 238525 is fixed
#ifdef Q_DEAD_CODE_FROM_QT4_MAC
        dialog.setOption(QColorDialog::DontUseNativeDialog, true);
#endif
        dialog.move(280, 120);
        if (dialog.exec() == QDialog::Rejected)
            return;
        QRgb newColor = dialog.selectedColor().rgba();
        if (newColor == m_color)
            return;
        setColor(newColor);
    }
}

void ColorEdit::setColor(QRgb color)
{
    m_color = color;
    QPalette palette = m_button->palette();
    palette.setColor(QPalette::Window, QColor(m_color));
    m_button->setPalette(palette);
    emit colorChanged(m_color, m_id);
}
