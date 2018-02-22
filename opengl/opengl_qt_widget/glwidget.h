#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "logo.h"

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
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void cleanup();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

private:
    GLuint build_shader(const GLchar *shader_source, GLenum type) ;
    int build_program(GenericShaderContext *gs, const QString& fragSource) ;
};

#endif
