#ifndef _GLWidget_h
#define _GLWidget_h
#include <QGLWidget>
#include <QGLFunctions>
#include <QMap>
#include <QVector>
#include <QImage>
#include <memory>
#include <QVariant>
#include <QMutex>
#include "MMComm.h"

const float position[12] = {
    -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};

class GLWidget : public QGLWidget, protected QGLFunctions
{
	Q_OBJECT
public:
	//GLWidget(QGLFormat format, QWidget *parent=0);
	GLWidget(const QGLFormat &format, QWidget *parent = Q_NULLPTR, QGLWidget *shareWidget = Q_NULLPTR);
	virtual ~GLWidget();
protected:
	//virtual void glInit();
	//virtual void glDraw();

	virtual void initializeGL(); //setContext()之后;onece; paintGL() or resizeGL()之前; 
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

    //void resizeEvent(QResizeEvent *event);
    virtual void showEvent(QShowEvent *event);
    virtual void paintEvent(QPaintEvent *);
public:
    void initializeGLResource(int width, int height,bool bForce=false); //必须在主线程中初始化
    GLint createAndSetupTexture();
    GLint load2DTexture(int w, int h,const unsigned char *pixels);
    int fragRenderForOtherThreadAgain(const QString& effectname
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2=0
        , STFboInfo* finalFboinfo=NULL /*, bool useFbo=true*/, bool oneFrameFirstgl=false
        , int width=0, int height=0);
    int fragRenderForOtherThreadAgain(const QString& effectname, const STFragmentInfo& fragInfo
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2=0
        , STFboInfo* finalFboinfo=NULL /*,bool useFbo=true*/, bool oneFrameFirstgl=false
        , int width=0, int height=0);
    int LoadUniformAllTexture(const QString& effectname, const STFragmentInfo& fragInfo
        , GLuint texture , GLuint texture2);
    int UniformAllParament(const QString& effectname, const STFragmentInfo& fragInfo);
    int ParseConfCreateProgram(const QString pathpre, const QString& effectname);
    int RMEffectProgram(const QString& effectname);
    int RMEffectRef(const QString& effectname);
    int createProgram(QString effectname);
    void clearOpenglColor();
    void saveOpenglBuffer(int requested_frame=0);
    STFragmentInfo getFragmentInfo(const QString& effectname);
    void releaseResource();
    void deleteTexture(GLuint texture);
    //void checkMakeCurrent();
    //int makeCurrentOut();
    //int doneCurrentOut();
    virtual void setGLSize(int width, int height);
private:
    GLuint buildShader(const GLchar *shader_source, GLenum type);
public:
    int glw=0;
    int glh=0;
private:
    QByteArray fragVertex;
    GLuint textureId[MAX_TEXTURES_CNT];
    int textureCnt=0;
    QByteArray arrTexturesVar[MAX_TEXTURES_CNT];

    //QMutex m_mutexRender;
};

#endif // _GLWidget_h
