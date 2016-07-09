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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, m_treeView(new QTreeView)
    //, m_detailsText(new QTextEdit)
    , m_player(new VideoPlayer)
    , m_elementsEdit(new ElementsEdit)
{
    setWindowTitle(tr("img to movie"));

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));


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
    m_scrollArea = new QScrollArea;
    m_scrollArea->setWidget(m_elementsEdit);
    //centralSplitter->addWidget(m_elementsEdit);
    centralSplitter->addWidget(m_scrollArea);
}
