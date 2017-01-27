#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include "musics.h"
#include "comm.h"

Musics::Musics(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    {
        // QLabel* 
        // layout->addWidget(graphicsView);
    }
    {
        // layout->addWidget(graphicsView);
    }
    setLayout(layout);
}

Musics::~Musics()
{
}
