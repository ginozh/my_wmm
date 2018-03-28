#include <QDebug>
#include <QFile>
#include <QImage>
#include <QCoreApplication>
#include "glhiddenwidget.h"
#include "playerwidget.h"
GLuint globalid=0;
static int w=512;
static int h=384;
static QSize m_size(w,h);

static const float position[12] = {
    -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
static const GLchar *v_shader_source =
"attribute vec2 position;\n"
"varying vec2 texCoord;\n"
"void main(void) {\n"
"  gl_Position = vec4(position, 0, 1);\n"
"  texCoord = position;\n"
"}\n";
static GLuint VAOId, VBOId;
static GLuint programId;
static GenericShaderContext* gs;

GLuint GLHiddenWidget::build_shader(const GLchar *shader_source, GLenum type) 
{
    GLuint shader = glCreateShader(type);
    if (!shader || !glIsShader(shader)) {
        return 0;
    }
    qDebug()<<"GLHiddenWidget::build_shader shader: "<<shader<<" glIsShader: "<<glIsShader(shader)
        <<" 20: "<<glIsShader(20);
    glShaderSource(shader, 1, &shader_source, 0);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
    }
    return status == GL_TRUE ? shader : 0;
}
int GLHiddenWidget::build_program(GenericShaderContext *gs, const QString& fragSource) 
{
    GLuint v_shader, f_shader;
    v_shader = build_shader(v_shader_source, GL_VERTEX_SHADER);
    //f_shader = build_shader(f_shader_source, GL_FRAGMENT_SHADER);
    f_shader = build_shader(fragSource.toLocal8Bit().constData(), GL_FRAGMENT_SHADER);
    if (!(v_shader && f_shader )) {
        qDebug()<<"GLHiddenWidget::build_shader error";
        return -1;
    }

    gs->program = glCreateProgram();
    qDebug()<<"GLHiddenWidget::build_program gs->program: "<<gs->program;
    glAttachShader(gs->program, v_shader);
    glAttachShader(gs->program, f_shader);
    glLinkProgram(gs->program);

    GLint status;
    glGetProgramiv(gs->program, GL_LINK_STATUS, &status);
    return status == GL_TRUE ? 0 : -1;
}

GLHiddenWidget::GLHiddenWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMinimumSize(512, 384);
    gs = new GenericShaderContext;
    gs->w=w;
    gs->h=h;

}

GLHiddenWidget::~GLHiddenWidget()
{
}

void GLHiddenWidget::test(QOpenGLContext* sharecontext)
{
    QOpenGLContext * globalcon=QOpenGLContext::globalShareContext();
    //globalcon->makeCurrent(m_surface);
    //globalcon->makeCurrent(surface);
    if(globalcon && globalcon->isValid())
    {
        qDebug()<<"GLHiddenWidget::test is valid globalcon: "<<globalcon;
    }
    else
    {
        qDebug()<<"GLHiddenWidget::test is not valid globalcon: "<<globalcon;
    }
    qDebug()<<"GLHiddenWidget::test shareContext: "<<context()->shareContext()<<" context: "<<context();
    if(sharecontext)
    {
        sharecontext->makeCurrent(m_surface);
    }
    else
    {
        makeCurrent();
    }
    //context()->shareContext()->makeCurrent(m_surface);
    qDebug()<<"GLHiddenWidget::test currentthread: "<<QThread::currentThread()<<" belongthread: "<<context()->thread();
    qDebug()<<"GLHiddenWidget::test 9 glIsProgram: "<<glIsProgram(9);
    qDebug()<<"GLHiddenWidget::test 7 glIsShader: "<<glIsShader(7);
    qDebug()<<"GLHiddenWidget::test 2 glIsTexture: "<<glIsTexture(2);
    qDebug()<<"GLHiddenWidget::test 8 glIsTexture: "<<glIsTexture(8);


    QImage image;
    QString fileName="c:\\shareproject\\jpg\\512img001.jpg";
    image.load(fileName);
    if (image.isNull()) {
        qDebug()<<"error";
        return;
    }
    image = image.convertToFormat(QImage::Format_RGB888);
    const uchar* bits = image.bits();
    {
        // Step1 创建并绑定纹理对象
        glGenTextures(1, &gs->frame_tex);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gs->frame_tex);
        globalid = gs->frame_tex;
        qDebug()<<"GLHiddenWidget::test globalid: "<<globalid;
        // Step2 设定wrap参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // Step3 设定filter参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // Step4 加载纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gs->w, gs->h, 0,
                //GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, image.bits());
            GL_RGB, GL_UNSIGNED_BYTE, bits);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    for(GLuint txtid=1; txtid<=15; txtid++)
    {
        //qDebug()<<"GLHiddenWidget::test txtid: "<<txtid<<" glIsShader: "<<glIsShader(txtid);
    }
    if(sharecontext)
    {
        sharecontext->doneCurrent();
    }
    else
    {
        doneCurrent();
    }
    //context()->moveToThread(qApp->thread());
#if 0
    initial();
    GLuint textureId0;
    glGenTextures(1, &textureId0);
    glBindTexture(GL_TEXTURE_2D, textureId0);
    qDebug()<<"GLHiddenWidget::test textureId0: "<<textureId0;
#if 0
    glGenTextures(1, &textureId0);
    glBindTexture(GL_TEXTURE_2D, textureId0);
    qDebug()<<"GLHiddenWidget::test textureId0: "<<textureId0;
#endif
    //doneCurrent();
    for(GLuint txtid=1; txtid<=15; txtid++)
    {
        qDebug()<<"GLHiddenWidget::test txtid: "<<txtid<<" glIsShader: "<<glIsShader(txtid);
    }
#endif
}
void GLHiddenWidget::main2sub(QOpenGLContext* sharecontext)
{
    if(sharecontext)
    {
        sharecontext->makeCurrent(m_surface);
    }
    else
    {
        makeCurrent();
    }
    //context()->shareContext()->makeCurrent(m_surface);
    qDebug()<<"GLHiddenWidget::main2sub currentthread: "<<QThread::currentThread()<<" belongthread: "<<thread();
    qDebug()<<"GLHiddenWidget::main2sub 9 glIsProgram: "<<glIsProgram(9);
    qDebug()<<"GLHiddenWidget::main2sub 7 glIsShader: "<<glIsShader(7);
    qDebug()<<"GLHiddenWidget::main2sub 2 glIsTexture: "<<glIsTexture(2);
    qDebug()<<"GLHiddenWidget::main2sub 8 glIsTexture: "<<glIsTexture(8);
    if(sharecontext)
    {
        sharecontext->doneCurrent();
    }
    else
    {
        doneCurrent();
    }
}
void GLHiddenWidget::initializeGL()
{
    QOffscreenSurface *surface = new QOffscreenSurface;
    surface->setFormat(context()->format());
    surface->create();
    m_surface = surface;

    initializeOpenGLFunctions();
    for(GLuint txtid=1; txtid<=10; txtid++)
    {
        //qDebug()<<"GLHiddenWidget::initializeGL txtid: "<<txtid<<" glIsShader: "<<glIsShader(txtid);
    }
    GLuint textureId0;
    glGenTextures(1, &textureId0);
    glBindTexture(GL_TEXTURE_2D, textureId0);
    qDebug()<<"GLHiddenWidget::initializeGL textureId0: "<<textureId0;
#if 0
    glGenTextures(1, &textureId0);
    glBindTexture(GL_TEXTURE_2D, textureId0);
    qDebug()<<"GLHiddenWidget::initializeGL textureId0: "<<textureId0;
#endif
}
void GLHiddenWidget::initial()
{
    if(bInitial)
    {
        return;
    }
    QString effectid = "Aibao";
    qDebug()<<"GLHiddenWidget::initializeGL start";

    //s = new QWindow();
    //m_context->makeCurrent(s);
    makeCurrent();
    //initializeOpenGLFunctions();
    //initializeOpenGLFunctions();
    gs->w=w;
    gs->h=h;
    QString filePathPre=".";
    QString fileName=filePathPre+"/"+effectid+".frag";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qInfo()<<"error can't read theme file: "<<fileName;
        return ;//-1;
    }
    //QString fragSource = file.readAll();
#if 1
    QString fragSource = 
        "vec4 INPUT(vec2 tc);\n"
        "\n" + file.readAll()+
        "uniform sampler2D tex;\n"
        "varying vec2 texCoord;\n"
        "vec4 INPUT(vec2 tc)\n"
        "{\n"
        "//return texture2D(tex, texCoord * 0.5 + 0.5);\n"
        "return texture2D(tex, tc);\n"
        "}\n"
        "void main() {\n"
        "//gl_FragColor = texture2D(tex, texCoord * 0.5 + 0.5);\n"
        "gl_FragColor = FUNCNAME(texCoord * 0.5 + 0.5);\n"
        "//gl_FragColor = FUNCNAME(texCoord);\n"
        "}\n";
#endif
    file.close();
    int ret;
    if((ret = build_program(gs, fragSource)) < 0) {
        qDebug()<<"GLHiddenWidget build_program error: "<<ret;
        return ;//-2;
    }
    bInitial=true;
    qDebug()<<"GLHiddenWidget::initializeGL end";
}
void GLHiddenWidget::showEvent(QShowEvent *event)
{
    setVisible(false);
    QOpenGLWidget::showEvent(event);
}
