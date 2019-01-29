render_thread:
    QQuickItem::update()
    QQuickItem::updatePolish()
    Start a new frame: OpenGL Context is made current
    emit QQuickWindow::beforeSynchronization()
    QQuickItem::updatePaintNode()
        First: 
            m_renderThread->context = new QOpenGLContext();
            m_renderThread->context->setShareContext(current);
            m_renderThread->context->moveToThread(m_renderThread);
            QMetaObject::invokeMethod(this, "ready");//Qt::QueuedConnection //=>gui_thread, 因为ThreadRenderer所属gui
gui_thread:
    ThreadRenderer::ready()
        m_renderThread->surface = new QOffscreenSurface();
        m_renderThread->moveToThread(m_renderThread);
        m_renderThread->start();
        this->update(); //=> QQuickItem::update
render_thread:
    QQuickItem::updatePaintNode()
        second:
            TextureNode *node = static_cast<TextureNode *>(oldNode);
            node = new TextureNode(window());
                m_texture = m_window->createTextureFromId(0, QSize(1, 1));
            connect(m_renderThread, &RenderThread::textureReady, node, &TextureNode::newTexture, Qt::DirectConnection);
            connect(node, &TextureNode::pendingNewTexture, window(), &QQuickWindow::update, Qt::QueuedConnection);
            connect(window(), &QQuickWindow::beforeRendering, node, &TextureNode::prepareNode, Qt::DirectConnection);
            connect(node, &TextureNode::textureInUse, m_renderThread, &RenderThread::renderNext, Qt::QueuedConnection);

            QMetaObject::invokeMethod(m_renderThread, "renderNext", Qt::QueuedConnection);//=>mythread, 因为m_renderThread所属mythread

Loop:
render_thread:
    emit QQuickWindow::beforeRendering()  //Qt::DirectConnection
    =>TextureNode::prepareNode()
        m_mutex.lock();
        int newId = m_id;
        QSize size = m_size;
        m_id = 0;
        m_mutex.unlock();
        if (newId) {
            delete m_texture;
            m_texture = m_window->createTextureFromId(newId, size);
            setTexture(m_texture);
            emit textureInUse(); =>mythread RenderThread::renderNext
        }
mythread:
    RenderThread::renderNext //<= by render_thread
        context->makeCurrent(surface);
        if (!m_renderFbo) {new QOpenGLFramebufferObject;}
        m_renderFbo->bind();
        m_logoRenderer->render();
        context->functions()->glFlush();
        qSwap(m_renderFbo, m_displayFbo);
        emit textureReady(m_displayFbo->texture(), m_size); 
        => TextureNode::newTexture
            m_mutex.lock();
            m_id = id;
            m_size = size;
            m_mutex.unlock();
            emit pendingNewTexture(); //=> render_thread QQuickWindow::update (no QQuickItem::update?)
                                      //Calling QQuickWindow::update() differs from QQuickItem::update() in that it always triggers a repaint, regardless of changes in the underlying scene graph or not.
render_thread:
    Scene Graph is rendered
    emit QQuickWindow::afterRendering()
    QOpenGLContext::swapBuffer()
    emit QQuickWindow::frameSwapped()
    Frame is complete and on screen
    Advance Animations Even Processing etc.
