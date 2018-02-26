#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

typedef struct {
    //const AVClass *class;
    GLuint        program;
    GLuint        frame_tex;
    //GLFWwindow    *window;
    GLuint        pos_buf;
    int w;
    int h;
} GenericShaderContext;
#include <QThread>
class PlayerPrivate : public QThread
{
    Q_OBJECT
public:
    PlayerPrivate(QObject *parent = 0);
    ~PlayerPrivate();
protected:
    void run();
};
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
private:
    //GLWidget(QWidget *parent = 0);
    static GLWidget *m_pInstance;
public:
    static GLWidget* instance();
    //GLWidget(QWidget *parent = 0);
    GLWidget(QOpenGLWidget *shareWidget=0, QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void initial() ;

public slots:
    void cleanup();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

private:
    GLuint build_shader(const GLchar *shader_source, GLenum type) ;
    int build_program(GenericShaderContext *gs, const QString& fragSource) ;

private:
    bool bInitial=false;
    //QOpenGLContext *m_context=NULL;
};

#endif
