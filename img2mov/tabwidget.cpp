#include "tabwidget.h"
#include <QLayout>
#include <QScrollArea>
#include "flowlayout.h"
#include "animation.h"
#include <QFontComboBox>
#include <QColorDialog>
#include <QTableWidget>
#include <QMessageBox>
#include "videoscene.h"


//tmp
#include <QTextEdit>
#include <QPushButton>
#include <QToolButton>
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


TabWidget::TabWidget(QWidget *parent, QWidget *elementsEdit, DiagramScene* scene)
    : QTabWidget(parent)
    , m_iconSize(100, 100)
    , m_elementsEdit(elementsEdit)
    , m_scene(scene)
    , m_element(0)
{
    QWidget *tabNull = new QWidget;
    addTab(tabNull, tr("   "));
    createTabHome();
    createTabAnimations();
    createTabText();

    setTabEnabled(0, false);
    //setCurrentWidget(contentsWidget);
    setCurrentWidget(m_tabHome);
    setMaximumHeight(m_iconSize.height() + 120);
}
void TabWidget::createTabHome()
{
    m_tabHome = new QWidget;
    QHBoxLayout *hbox = new QHBoxLayout;
    m_tabHome->setLayout(hbox);
    {
        QVBoxLayout *vboxAdd = new QVBoxLayout;
        hbox->addLayout(vboxAdd);
        {
            QHBoxLayout* hboxTop = new QHBoxLayout;
            vboxAdd->addLayout(hboxTop);
            {
                QToolButton *addPhotos = new QToolButton();
                hboxTop->addWidget(addPhotos, Qt::AlignLeft);

                addPhotos->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                addPhotos->setIcon(QIcon("images/addphotos.png"));
                addPhotos->setIconSize(m_iconSize);
                addPhotos->setText("Add photos");
                //addPhotos->setMaximumHeight(m_iconSize.height() + 250);
                addPhotos->setMinimumWidth(m_iconSize.width() + 100);
                //addPhotos->setMinimumWidth(addPhotos->text().length());
                connect(addPhotos, SIGNAL(clicked()), m_elementsEdit, SLOT(load()));
            }
            {
                QToolButton *addMusic = new QToolButton();
                //hboxTop->addWidget(addMusic, Qt::AlignLeft);
                addMusic->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                addMusic->setIcon(QIcon("images/addmusic.png"));
                addMusic->setIconSize(m_iconSize);
                addMusic->setText("Add music");
                addMusic->setMinimumWidth(m_iconSize.width() + 100);
                //connect(addMusic, SIGNAL(clicked()), m_elementsEdit, SLOT(load()));
            }
            {
                QVBoxLayout *vboxTopRightAdd = new QVBoxLayout;
                hboxTop->addLayout(vboxTopRightAdd);
                {
                    QToolButton *addCaption = new QToolButton();
                    //vboxTopRightAdd->addWidget(addCaption, Qt::AlignLeft);

                    addCaption->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                    //addCaption->setIcon(QIcon("images/addCaption.png"));
                    //addCaption->setIconSize(m_iconSize);
                    addCaption->setText("Caption");
                    //addCaption->setMaximumHeight(m_iconSize.height() + 250);
                    //addCaption->setMinimumWidth(m_iconSize.width() + 100);
                    //addCaption->setMinimumWidth(addCaption->text().length());
                    //connect(addCaption, SIGNAL(clicked()), m_elementsEdit, SLOT(load()));

                }
            }
        }
        {
            QLabel *lbl = new QLabel(tr("Add"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxAdd->addWidget(lbl);
        }
    }
    {
        appendLine(hbox);
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
                        FlowLayout* flowLayout = new FlowLayout();
                        //flowLayout->setContentsMargins(10, 10, 10, 10);
                        flowLayout->setContentsMargins(5, 5, 5, 5);
                        {
#define INITIAL_ANIMATION(animation_name) do {\
    Animation *animation_name=new Animation("images/"#animation_name".png", ""#animation_name"", m_iconSize) ; \
    connect(animation_name, SIGNAL(selectedAnimationSignal(const QString&)), m_elementsEdit, SLOT(selectedTransition(const QString&))); \
    flowLayout->insertWidget(flowLayout->count(), animation_name); \
} while(0)
#if 0
                            Animation *animation=new Animation("images/bowtieh.png", "bowtieh") ;
                            connect(animation, SIGNAL(selectedAnimationSignal(const QString&)), m_elementsEdit, SLOT(selectedTransition(const QString&)));

                            flowLayout->insertWidget(0, animation);
#endif
                            INITIAL_ANIMATION( bowtieh);
                            INITIAL_ANIMATION( bowtiev);
                            INITIAL_ANIMATION( diagboxout);
                            INITIAL_ANIMATION( diagcrossout);
                            INITIAL_ANIMATION( diagdownright);
                            INITIAL_ANIMATION( filledvdown);
                            INITIAL_ANIMATION( filledvleft);
                            INITIAL_ANIMATION( filledvright);
                            INITIAL_ANIMATION( filledvup);
                            INITIAL_ANIMATION( barsh);
                            INITIAL_ANIMATION( barsvertical);
                            INITIAL_ANIMATION( crossfade);
                            INITIAL_ANIMATION( checkerb);
                            INITIAL_ANIMATION( circle);
                            INITIAL_ANIMATION( circles);
                            INITIAL_ANIMATION( diamond);
                            INITIAL_ANIMATION( heart);
                            INITIAL_ANIMATION( rectangle);
                            INITIAL_ANIMATION( wheel);
                            INITIAL_ANIMATION( insetbottoml);
                            INITIAL_ANIMATION( insetbottomr);
                            INITIAL_ANIMATION( insettopleft);
                            INITIAL_ANIMATION( insettopr);
                            INITIAL_ANIMATION( iris);
                            INITIAL_ANIMATION( revealdown);
                            INITIAL_ANIMATION( revealright);
                            INITIAL_ANIMATION( revealleft);
                            INITIAL_ANIMATION( roll);
                            INITIAL_ANIMATION( slide);
                            INITIAL_ANIMATION( slideupt);
                            //INITIAL_ANIMATION( slidedownt);
                            INITIAL_ANIMATION( splith);
                            INITIAL_ANIMATION( splitv);
                            INITIAL_ANIMATION( fanin);
                        }

                        animations->setLayout(flowLayout);
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
                    //connect(btnApplyTrans, SIGNAL(clicked()), m_elementsEdit, SLOT(openFile()));
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

    m_tabAnimations->setLayout(hboxAnimations);

    addTab(m_tabAnimations, tr("&Animations"));
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
    QHBoxLayout *hboxText = new QHBoxLayout;
    {
        QVBoxLayout *vboxFont = new QVBoxLayout;
        hboxText->addLayout(vboxFont);
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
            hboxMidFont->setMargin(5);
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
                m_colorEdit = new ColorEdit(Qt::white, 1);
                hboxMidFont->addWidget(m_colorEdit);
            }

        }
        {
            QLabel *lbl = new QLabel(tr("Font"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxFont->addWidget(lbl);
        }

    }
    {
        appendLine(hboxText);
    }
    {
        // /c/QtProjects/QtExamples/widgets/richtext/textedit/textedit.cpp
        QVBoxLayout *vboxParagraph = new QVBoxLayout;
        hboxText->addLayout(vboxParagraph);
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
                m_leftTextButton->setIcon(leftIcon);

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

        }
        {
            QLabel *lbl = new QLabel(tr("Paragraph"));
            lbl->setAlignment(Qt::AlignCenter);

            vboxParagraph->addWidget(lbl);
        }

    }
    {
        appendLine(hboxText);
    }
    {
        QVBoxLayout *vboxAdjust = new QVBoxLayout;
        hboxText->addLayout(vboxAdjust);
        {
            QHBoxLayout* hboxAdjust = new QHBoxLayout;
            vboxAdjust->addLayout(hboxAdjust);
            {
                QToolButton *editextBtn = new QToolButton(this);
                editextBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                editextBtn->setIcon(QIcon("images/edittext.png"));
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
                        QLabel *lbl = new QLabel(tr("Start time:"));

                        hboxTopAdjust->addWidget(lbl);
                    }
                    {
                        m_startTimeTextCombo = new QComboBox();
                        m_startTimeTextCombo->setEditable(true);
                        m_startTimeTextCombo->addItem(QString(tr("2.00s")));

                        hboxTopAdjust->addWidget(m_startTimeTextCombo);
                    }

                    vboxTopAdjust->addLayout(hboxTopAdjust);
                }
                {
                    QHBoxLayout* hboxMidAdjust = new QHBoxLayout;
                    {
                        QLabel *lbl = new QLabel(tr("Text duration:"));

                        hboxMidAdjust->addWidget(lbl);
                    }
                    {
                        m_durationTextCombo = new QComboBox();
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
        appendLine(hboxText);
    }
    {
        QVBoxLayout *vboxEffects = new QVBoxLayout;
        hboxText->addLayout(vboxEffects);
        {
            QScrollArea *scrollArea = new QScrollArea;
            initialScrollArea(scrollArea);
            {
                QWidget* effects = new QWidget();
                {
                    FlowLayout* flowLayout = new FlowLayout();
                    flowLayout->setContentsMargins(5, 5, 5, 5);
                    {
#define INITIAL_TEXT(animation_name) do {\
    Animation *animation_name=new Animation("images/"#animation_name".png", ""#animation_name"", m_iconSize) ; \
    connect(animation_name, SIGNAL(selectedAnimationSignal(const QString&)), m_elementsEdit, SLOT(selectedTransition(const QString&))); \
    flowLayout->insertWidget(0, animation_name); \
} while(0)
                        //INITIAL_TEXT(bowtiev);
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
        appendLine(hboxText);
    }

    m_tabText->setLayout(hboxText);

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
QIcon TabWidget::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
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
    m_colorEdit->setColor(textItem->m_fontColor.rgb());

    m_leftTextButton->setChecked(textItem->m_textAlign == Qt::AlignLeft);
    m_centerTextButton->setChecked(textItem->m_textAlign == Qt::AlignHCenter);
    m_rightTextButton->setChecked(textItem->m_textAlign == Qt::AlignRight);

    m_startTimeTextCombo->setCurrentText(textItem->m_qsStartTimeText);
    m_durationTextCombo->setCurrentText(textItem->m_qsDurationText);
}
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
    if(!m_element || !m_mapText.contains(m_element))
        return;

    //QMessageBox::information(this, "Error Opening Picture", QString(tr("x: %1 y: %2")).arg(frameGeometry().x()).arg(frameGeometry().y()));
    stTextAttr *textItem=m_mapText[m_element];

    QFont font = m_fontCombo->currentFont();
    font.setPointSize(m_fontSizeCombo->currentText().toInt());
    font.setWeight(m_boldButton->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(m_italicButton->isChecked());
    font.setUnderline(m_underlineButton->isChecked());

    textItem->m_qfont = font;

    //m_scene->setFont(m_element, font);
    m_scene->setTextAttr(m_element, textItem);

}
