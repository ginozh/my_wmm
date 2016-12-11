#include <QtWidgets>
#include <QGraphicsView>
#include <QGraphicsScene>

//#include "characterwidget.h"
#include "mainwindow.h"
#include "eobject.h"

MainWindow::MainWindow()
{

    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,800,480);

    QGraphicsView* graphicsView = new QGraphicsView(scene);
    //graphicsView->setSceneRect(0,0,800,480);
    //graphicsView->setFixedSize(700,400);

    EObject* videoItem = new EObject;
    //videoItem->setSize(QSizeF(500, 480));
    scene->addItem(videoItem);

    QWidget *tabAnimations = new QWidget;

    QHBoxLayout* hboxAnimations = new QHBoxLayout;
    hboxAnimations->addWidget(graphicsView);
    {
        QLabel *lbl = new QLabel(tr("Text duration:"));

        hboxAnimations->addWidget(lbl);
    }
    tabAnimations->setLayout(hboxAnimations);
    setCentralWidget(tabAnimations);
}
