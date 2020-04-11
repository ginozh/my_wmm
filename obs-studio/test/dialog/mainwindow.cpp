#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    //setWindowTitle(tr("Movie Maker"));
    QWidget* centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* lbl=NULL;

    centralWidget->setLayout(mainLayout);
}
void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}
