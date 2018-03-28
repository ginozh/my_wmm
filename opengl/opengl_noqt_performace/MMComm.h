#ifndef MMCOMM_H
#define MMCOMM_H


#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else

#ifdef __UBUNTU__
#include <QMap>
#else
#include <GL/glew.h>
//#include <GL/wglew.h>
#endif

#endif

#include <GLFW/glfw3.h>

#include <QMap>
#include <QVector>
#include <QImage>
#include <memory>
//#include <SDL.h>
#include <QVariant>

class ReaderBase;
//#define PIXEL_FORMAT GL_RGB

#define MAX_TEXTURES_CNT 10
const float position[12] = {
    -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
typedef struct {
    //const AVClass *class;
    ////GLuint        program;
    ////GLuint        frame_tex;
    GLFWwindow    *window=0;
    GLFWwindow    *threadWin=0;
    ////GLuint        pos_buf;
    ////GLuint v_shader; 
    ////GLuint f_shader;
    int w=0;
    int h=0;
} GenericShaderContext;

enum MMEffectType{
    MMET_UNKNOW=0,
    MMET_TRANSITION_FRAG,
    MMET_TRANSITION_FFMPEG,
    MMET_FILTER_FRAG,
    MMET_FILTER_FFMPEG,
    MMET_MOTION_FRAG,
    MMET_MOTION_FFMPEG,

    MMET_TRANSITION,
    MMET_FILTER,
    MMET_MOTION,
    MMET_SUBTITLE,
};
struct ZPMincontext;
#if 0
typedef struct STEffectsInfo {
    STEffectsInfo(){}
    STEffectsInfo(int t, const QString& n, ZPMincontext* z=NULL)
        :type(t),name(n),zoompan(z){}
    int type;
    QString name;
    ZPMincontext* zoompan;
}STEffectsInfo;
#endif
struct AVFrame;
typedef struct STTextureInfo {
    //~STTextureInfo();
    int index;
    QString filename;
    //QByteArray arrData;
    //AVFrame* avFrame=NULL;
    std::shared_ptr<QImage> image;
}STTextureInfo;
typedef struct STParaInfo {
    QByteArray name;
    QString type;
    QVariant deflt;
    QVariant min;
    QVariant max;
}STParaInfo;
typedef struct STFragmentInfo {
    STFragmentInfo(){}
    GLuint program;
    QString fragFile;
    QVector<STTextureInfo> textures;
    QVector<STParaInfo> paras;
}STFragmentInfo;


class MMGlobalContext
{
private:
    MMGlobalContext();
    static MMGlobalContext *m_pInstance;
public:
    static MMGlobalContext* instance();
    //void initialOpengl(int width, int height); //必须在主线程中初始化
    void initialOpengl(int width, int height,bool bForce=false); //必须在主线程中初始化
    static GLint createAndSetupTexture();
    static GLint load2DTexture(int w, int h,const unsigned char *pixels);
    //int fragRender(const QString& effectid, const uchar* bits);
#if 1
    int fragRenderForOtherThread(const QString& effectname, 
            const unsigned char* bits, float globaltime=0, float totaltime=0, 
            const unsigned char* bits1=NULL);
#endif
    int fragRenderForOtherThread(const QString& effectname, 
            const unsigned char* bits, int width, int height, float globaltime=0, float totaltime=0, 
            const unsigned char* bits1=NULL, int width2=0, int height2=0);
    int ParseConfCreateProgram(const QString pathpre, const QString& effectname);
    int createProgram(QString effectname);
    static void createBindFramebufferTexture(int w, int h);
    int LoadUniformAllTexture(const QString& effectname
            , const unsigned char* bits, int width, int height
            , const unsigned char* bits1, int width2, int height2);
    int UniformAllParament(const QString& effectname);
private:
public:
    //QMap<QString/*effectname*/, GLuint/*program*/> mapEffectProgram;
    QMap<QString/*effectname*/, STFragmentInfo > mapEffectProgram;
    GenericShaderContext* m_gs=NULL;
#if 0
    SDL_AudioDeviceID audioID=0;
    SDL_AudioSpec wanted_spec;
#endif
    ReaderBase* timeline=NULL;
#if 0
    enum AVSampleFormat sdl_sample_fmt;
    int sdl_channels;
    uint64_t sdl_channel_layout;
    int sdl_sample_rate;
#endif
    //QMap<QString /*id*/, STEffectsInfo > allEffects;
    QByteArray arrTexturesVar[MAX_TEXTURES_CNT];

    //for 帧缓冲
    static QVector<GLuint> textures;
    static QVector<GLuint> framebuffers;
};

#endif
