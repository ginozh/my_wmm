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

#include <QFileInfo>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, m_treeView(new QTreeView)
    //, m_detailsText(new QTextEdit)
    , m_player(new VideoPlayer)
    , m_elementsEdit(new ElementsEdit)
{
    setWindowTitle(tr("img to movie"));

    //widgets/widgets/tablet
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    fileMenu->addAction(tr("&Open..."), m_elementsEdit, &ElementsEdit::load, QKeySequence::Open);

    QAction *exitAction = fileMenu->addAction(tr("E&xit"), qApp, &QApplication::closeAllWindows);
    exitAction->setShortcuts(QKeySequence::Quit);


    QMenu *findMenu = menuBar()->addMenu(tr("&Edit"));

    menuBar()->addMenu(tr("&About"))->addAction(tr("&About Qt"), qApp, &QApplication::aboutQt);

    QSplitter *centralSplitter = new QSplitter;
    setCentralWidget(centralSplitter);
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
