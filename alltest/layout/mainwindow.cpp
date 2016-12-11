#include <QtWidgets>

//#include "characterwidget.h"
#include "mainwindow.h"
#include "flowlayout.h"

//! [0]
/*
-----------------------------------------------------------
|                |     |     |                |           |
|                |-----------|                |           |
|                |           |                |           |
|----------------------------|----------------------------|
|                            |                            |
|----------------------------|----------------------------|
 * */
MainWindow::MainWindow()
{
    QWidget *tabAnimations = new QWidget;
    tabAnimations->setMinimumHeight(200);
    tabAnimations->setMaximumHeight(200);
    QHBoxLayout *hboxAnimations = new QHBoxLayout;
    {
        QVBoxLayout *vboxLeftTransition = new QVBoxLayout;
        {
            QHBoxLayout* hboxTopTransition = new QHBoxLayout;
            {
                QScrollArea *scrollArea = new QScrollArea;
                scrollArea->setBackgroundRole(QPalette::Dark);
                scrollArea->setMinimumWidth(500);
                scrollArea->setMinimumHeight(120);
                scrollArea->setMaximumHeight(120);
                //scrollArea->setContentsMargins(10, 10, 10, 10);
                scrollArea->setContentsMargins(0, 0, 0, 0);
                scrollArea->setWidgetResizable (true);
                {
                    QWidget* animations = new QWidget();
                    {
                        FlowLayout* flowLayout = new FlowLayout();
                        flowLayout->setContentsMargins(10, 10, 10, 10);
                        {
                            for(int i=0; i<10; i++){
                            QLabel* label = new QLabel();
                            label->setMinimumSize(QSize(100, 100));
                            label->setMaximumSize(QSize(100, 100));
                            label->setContentsMargins(5, 5, 5, 5);
                            label->setPixmap(QPixmap(QString::fromUtf8("../../../../QtProjects/qtmovie/jpg/img001.jpg")));
                            label->setScaledContents(true);

                            flowLayout->insertWidget(0, label);
                            }
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

                        hboxTopRightTopTransition->addWidget(comboBox);
                    }

                    vboxTopRightTransition->addLayout(hboxTopRightTopTransition);
                }
                {
                    QLabel *lblApplyTransiton = new QLabel(tr("Apply to all"));
                    lblApplyTransiton->setAlignment(Qt::AlignCenter);

                    vboxTopRightTransition->addWidget(lblApplyTransiton);
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
        QVBoxLayout *vboxRightPanZoom = new QVBoxLayout;
        {
            QHBoxLayout* hboxTopPanZoom = new QHBoxLayout;
            {
                QScrollArea *scrollArea = new QScrollArea;
                {
                    QWidget* panzooms = new QWidget();
                    {
                    }

                    scrollArea->setBackgroundRole(QPalette::Dark);
                    scrollArea->setMinimumWidth(500);
                    scrollArea->setMinimumHeight(120);
                    scrollArea->setMaximumHeight(120);
                    scrollArea->setContentsMargins(10, 10, 10, 10);
                    scrollArea->setWidgetResizable (true);
                    scrollArea->setWidget(panzooms);
                }

                hboxTopPanZoom->addWidget(scrollArea);
            }
            {
                QLabel *lblApplyPanzoom = new QLabel(tr("Apply to all"));
                lblApplyPanzoom->setAlignment(Qt::AlignCenter);

                hboxTopPanZoom->addWidget(lblApplyPanzoom);
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

    tabAnimations->setLayout(hboxAnimations);
    setCentralWidget(tabAnimations);
    setWindowTitle(tr("Character Map"));
}
#if 0
MainWindow::MainWindow()
{
    QWidget *centralWidget = new QWidget;

    QLabel *fontLabel = new QLabel(tr("Font:"));
    fontCombo = new QFontComboBox;
    QLabel *sizeLabel = new QLabel(tr("Size:"));
    sizeCombo = new QComboBox;
    QLabel *styleLabel = new QLabel(tr("Style:"));
    styleCombo = new QComboBox;
    QLabel *fontMergingLabel = new QLabel(tr("Automatic Font Merging:"));
    fontMerging = new QCheckBox;
    fontMerging->setChecked(true);

    scrollArea = new QScrollArea;
    //characterWidget = new CharacterWidget;
    QWidget *tmp = new QWidget;
    //scrollArea->setWidget(characterWidget);
    scrollArea->setWidget(tmp);
//! [0]

//! [1]
    findStyles(fontCombo->currentFont());
//! [1]
    findSizes(fontCombo->currentFont());

//! [2]
    lineEdit = new QLineEdit;
#ifndef QT_NO_CLIPBOARD
    QPushButton *clipboardButton = new QPushButton(tr("&To clipboard"));
//! [2]

//! [3]
    clipboard = QApplication::clipboard();
//! [3]
#endif

//! [4]
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(findStyles(QFont)));
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(findSizes(QFont)));
#if 0
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            characterWidget, SLOT(updateFont(QFont)));
    connect(sizeCombo, SIGNAL(currentIndexChanged(QString)),
            characterWidget, SLOT(updateSize(QString)));
    connect(styleCombo, SIGNAL(currentIndexChanged(QString)),
            characterWidget, SLOT(updateStyle(QString)));
//! [4] //! [5]
    connect(characterWidget, SIGNAL(characterSelected(QString)),
            this, SLOT(insertCharacter(QString)));
#endif
#ifndef QT_NO_CLIPBOARD
    connect(clipboardButton, SIGNAL(clicked()), this, SLOT(updateClipboard()));
#endif
//! [5]
    //connect(fontMerging, SIGNAL(toggled(bool)), characterWidget, SLOT(updateFontMerging(bool)));

//! [6]
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(fontLabel);
    controlsLayout->addWidget(fontCombo, 1);
    controlsLayout->addWidget(sizeLabel);
    controlsLayout->addWidget(sizeCombo, 1);
    controlsLayout->addWidget(styleLabel);
    controlsLayout->addWidget(styleCombo, 1);
    controlsLayout->addWidget(fontMergingLabel);
    controlsLayout->addWidget(fontMerging, 1);
    controlsLayout->addStretch(1);

    QHBoxLayout *lineLayout = new QHBoxLayout;
    lineLayout->addWidget(lineEdit, 1);
    lineLayout->addSpacing(12);
#ifndef QT_NO_CLIPBOARD
    lineLayout->addWidget(clipboardButton);
#endif

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addLayout(controlsLayout);
    centralLayout->addWidget(scrollArea, 1);
    centralLayout->addSpacing(4);
    centralLayout->addLayout(lineLayout);
    centralWidget->setLayout(centralLayout);

    setCentralWidget(centralWidget);
    setWindowTitle(tr("Character Map"));
}
#endif
//! [6]

//! [7]
void MainWindow::findStyles(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentItem = styleCombo->currentText();
    styleCombo->clear();
//! [7]

//! [8]
    QString style;
    foreach (style, fontDatabase.styles(font.family()))
        styleCombo->addItem(style);

    int styleIndex = styleCombo->findText(currentItem);

    if (styleIndex == -1)
        styleCombo->setCurrentIndex(0);
    else
        styleCombo->setCurrentIndex(styleIndex);
}
//! [8]

void MainWindow::findSizes(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentSize = sizeCombo->currentText();
    sizeCombo->blockSignals(true);
    sizeCombo->clear();

    int size;
    if(fontDatabase.isSmoothlyScalable(font.family(), fontDatabase.styleString(font))) {
        foreach(size, QFontDatabase::standardSizes()) {
            sizeCombo->addItem(QVariant(size).toString());
            sizeCombo->setEditable(true);
        }

    } else {
        foreach(size, fontDatabase.smoothSizes(font.family(), fontDatabase.styleString(font))) {
            sizeCombo->addItem(QVariant(size).toString());
            sizeCombo->setEditable(false);
        }
    }

    sizeCombo->blockSignals(false);

    int sizeIndex = sizeCombo->findText(currentSize);

    if(sizeIndex == -1)
        sizeCombo->setCurrentIndex(qMax(0, sizeCombo->count() / 3));
    else
        sizeCombo->setCurrentIndex(sizeIndex);
}

//! [9]
void MainWindow::insertCharacter(const QString &character)
{
    lineEdit->insert(character);
}
//! [9]

//! [10]
#ifndef QT_NO_CLIPBOARD
void MainWindow::updateClipboard()
{
//! [11]
    clipboard->setText(lineEdit->text(), QClipboard::Clipboard);
//! [11]
    clipboard->setText(lineEdit->text(), QClipboard::Selection);
}
#endif
//! [10]
