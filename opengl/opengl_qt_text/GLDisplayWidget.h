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
//#include "GLBaseWidget.h"
class GLHiddenWidget;
class Frame;
class FFmpegWriter;
class ExporterPrivate;
class GLDisplayWidget : public QGLWidget, protected QGLFunctions
//class GLDisplayWidget : public GLBaseWidget
{
	Q_OBJECT
public:
	//GLDisplayWidget(bool bHidden,QGLFormat format, const QGLWidget *shareWidget=0, QWidget *parent=0);
	GLDisplayWidget(QGLFormat& format, GLHiddenWidget *shareWidget, QWidget *parent=0, bool bExport=false);
	~GLDisplayWidget();
	void setShareGLContext();
    void initializeGLResource(int width, int height,bool bForce=false); //必须在主线程中初始化
    void setGLSize(int width, int height);
    int makeCurrentOut();
    int doneCurrentOut();
    std::shared_ptr<QImage> getImage();
    std::shared_ptr<QImage> getCapture();
signals:
    void sigGLPainted();
protected:
	virtual void glInit(); //overlay initializeGL
	virtual void glDraw(); //overlay paintGL
    virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

	///virtual void resizeEvent(QResizeEvent *evt);
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
	virtual void closeEvent(QCloseEvent *evt) Q_DECL_OVERRIDE;
private:
    ////GLuint bindTexture(GLuint texture);
public:
    GLHiddenWidget *m_shareWidget=NULL;
    std::shared_ptr<Frame> frame;
    std::shared_ptr<QImage> image;

    //test
    //FFmpegWriter* fw=NULL;
    //ExporterPrivate* ep=NULL;
    //bool m_bhidden=false;

    bool m_bExport=false;

    //int glw=0;
    //int glh=0;
    GLuint displayTexture=0;
private:
    QGLFormat m_format;
	QGLContext *m_glContext=NULL;
    QPushButton* m_pbPreFrame = NULL;
    int m_width=0, m_height=0; //显示视图长宽
};

#endif // _GLDisplayWidget_h_
