#ifndef _GLHiddenWidget_h
#define _GLHiddenWidget_h
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
class GenericShaderContext;
class GLHiddenWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLHiddenWidget(QWidget *parent = 0);
    ~GLHiddenWidget();
    void test(QOpenGLContext* sharecontext);
    void main2sub(QOpenGLContext* sharecontext);

    void initial() ;
private:
    GLuint build_shader(const GLchar *shader_source, GLenum type) ;
    int build_program(GenericShaderContext *gs, const QString& fragSource) ;
protected:
    //void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    //void paintGL() Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    QOffscreenSurface *m_surface;
    bool bInitial=false;
};
#endif // _GLHiddenWidget_h
