#ifndef _GLHiddenWidget_h
#define _GLHiddenWidget_h
#include <QGLWidget>
#include <QGLFunctions>
#include "GLWidget.h"
#include "MMComm.h"

//class GLHiddenWidget : public QGLWidget, protected QGLFunctions
class GLHiddenWidget : public GLWidget
{
	Q_OBJECT
public:
	GLHiddenWidget(QGLFormat format, QWidget *parent=0);
	~GLHiddenWidget();
protected:
	//virtual void glInit();
	//virtual void glDraw();

	virtual void initializeGL(); //setContext()之后;onece; paintGL() or resizeGL()之前; 
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

    //void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *);

public:
    void createBindFramebufferTexture();
    void initializeGLProgram();
    int fragRenderForFbo(STFboInfo* finalFboinfo);
    STFboInfo* getNewFboInfo();
    STFboInfo* getFboInfo(int idx);
    void resetFboUsed(int idx);
    GLuint GetTexture(int idx);
    void clearFboBuffer();
private:
    QVector<STFboInfo> vFboInfo; //只能更新一次,因为给外面接口提供的是STFboInfo元素内存指针
    int idxFrameBuf;
    int maxFrameBuf;
};

#endif // _GLHiddenWidget_h
