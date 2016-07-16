#include "mainwindow.h"
#include "elementsedit.h"
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QStatusBar>
#include <QTextEdit>
#include <QTreeView>
#include <QTableWidget>

#include <QFileInfo>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, m_treeView(new QTreeView)
    //, m_detailsText(new QTextEdit)
    , m_centralWidget(new QWidget(this))
    , m_player(new VideoPlayer)
    , m_elementsEdit(new ElementsEdit)
{
    setWindowTitle(tr("img to movie"));
#if 0
    //widgets/widgets/tablet
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    //fileMenu->addAction(tr("&Open..."), m_elementsEdit, &ElementsEdit::load, QKeySequence::Open);
    fileMenu->addAction(tr("&Open..."), m_elementsEdit, SLOT(load()), QKeySequence::Open);
    QAction *exitAction = fileMenu->addAction(tr("E&xit"), qApp, &QApplication::closeAllWindows);
    exitAction->setShortcuts(QKeySequence::Quit);

    //QMenu *findMenu = menuBar()->addMenu(tr("&Edit"));
    menuBar()->addMenu(tr("&About"))->addAction(tr("&About Qt"), qApp, &QApplication::aboutQt);
#endif
    m_tabWidget = new QTabWidget(m_centralWidget);
    QWidget *nullTab = new QWidget;
    QWidget *tab1 = new QWidget;
    QTableWidget *tableWidget = new QTableWidget(10, 10);

    QHBoxLayout *tab1hbox = new QHBoxLayout;
    tab1hbox->setMargin(5);
    tab1hbox->addWidget(tableWidget);
    tab1->setLayout(tab1hbox);

    QWidget *tab2 = new QWidget;
    QTextEdit *textEdit = new QTextEdit;
    textEdit->setPlainText(tr("Twinkle, twinkle, little star,\n"
                              "How I wonder what you are.\n"
                              "Up above the world so high,\n"
                              "Like a diamond in the sky.\n"
                              "Twinkle, twinkle, little star,\n"
                              "How I wonder what you are!\n"));

    QHBoxLayout *tab2hbox = new QHBoxLayout;
    tab2hbox->setMargin(5);
    tab2hbox->addWidget(textEdit);
    tab2->setLayout(tab2hbox);

    m_tabWidget->addTab(nullTab, tr("   "));
    m_tabWidget->addTab(tab1, tr("&Home"));
    m_tabWidget->addTab(tab2, tr("&Animations"));
    m_tabWidget->setTabEnabled(0, false);
    m_tabWidget->setCurrentWidget(tab1);

    QSplitter *centralSplitter = new QSplitter(m_centralWidget);
    //setCentralWidget(centralSplitter);
    //m_treeView = new QTreeView;

    //centralSplitter->addWidget(m_treeView);
    centralSplitter->addWidget(m_player);
    /*const QRect availableGeometry = QApplication::desktop()->availableGeometry(m_player);
    m_player->resize(availableGeometry.width() / 6, availableGeometry.height() / 4);
    m_player->show();*/
    //m_detailsText->setReadOnly(true);
#if 1
    m_scrollArea = new QScrollArea;
    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setMinimumWidth(500);
    m_scrollArea->setWidgetResizable (true);
    m_scrollArea->setWidget(m_elementsEdit);
    centralSplitter->addWidget(m_scrollArea);
#else
    centralSplitter->addWidget(m_elementsEdit);
#endif
    connect(m_elementsEdit, SIGNAL(playVideo(const QString&)), m_player, SLOT(playVideo(const QString&)));

    setCentralWidget(m_centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_menu = new Menu(m_centralWidget, m_elementsEdit);
    mainLayout->addWidget(m_tabWidget);
    mainLayout->addWidget(centralSplitter);

    m_centralWidget->setLayout(mainLayout);

}
#if 0
void MainWindow::load()
{
    //qtconcurrent/imagescaling
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"),
            /*QDir::currentPath()*/QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            "*.jpg *.png");
    if (files.count() == 0)
        return;
    for (int i = 0; i < files.count(); ++i) {
    }

    /*if (!m_canvas->loadImage(fileName))
        QMessageBox::information(this, "Error Opening Picture",
                                 "Could not open picture");
                                 */
}
#endif
