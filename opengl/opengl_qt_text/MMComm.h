#ifndef MMCOMM_H
#define MMCOMM_H

#include <QMap>
#include <QVector>
#include <QImage>
#include <memory>
#include <SDL.h>
#include <QVariant>
#include <QGLFunctions>
#include <QOpenGLFunctions>
#include <QGLFramebufferObject>
#include <QMutex>
#include <QMap>

#define MAX_TEXTURES_CNT 10
#if 1
//init ffmpeg
void initFFmpeg();
#endif
void Display(QImage& previewImage);
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
    STFboInfo(int idx, int w, int h);
    ~STFboInfo();
    int idxFrameBuf=-1;
    bool bUsed=false;
    bool bClean=true;
    GLuint framebuffer=0;
    GLuint fbotexture=0;
    QGLFramebufferObject* qfbo=NULL;
    //bool bFboUsed;
    //bool bTextureUsed;
}STFboInfo;

//class MMGlobalContext : public QObject, protected QGLFunctions
class MMGlobalContext : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
private:
    MMGlobalContext();
    static MMGlobalContext *m_pInstance;
public:
    static MMGlobalContext* instance();
public:
    QMap<QString /*id*/, STEffectsInfo > allEffects;
    ///QByteArray arrTexturesVar[MAX_TEXTURES_CNT];
    QMap<QString/*effectname*/, STFragmentInfo > m_mapEffectProgram;
};

#endif
