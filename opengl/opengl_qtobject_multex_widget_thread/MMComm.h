#ifndef MMCOMM_H
#define MMCOMM_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

class MMGlobalContext : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
private:
    MMGlobalContext();
    static MMGlobalContext *m_pInstance;
public:
    static MMGlobalContext* instance();
    void initShaders();
    void initTextures();
    void initialOpengl(int width, int height,bool bForce=false); //必须在主线程中初始化
    int fragRenderForOtherThreadAgain();
public:
    int glw=0;
    int glh=0;
private:
    QOpenGLShaderProgram program;
    QOpenGLShaderProgram program1977;
    QOpenGLTexture *texture;
};

#endif
