#include <QDebug>
#include "GLHiddenWidget.h"

GLHiddenWidget::GLHiddenWidget(QGLFormat format):
QGLWidget(format)
{
	setAutoBufferSwap(false);
	doneCurrent();
}

GLHiddenWidget::~GLHiddenWidget()
{
    qDebug()<<"GLHiddenWidget::~GLHiddenWidget";
}

void GLHiddenWidget::glInit()
{
    qDebug()<<"GLHiddenWidget::glInit";
}

void GLHiddenWidget::glDraw()
{
    qDebug()<<"GLHiddenWidget::glDraw";
}

void GLHiddenWidget::initializeGL()
{
    qDebug()<<"GLHiddenWidget::initializeGL";
}

void GLHiddenWidget::resizeGL(int width, int height)
{
    qDebug()<<"GLHiddenWidget::resizeGL width: "<<width<<" height: "<<height;
}

void GLHiddenWidget::paintGL()
{
    qDebug()<<"GLHiddenWidget::paintGL";
}

void GLHiddenWidget::paintEvent(QPaintEvent *)
{
    qDebug()<<"GLHiddenWidget::paintEvent";
}

void GLHiddenWidget::resizeEvent(QResizeEvent *event)
{
    qDebug()<<"GLHiddenWidget::resizeEvent";
}
