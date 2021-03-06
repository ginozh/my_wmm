#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

class Clip;
class Effect;
class OpenGLWidget;

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
	void start_render(QOpenGLContext* share, void* s, const QString &save = nullptr, GLvoid *pixels = nullptr, int idivider = 0);
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
	//int tex_width;
	//int tex_height;
	bool queued;
	bool texture_failed;
	bool running;
	QString save_fn;
	GLvoid *pixel_buffer;
signals:
	void start_create_effect_ui(Clip* c);
public:
    GLuint compose_sequencet() ;
    void setGLWidget(OpenGLWidget*);
    OpenGLWidget* m_glwidget=NULL;
    QOpenGLTexture* texture=nullptr;
    QOpenGLTexture* texture2=nullptr;
    QOpenGLTexture* texture3=nullptr;
    QOpenGLFramebufferObject** fbo=nullptr;
    QOpenGLFramebufferObject** fbo2=nullptr;
    QOpenGLFramebufferObject** fbo3=nullptr;
    QOpenGLFramebufferObject* fbotest=nullptr;
    QOpenGLFramebufferObject* fbotest1=nullptr;
    QOpenGLFramebufferObject* fbotest2=nullptr;
    QOpenGLFramebufferObject* fbotest3=nullptr;
    Clip* clip=nullptr;

    QOpenGLShaderProgram* glslProgram=nullptr;
    bool isOpen = false;
    QOpenGLShaderProgram* glslProgram2=nullptr;
    bool isOpen2 = false;
    QOpenGLShaderProgram* glslProgramOverlay=nullptr;
    bool isOpenOverlay = false;
    QOpenGLFramebufferObject* fboOverlay=nullptr;
};

#endif // RENDERTHREAD_H
