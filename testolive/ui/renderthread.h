#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLTexture>

struct Clip;
class Clipt;
struct Sequence;
class Effect;
class ViewerWidget;
class Viewer;

class RenderThread : public QThread {
	Q_OBJECT
public:
	RenderThread();
	~RenderThread();
	void run();
	QMutex mutex;
	GLuint frameBuffer;
	GLuint texColorBuffer;
	Effect* gizmos;
	void paint();
	void start_render(QOpenGLContext* share, Sequence* s, const QString &save = nullptr, GLvoid *pixels = nullptr, int idivider = 0);
	bool did_texture_fail();
	void cancel();

public slots:
	// cleanup functions
	void delete_ctx();
signals:
	void ready();
private:
	// cleanup functions
	void delete_texture();
	void delete_fbo();

	QWaitCondition waitCond;
	QOffscreenSurface surface;
	QOpenGLContext* share_ctx;
	QOpenGLContext* ctx;
	int divider;
	int tex_width;
	int tex_height;
	bool queued;
	bool texture_failed;
	bool running;
	QString save_fn;
	GLvoid *pixel_buffer;
signals:
	void start_create_effect_ui(Clipt* c);
public:
    GLuint compose_sequencet(void* viewer,
            QOpenGLContext* ctx,
            void* seq,
            QVector<Clip*>& nests,
            bool video,
            bool render_audio,
            Effect** gizmos,
            bool& texture_failed,
            bool rendering) ;
    void setGLWidget(ViewerWidget*);
    ViewerWidget* m_glwidget=NULL;
    QOpenGLTexture* texture=nullptr;
    QOpenGLFramebufferObject** fbo=nullptr;
    Clipt* clip=nullptr;
};

#endif // RENDERTHREAD_H
