#ifndef _GLHiddenWidget_h
#define _GLHiddenWidget_h
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
class GLHiddenWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLHiddenWidget(QWidget *parent = 0);
    ~GLHiddenWidget();
    void test(QSurface* surface);

protected:
    //void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    //void paintGL() Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;
private:
    QOffscreenSurface *m_surface;
};
#endif // _GLHiddenWidget_h
