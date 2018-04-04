#ifndef _GLDisplayWidget_h_
#define _GLDisplayWidget_h_

#include <QGLWidget>
#include <QGLFunctions>
#include <memory>
#include <QImage>
#include <QMap>
#include <QVector>
#include <QVariant>
#include <QPushButton>
#include <QMutex>
#include <QFile>
#include "GLWidget.h"
class GLHiddenWidget;
//class GLDisplayWidget : public QGLWidget, protected QGLFunctions
class GLDisplayWidget : public GLWidget
{
	Q_OBJECT
public:
	//GLDisplayWidget(bool bHidden,QGLFormat format, const QGLWidget *shareWidget=0, QWidget *parent=0);
	GLDisplayWidget(QGLFormat format, GLHiddenWidget *shareWidget=0, QWidget *parent=0);
	~GLDisplayWidget();
	void setShareGLContext();
    void initializeGLResource(int width, int height,bool bForce=false); //必须在主线程中初始化
    void setGLSize(int width, int height);
protected:
	virtual void glInit(); //overlay initializeGL
	virtual void glDraw(); //overlay paintGL
    virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

	///void resizeEvent(QResizeEvent *evt);
    void showEvent(QShowEvent *event);
    ////void paintEvent(QPaintEvent *);
	void closeEvent(QCloseEvent *evt);
public:
    GLHiddenWidget *m_shareWidget=NULL;
    QMutex m_mutexRender;
    int m_idxFbo=-1;
    int m_texture=-1;
private:
    QGLFormat m_format;
	QGLContext *m_glContext=NULL;
    QPushButton* m_pbPreFrame = NULL;
    int m_width=0, m_height=0;
};

#endif // _GLDisplayWidget_h_
