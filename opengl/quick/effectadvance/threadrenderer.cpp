#include "threadrenderer.h"

#include <QDebug>

QList<QThread *> ThreadRenderer::threads;
/*
 * The render thread shares a context with the scene graph and will
 * render into two separate FBOs, one to use for display and one
 * to use for rendering
 */

ThreadRenderer::ThreadRenderer()
    : m_renderThread(0)
{
    qDebug()<<"ThreadRenderer::ThreadRenderer";
    setFlag(ItemHasContents, true);
    m_renderThread = new RenderThread(QSize(512, 512));
}

void ThreadRenderer::ready()
{
    qDebug()<<"ThreadRenderer::ready";
    m_renderThread->surface = new QOffscreenSurface(); //new by gui-thread 
    m_renderThread->surface->setFormat(m_renderThread->context->format());
    m_renderThread->surface->create();

    m_renderThread->moveToThread(m_renderThread);

    connect(window(), &QQuickWindow::sceneGraphInvalidated, m_renderThread, &RenderThread::shutDown, Qt::QueuedConnection);

    m_renderThread->start();
    update();
    qDebug()<<"ThreadRenderer::ready update";
}

QSGNode *ThreadRenderer::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    qDebug()<<"ThreadRenderer::updatePaintNode";
    TextureNode *node = static_cast<TextureNode *>(oldNode);

    if (!m_renderThread->context) {
        QOpenGLContext *current = window()->openglContext();
        // Some GL implementations requres that the currently bound context is
        // made non-current before we set up sharing, so we doneCurrent here
        // and makeCurrent down below while setting up our own context.
        current->doneCurrent();

        m_renderThread->context = new QOpenGLContext();
        m_renderThread->context->setFormat(current->format());
        m_renderThread->context->setShareContext(current);
        m_renderThread->context->create();
        m_renderThread->context->moveToThread(m_renderThread);
        qDebug()<<"ThreadRenderer::updatePaintNode new openglcontext";

        current->makeCurrent(window());

        QMetaObject::invokeMethod(this, "ready"); //=> guithread
        //QMetaObject::invokeMethod(this, "ready", Qt::AutoConnection);
        //QMetaObject::invokeMethod(this, "ready", Qt::QueuedConnection);
        //ready();
        return 0;
    }

    if (!node) {
        node = new TextureNode(window());
        qDebug()<<"ThreadRenderer::updatePaintNode new TextureNode";

        /* Set up connections to get the production of FBO textures in sync with vsync on the
         * rendering thread.
         *
         * When a new texture is ready on the rendering thread, we use a direct connection to
         * the texture node to let it know a new texture can be used. The node will then
         * emit pendingNewTexture which we bind to QQuickWindow::update to schedule a redraw.
         *
         * When the scene graph starts rendering the next frame, the prepareNode() function
         * is used to update the node with the new texture. Once it completes, it emits
         * textureInUse() which we connect to the FBO rendering thread's renderNext() to have
         * it start producing content into its current "back buffer".
         *
         * This FBO rendering pipeline is throttled by vsync on the scene graph rendering thread.
         */
        connect(m_renderThread, &RenderThread::textureReady, node, &TextureNode::newTexture, Qt::DirectConnection);
        connect(node, &TextureNode::pendingNewTexture, window(), &QQuickWindow::update, Qt::QueuedConnection);
        connect(window(), &QQuickWindow::beforeRendering, node, &TextureNode::prepareNode, Qt::DirectConnection);
        connect(node, &TextureNode::textureInUse, m_renderThread, &RenderThread::renderNext, Qt::QueuedConnection);

        // Get the production of FBO textures started..
        QMetaObject::invokeMethod(m_renderThread, "renderNext", Qt::QueuedConnection);
    }

    node->setRect(boundingRect());

    return node;
}
RenderThread::RenderThread(const QSize &size)
    : surface(0)
    , context(0)
    , m_renderFbo(0)
      , m_displayFbo(0)
#ifdef USE_LOGO
      , m_logoRenderer(0)
#endif
, m_size(size)
{
    qDebug()<<"RenderThread::RenderThread";
    ThreadRenderer::threads << this;
}
void RenderThread::renderNext()
{
    ////QThread::msleep(5000);
    qDebug()<<"RenderThread::renderNext";
    context->makeCurrent(surface);

    if (!m_renderFbo) {
        // Initialize the buffers and renderer
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        m_renderFbo = new QOpenGLFramebufferObject(m_size, format);
        m_displayFbo = new QOpenGLFramebufferObject(m_size, format);
#ifdef USE_LOGO
        m_logoRenderer = new LogoRenderer();
        m_logoRenderer->initialize();
#endif
    }

    qDebug()<<"RenderThread::renderNext m_renderFbo->texture(): "<<m_renderFbo->texture();
    m_renderFbo->bind();
    context->functions()->glViewport(0, 0, m_size.width(), m_size.height());

#ifdef USE_LOGO
    m_logoRenderer->render();
#endif

    // We need to flush the contents to the FBO before posting
    // the texture to the other thread, otherwise, we might
    // get unexpected results.
    context->functions()->glFlush();

    m_renderFbo->bindDefault();
    qSwap(m_renderFbo, m_displayFbo);

    emit textureReady(m_displayFbo->texture(), m_size);
}

void RenderThread::shutDown()
{
    qDebug()<<"RenderThread::shutDown";
    context->makeCurrent(surface);
    delete m_renderFbo;
    delete m_displayFbo;
#ifdef USE_LOGO
    delete m_logoRenderer;
#endif
    context->doneCurrent();
    delete context;

    // schedule this to be deleted only after we're done cleaning up
    surface->deleteLater();

    // Stop event processing, move the thread to GUI and make sure it is deleted.
    exit();
    moveToThread(QGuiApplication::instance()->thread());
}

TextureNode::TextureNode(QQuickWindow *window)
    : m_id(0)
    , m_size(0, 0)
    , m_texture(0)
      , m_window(window)
{
    qDebug()<<"TextureNode::TextureNode";
    connect(window, &QQuickWindow::beforeRendering, this, &TextureNode::maybeRotate);
    connect(window, &QQuickWindow::frameSwapped, this, &TextureNode::maybeUpdate);
    // Our texture node must have a texture, so use the default 0 texture.
    m_texture = m_window->createTextureFromId(0, QSize(1, 1));
    setTexture(m_texture);
    setFiltering(QSGTexture::Linear);
}

TextureNode::~TextureNode()
{
    delete m_texture;
}
void TextureNode::newTexture(int id, const QSize &size) 
{
    qDebug()<<"TextureNode::newTexture id: "<<id;
    m_mutex.lock();
    m_id = id;
    m_size = size;
    m_mutex.unlock();

    // We cannot call QQuickWindow::update directly here, as this is only allowed
    // from the rendering thread or GUI thread.
    emit pendingNewTexture();
}


// Before the scene graph starts to render, we update to the pending texture
void TextureNode::prepareNode() 
{
    qDebug()<<"TextureNode::prepareNode m_id: "<<m_id;
    m_mutex.lock();
    int newId = m_id;
    QSize size = m_size;
    m_id = 0;
    m_mutex.unlock();
    if (newId) {
        delete m_texture;
        // note: include QQuickWindow::TextureHasAlphaChannel if the rendered content
        // has alpha.
        m_texture = m_window->createTextureFromId(newId, size);
        setTexture(m_texture);

        markDirty(DirtyMaterial);

        // This will notify the rendering thread that the texture is now being rendered
        // and it can start rendering to the other one.
        emit textureInUse();
    }
}

void TextureNode::maybeRotate() 
{
    qDebug()<<"TextureNode::maybeRotate beforeRendering";
}

void TextureNode::maybeUpdate() 
{
    qDebug()<<"TextureNode::maybeUpdate frameSwapped";
}

