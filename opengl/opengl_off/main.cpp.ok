#include <QApplication>
#include "logorenderer.h"
#include <QDebug>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QOpenGLFramebufferObject>
#include <QImage>

void opengl()
{
    QOpenGLFramebufferObject *m_renderFbo=0;
    //QOpenGLFramebufferObject *m_displayFbo=0;

    LogoRenderer *m_logoRenderer;
    QSize m_size(512,384);
    QOffscreenSurface *surface;
    QOpenGLContext *context;

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setSamples(4);
    //fmt.setVersion(3, 2);
    fmt.setProfile(QSurfaceFormat::CoreProfile);

    context = new QOpenGLContext();
    context->setFormat(fmt);
    //context->setShareContext(current);
    context->create();

    surface = new QOffscreenSurface();
    surface->setFormat(context->format());
    surface->create();

    context->makeCurrent(surface);
    if (!m_renderFbo) {
        // Initialize the buffers and renderer
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        m_renderFbo = new QOpenGLFramebufferObject(m_size, format);
        //m_displayFbo = new QOpenGLFramebufferObject(m_size, format);
        m_logoRenderer = new LogoRenderer();
        m_logoRenderer->initialize();
    }
    m_renderFbo->bind();
    context->functions()->glViewport(0, 0, m_size.width(), m_size.height());

    m_logoRenderer->render();

    // We need to flush the contents to the FBO before posting
    // the texture to the other thread, otherwise, we might
    // get unexpected results.
    context->functions()->glFlush();

    m_renderFbo->bindDefault();
    QImage fboImage(m_renderFbo->toImage());
    //fboImage = fboImage.convertToFormat(QImage::Format_RGB888);
    fboImage.save(QString::fromLocal8Bit("c:\\shareproject\\jpg\\512img005_frag.jpg"));
}
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    opengl();
    return 0;//a.exec();
}
