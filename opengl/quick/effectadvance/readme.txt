一、后端线程流程
1, run之前初始化opengl相关资源:
    1.1, render_thread新建QOpenGLContext, moveToThread到myrenderThread
    1.2, gui_thread新建QOffscreenSurface, moveToThread到myrenderThread. 经由render_thread调用invokeMethod
    1.3, render_thread新建TextureNode, 后续所有的gpu图像都在此Node上显示
    1.4, 创建fbo数组，预编译glsl
2, 获取一帧
    2.1 遍历clips, 获取当前帧的nearby_clips
    2.2 遍历nearby_clips, 获取每个clip的帧
    2.3 获得clip对应reader的图像、音频帧
    2.4 获取图像帧:a)上传到gpu,运行glsl完成reader特效,加载到某个fbo;
                   b)运行glsl完成clip特效,加载到某个fbo;
                   c)overlay多个clips,加载到某个fbo;
                   d)fbo->texture设置到TextureNode
3, 文字帧
    c++控制(quick\quickwidgets\qquickviewcomparison):
    3.1 新建、销毁，
        c++调用qml函数: quick/quickwidgets/quickwidget
        /c/qtproject/opengl/qmlbook/docs/ch16-qtcpp/src/modelview
    3.2 显示、不可见
        view visible:model.name!="#ff0000"?true:false
        emit QAbstractListModel::dataChanged(topLeft, topLeft);
        //quickcontrols/controls/tableview
    3.3 动态修改文字属性
        font众多属性: quickcontrols/controls/texteditor
        动态属性: quick/quickwidgets/qquickviewcomparison

        阴影: DropShadow
        Outline: Text { font.pointSize: 24; text: "Outline";style: Text.Outline; styleColor: "red" }

4, c++控制qml
    http://doc.qt.io/qt-5/qtqml-modules-cppplugins.html
二、qtquick+后端thread基本流程
render_thread:
    QQuickItem::update()
    QQuickItem::updatePolish()
    Start a new frame: OpenGL Context is made current
    emit QQuickWindow::beforeSynchronization()
    QQuickItem::updatePaintNode()
        First: 
            m_renderThread->context = new QOpenGLContext();
            m_renderThread->context->setFormat(current->format());
            m_renderThread->context->setShareContext(current);
            m_renderThread->context->create();
            m_renderThread->context->moveToThread(m_renderThread);
            //Qt::QueuedConnection //=>gui_thread, 因为ThreadRenderer所属gui创建
            QMetaObject::invokeMethod(this, "ready");
gui_thread:
    ThreadRenderer::ready()
        m_renderThread->surface = new QOffscreenSurface();
        m_renderThread->surface->setFormat(m_renderThread->context->format());
        m_renderThread->surface->create();
        m_renderThread->moveToThread(m_renderThread);
        m_renderThread->start();
            run()
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
