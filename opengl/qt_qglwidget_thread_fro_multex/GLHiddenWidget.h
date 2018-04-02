#ifndef _GLHiddenWidget_h
#define _GLHiddenWidget_h
#include <QGLWidget>

class GLHiddenWidget : public QGLWidget
{
	Q_OBJECT
public:
	GLHiddenWidget(QGLFormat format, QWidget *parent=0);
	~GLHiddenWidget();
protected:
	//virtual void glInit();
	//virtual void glDraw();

	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *);

private:
};

#endif // _GLHiddenWidget_h
