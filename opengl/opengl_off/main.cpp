#include <QApplication>
#include "fragrenderer.h"
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

    if(!context->makeCurrent(surface))
    {
        qDebug()<<"error";
        return;
    }

    QString fileName;
    QImage image;
    fileName="c:\\shareproject\\jpg\\512img004.jpg";
    image.load(fileName);
    if (image.isNull()) {
        qDebug()<<"error";
        return;
    }
    image = image.convertToFormat(QImage::Format_RGB888);
    int width=image.width();
    int height=image.height();
    QImage imagefrag(width,height,QImage::Format_RGB888);
    unsigned char *pixels = (unsigned char *) imagefrag.bits();
    QSize m_size(width,height);

    // Initialize the buffers and renderer
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    context->functions()->glViewport(0, 0, m_size.width(), m_size.height());


    FragRenderer* fragRenderer = new FragRenderer();
    {
        m_renderFbo = new QOpenGLFramebufferObject(m_size, format);
        m_renderFbo->bind();

        //frag
        fragRenderer->initialize("Aibao", width, height);

        fragRenderer->render(image.bits());
        context->functions()->glFlush();
        fragRenderer->readPixels(&pixels);
        //end frag
        //QFile file("c:\\shareproject\\jpg\\512img004_frag.jpg");
        //file.remove();
        imagefrag.save("c:\\shareproject\\jpg\\512img004_frag.jpg");
        m_renderFbo->bindDefault();

    }
    {
        m_renderFbo = new QOpenGLFramebufferObject(m_size, format);

        fileName="c:\\shareproject\\jpg\\512img005.jpg";
        image.load(fileName);
        if (image.isNull()) {
            qDebug()<<"error";
            return;
        }
        m_renderFbo->bind();

        //frag
        fragRenderer->initialize("Aibao", width, height);

        fragRenderer->render(image.bits());
        context->functions()->glFlush();
        fragRenderer->readPixels(&pixels);
        //end frag
        imagefrag.save("c:\\shareproject\\jpg\\512img005_frag.jpg");
        m_renderFbo->bindDefault();
    }

    //
    //delete fragRenderer;
    delete m_renderFbo;
    delete context;
    delete surface;


#if 0
    LogoRenderer *m_logoRenderer=0;
    if (!m_logoRenderer) {
        // Initialize the buffers and renderer
        //QOpenGLFramebufferObjectFormat format;
        //format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        //m_renderFbo = new QOpenGLFramebufferObject(m_size, format);
        //m_displayFbo = new QOpenGLFramebufferObject(m_size, format);
        m_logoRenderer = new LogoRenderer();
        m_logoRenderer->initialize();
    }
    //m_renderFbo->bind();
    context->functions()->glViewport(0, 0, m_size.width(), m_size.height());

    m_logoRenderer->render();

    // We need to flush the contents to the FBO before posting
    // the texture to the other thread, otherwise, we might
    // get unexpected results.
    context->functions()->glFlush();
#endif

#if 0
    m_renderFbo->bindDefault();
    QImage fboImage(m_renderFbo->toImage());
    //fboImage = fboImage.convertToFormat(QImage::Format_RGB888);
    fboImage.save(QString::fromLocal8Bit("c:\\shareproject\\jpg\\512img005_frag.jpg"));
#endif
}
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    opengl();
    return 0;//a.exec();
}
