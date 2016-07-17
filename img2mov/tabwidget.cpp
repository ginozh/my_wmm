#include "tabwidget.h"
#include <QLayout>


//tmp
#include <QTableWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>

TabWidget::TabWidget(QWidget *parent, QWidget *elementsEdit)
    : QTabWidget(parent)
    , m_iconSize(65, 65)
    , m_elementsEdit(elementsEdit)
{
    QWidget *tabNull = new QWidget;
    addTab(tabNull, tr("   "));
    //createTabHome();
    createTabHome1();
    createTabAnimations();

    setTabEnabled(0, false);
    //setCurrentWidget(contentsWidget);
    setMaximumHeight(m_iconSize.height() + 100);
}
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
void TabWidget::createTabHome1()
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
    addPhotos->setIcon(QIcon("addphotos.png"));
    addPhotos->setIconSize(m_iconSize);
    addPhotos->setText("Add photos");
    //addPhotos->setMaximumHeight(m_iconSize.height() + 250);
    addPhotos->setMinimumWidth(m_iconSize.width() + 100);
    //addPhotos->setMinimumWidth(addPhotos->text().length());
    connect(addPhotos, SIGNAL(clicked()), m_elementsEdit, SLOT(load()));

    QToolButton *addMusic = new QToolButton(this);
    addMusic->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addMusic->setIcon(QIcon("addmusic.png"));
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

    setCurrentWidget(tabHome);
}
void TabWidget::createTabAnimations()
{
    QWidget *tabAnimations = new QWidget;
    QTextEdit *textEdit = new QTextEdit;
    textEdit->setPlainText(tr("Twinkle, twinkle, little star,\n"
                              "How I wonder what you are.\n"
                              "Up above the world so high,\n"
                              "Like a diamond in the sky.\n"
                              "Twinkle, twinkle, little star,\n"
                              "How I wonder what you are!\n"));

    QHBoxLayout *hboxAnimations = new QHBoxLayout;
    hboxAnimations->setMargin(5);
    hboxAnimations->addWidget(textEdit);
    tabAnimations->setLayout(hboxAnimations);

    addTab(tabAnimations, tr("&Animations"));
}
