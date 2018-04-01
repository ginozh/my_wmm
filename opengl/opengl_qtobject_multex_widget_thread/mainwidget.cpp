#include <QDebug>
#include "MMComm.h"
#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
}

MainWidget::~MainWidget()
{
    makeCurrent();
    doneCurrent();
}

//! [1]
void MainWidget::initializeGL()
{
    qDebug()<<"MainWidget::initializeGL";
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    MMGlobalContext::instance()->initShaders();
    MMGlobalContext::instance()->initTextures();
}


//! [5]
void MainWidget::resizeGL(int w, int h)
{
    qDebug()<<"MainWidget::resizeGL";
    QOpenGLWidget::resizeGL(w, h);
    MMGlobalContext::instance()->initialOpengl(w, h);
}
//! [5]

void MainWidget::paintGL()
{
    qDebug()<<"MainWidget::paintGL";
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MMGlobalContext::instance()->fragRenderForOtherThreadAgain();
}
