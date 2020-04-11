#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "mainwindow.h"
#include "RecordDialog.hpp"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    //setWindowTitle(tr("Movie Maker"));
    QWidget* centralWidget = new QWidget(this);

    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* lbl=NULL;
    {
        QHBoxLayout* hbox = new QHBoxLayout;
        mainLayout->addLayout(hbox);
        {
            QPushButton* record = new QPushButton("record");
            hbox->addWidget(record);
            connect(record, &QAbstractButton::clicked, [=]() {
                    if(!recordDialog)
                        recordDialog = new RecordDialog(this);
                    recordDialog->setAttribute(Qt::WA_DeleteOnClose, true);
                    recordDialog->show();
                    });
        }
    }

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
