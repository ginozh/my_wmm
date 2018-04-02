#ifndef _GLWidget_h_
#define _GLWidget_h_

#include <QGLWidget>
#include <QGLFunctions>
#include <memory>
#include <QImage>
#include <QMap>
#include <QVector>
#include <QVariant>
#include <QMutex>
#include <QFile>
#define MAX_TEXTURES_CNT 10
const float position[12] = {
    -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
enum MMEffectType{
    MMET_UNKNOW=0,
    MMET_TRANSITION=0x1,
    MMET_FILTER=0x2,
    MMET_MOTION=0x4,
    MMET_SUBTITLE=0x8,
};
enum MMTransitionType{
    MMTT_UNKOWN = 0,
    MMTT_OVERLAP,
    MMTT_PREFIX,
    MMTT_POSTFIX,
};
typedef struct STEffectsInfo {
    STEffectsInfo(){}
    STEffectsInfo(int t, const QString& n, QString dn="")
        :type(t),name(n),detailname(dn){}
    int type;
    QString name;
    QString detailname;
}STEffectsInfo;
typedef struct STRunningEffectInfo {
    STRunningEffectInfo(){}
    STRunningEffectInfo(int type, const QString& id, const QString& guid, const QString& name, const QString& detailname, int layer, float pos, float duration, int ttype)
        :m_effecttype(type), m_effectid(id), m_guid(guid), m_effectName(name), m_detailname(detailname),m_layer(layer), m_position(pos), m_duration(duration), m_ttype(ttype)
    { }
    int m_effecttype;
    //STEffectsInfo m_info;
    QString m_effectid;
    QString m_guid;
    QString m_effectName;
    QString m_detailname;
    int m_layer=0;
    float m_position=0.0f;  //单位秒. 相对于宿主的时间位移
    float m_duration=-1.0f;      //单位秒. -1表示宿主多长时间它就多长时间
    int m_ttype=0;      //transition: left(-1) middle(0) right(1)
}STRunningEffectInfo;
typedef struct STTextureInfo {
    //~STTextureInfo();
    int index;
    QString filename;
    //QByteArray arrData;
    std::shared_ptr<QImage> image;
}STTextureInfo;
typedef struct STParaInfo {
    QByteArray name;
    QString type;
    QVariant deflt;
    QVariant min;
    QVariant max;
}STParaInfo;
class STFragmentInfo {
public:
    STFragmentInfo(){
        refcnt=1;
        program=0;
        vshader=0;
        fshader=0;
    }
    int refcnt;
    GLuint program;
    GLuint vshader;
    GLuint fshader;
    QString fragFile;
    QVector<STTextureInfo> textures;
    //QVector<STParaInfo> paras;
    QMap<QString /*name*/, STParaInfo> paras;
};

typedef struct STFboInfo {
    STFboInfo();
    STFboInfo(int idx, GLuint fbo, GLuint texture, int w, int h);
    ~STFboInfo();
    GLuint framebuffer;
    GLuint fbotexture;
    int idxFrameBuf=-1;
    bool bUsed=false;
    //QOpenGLFramebufferObject* qfbo=NULL;
    //bool bFboUsed;
    //bool bTextureUsed;
}STFboInfo;

class GLWidget : public QGLWidget, protected QGLFunctions
{
	Q_OBJECT
public:
	//GLWidget(bool bHidden,QGLFormat format, const QGLWidget *shareWidget=0, QWidget *parent=0);
	GLWidget(bool bHidden,QGLFormat format, GLWidget *shareWidget=0, QWidget *parent=0);
	~GLWidget();
	bool initialize();
	void initializeGLContext();
protected:
	//virtual void glInit(); //overlay initializeGL
	//virtual void glDraw(); //overlay paintGL
    virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

	void resizeEvent(QResizeEvent *evt);
    void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *evt);

	//bool event(QEvent *e);
public:
    void initialOpengl(int width, int height,bool bForce=false); //必须在主线程中初始化
    int fragRenderForOtherThreadAgain(const QString& effectname
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2=0
        , bool useFbo=true, bool oneFrameFirstgl=false);
    int fragRenderForOtherThreadAgain(const QString& effectname, const STFragmentInfo& fragInfo
        , float* ori_spos, int ori_spossize, GLuint texture
        , float globaltime, float totaltime, GLuint texture2=0
        , bool useFbo=true, bool oneFrameFirstgl=false);
    STFboInfo* getNewFboInfo();
    STFboInfo* getFboInfo(int idx);
    void resetFboUsed(int idx);
    GLuint GetTexture(int idx);
    int fragRenderForFbo(STFboInfo* finalFboinfo);
    GLint load2DTexture(int w, int h,const unsigned char *pixels);
private:
    GLuint buildShader(const GLchar *shader_source, GLenum type);
    int createProgram(QString effectname);
    void createBindFramebufferTexture(int w, int h);
    GLint createAndSetupTexture();
    int LoadUniformAllTexture(const QString& effectname, const STFragmentInfo& fragInfo
        , GLuint texture , GLuint texture2);
    int UniformAllParament(const QString& effectname, const STFragmentInfo& fragInfo);
    int ParseConfCreateProgram(const QString pathpre, const QString& effectname);
private:
	//boost::atomic<int> m_init;
	int m_init=0;
    bool bInitialGL=false;
	QGLFormat m_format;
	bool m_resize;
public:
    QMap<QString/*effectname*/, STFragmentInfo > m_mapEffectProgram;
    QVector<STFboInfo> vFboInfo; //只能更新一次,因为给外面接口提供的是STFboInfo元素内存指针
    int idxFrameBuf;
    int maxFrameBuf;
    QMap<QString /*id*/, STEffectsInfo > allEffects;
    QByteArray arrTexturesVar[MAX_TEXTURES_CNT];
    QByteArray fragVertex;
    GLuint textureId[MAX_TEXTURES_CNT];
    int textureCnt=0;
    int glw=0;
    int glh=0;
    QMutex m_mutexRender;
	//const QGLWidget *m_shareWidget=NULL;
	GLWidget *m_shareWidget=NULL;
    int m_idxFbo=-1;
    int m_texture=-1;

    bool m_bhidden;
    QMutex m_mutexInit;
	QGLContext *m_glContext=NULL;
};

#endif // _GLWidget_h_
