#ifndef THREADRENDERER_H
#define THREADRENDERER_H

#include <QQuickItem>
#include <QtCore/QMutex>
#include <QtCore/QThread>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QGuiApplication>
#include <QtGui/QOffscreenSurface>

#include <QtQuick/QQuickWindow>
#include <qsgsimpletexturenode.h>
#include <QWaitCondition>
#include "logorenderer.h"
class TextureNode;
class RenderThread : public QThread
{
    Q_OBJECT
public:
    RenderThread(const QSize &size);

    QOffscreenSurface *surface;
    QOpenGLContext *context;

public slots:
    void renderNext();
    void shutDown();
    // Before the scene graph starts to render, we update to the pending texture
    void beforeRenderingSlot();
    void frameSwappedSlot();

private:
    void run();

signals:
    void textureReady(int id, const QSize &size);
    void pendingNewTexture();

public:
    TextureNode *node=NULL;
    QMutex mutexAsynPause;
    QWaitCondition conditionAsynPause;
    bool m_allready=false;

private:
    QOpenGLFramebufferObject *m_renderFbo;
    QOpenGLFramebufferObject *m_displayFbo;

    LogoRenderer *m_logoRenderer=NULL;
    QSize m_size;
    bool abort=false;
};

class TextureNode : public QObject, public QSGSimpleTextureNode
{
    Q_OBJECT

public:
    TextureNode(QQuickWindow *window);
    ~TextureNode();
    // Before the scene graph starts to render, we update to the pending texture
    bool prepareNode();

signals:
    void textureInUse();
    //void pendingNewTexture();

public slots:

    // This function gets called on the FBO rendering thread and will store the
    // texture id and size and schedule an update on the window.
    void newTexture(int id, const QSize &size);

    //void maybeRotate();
    //void maybeUpdate();

private:

    int m_id;
    QSize m_size;

    QMutex m_mutex;

    QSGTexture *m_texture;
    QQuickWindow *m_window;
};

class ThreadRenderer : public QQuickItem
{
    Q_OBJECT

public:
    ThreadRenderer();

    static QList<QThread *> threads;

public Q_SLOTS:
    void ready();

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

private:
    RenderThread *m_renderThread;
};

#endif
