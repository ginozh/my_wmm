#include <QDebug>
#include "glhiddenwidget.h"
GLHiddenWidget::GLHiddenWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMinimumSize(512, 384);
}

GLHiddenWidget::~GLHiddenWidget()
{
}

void GLHiddenWidget::test(QSurface* surface)
{
    QOpenGLContext * globalcon=QOpenGLContext::globalShareContext();
    globalcon->makeCurrent(m_surface);
    //globalcon->makeCurrent(surface);
    qDebug()<<"GLHiddenWidget::test globalcon: "<<globalcon;
    //makeCurrent();
    GLuint textureId0;
    glGenTextures(1, &textureId0);
    qDebug()<<"GLHiddenWidget::test textureId0: "<<textureId0;
    glGenTextures(1, &textureId0);
    qDebug()<<"GLHiddenWidget::test textureId0: "<<textureId0;
    //doneCurrent();
}
void GLHiddenWidget::initializeGL()
{
    QOffscreenSurface *surface = new QOffscreenSurface;
    surface->setFormat(context()->format());
    surface->create();
    m_surface = surface;

    initializeOpenGLFunctions();
    GLuint textureId0;
    glGenTextures(1, &textureId0);
    glBindTexture(GL_TEXTURE_2D, textureId0);
    qDebug()<<"GLHiddenWidget::initializeGL textureId0: "<<textureId0;
    glGenTextures(1, &textureId0);
    glBindTexture(GL_TEXTURE_2D, textureId0);
    qDebug()<<"GLHiddenWidget::initializeGL textureId0: "<<textureId0;
}
