#include "mainwindow.h"
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
    , m_treeView(new QTreeView)
    , m_detailsText(new QTextEdit)
{
    setWindowTitle(tr("img to movie"));

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));


    QAction *exitAction = fileMenu->addAction(tr("E&xit"), qApp, &QApplication::closeAllWindows);
    exitAction->setShortcuts(QKeySequence::Quit);

    QMenu *findMenu = menuBar()->addMenu(tr("&Edit"));

    menuBar()->addMenu(tr("&About"))->addAction(tr("&About Qt"), qApp, &QApplication::aboutQt);

    QSplitter *centralSplitter = new QSplitter;
    setCentralWidget(centralSplitter);
    m_treeView = new QTreeView;

    centralSplitter->addWidget(m_treeView);
    m_detailsText->setReadOnly(true);
    centralSplitter->addWidget(m_detailsText);
}
