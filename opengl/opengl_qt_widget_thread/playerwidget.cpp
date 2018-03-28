#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <math.h>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QOpenGLFramebufferObject>
#include <QImage>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QTime>
#include <QWindow>
#include "playerwidget.h"

extern GLuint globalid;
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
//QWindow * s;

GLWidget* GLWidget::m_pInstance = NULL;
PlayerPrivate::PlayerPrivate(QObject *parent)
    :QThread(parent)
{
}
PlayerPrivate::~PlayerPrivate()
{
}
void PlayerPrivate::run()
{
    for (;;) 
    {
        GLWidget::instance()->initial();
        GLWidget::instance()->update();
        QThread::msleep(1000*1000);
    }
}
#if 1
GLWidget* GLWidget::instance()  
{  
    if(m_pInstance == NULL)  //判断是否第一次调用  
        m_pInstance = new GLWidget();  
    return m_pInstance;  
}  
#endif
GLuint GLWidget::build_shader(const GLchar *shader_source, GLenum type) 
{
    GLuint shader = glCreateShader(type);
    if (!shader || !glIsShader(shader)) {
        return 0;
    }
    qDebug()<<"GLWidget::build_shader shader: "<<shader<<" glIsShader: "<<glIsShader(shader)
        <<" 20: "<<glIsShader(20);
    glShaderSource(shader, 1, &shader_source, 0);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
    }
    return status == GL_TRUE ? shader : 0;
}
int GLWidget::build_program(GenericShaderContext *gs, const QString& fragSource) 
{
    GLuint v_shader, f_shader;
    v_shader = build_shader(v_shader_source, GL_VERTEX_SHADER);
    //f_shader = build_shader(f_shader_source, GL_FRAGMENT_SHADER);
    f_shader = build_shader(fragSource.toLocal8Bit().constData(), GL_FRAGMENT_SHADER);
    if (!(v_shader && f_shader )) {
        qDebug()<<"GLWidget::build_shader error";
        return -1;
    }

    gs->program = glCreateProgram();
    qDebug()<<"GLWidget::build_program gs->program: "<<gs->program;
    glAttachShader(gs->program, v_shader);
    glAttachShader(gs->program, f_shader);
    glLinkProgram(gs->program);

    GLint status;
    glGetProgramiv(gs->program, GL_LINK_STATUS, &status);
    return status == GL_TRUE ? 0 : -1;
}

GLWidget::GLWidget(QOpenGLWidget *shareWidget, QWidget *parent)
    : QOpenGLWidget(parent)
{
    gs = new GenericShaderContext;
    //m_context = new QOpenGLContext;
    //m_context->create();
#if 0
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
    if (m_transparent) {
        QSurfaceFormat fmt = format();
        fmt.setAlphaBufferSize(8);
        setFormat(fmt);
    }
#endif
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    //return QSize(50, 50);
    return QSize(w, h);
}

QSize GLWidget::sizeHint() const
{
    //return QSize(400, 400);
    return QSize(w, h);
}

void GLWidget::cleanup()
{
    makeCurrent();
    doneCurrent();
}

void GLWidget::initializeGL()
{
    //QOpenGLWidget::initializeGL();
    //initializeOpenGLFunctions();
    initial();
}

void GLWidget::initial()
{
    if(bInitial)
    {
        return;
    }
    QString effectid = "Aibao";
    qDebug()<<"GLWidget::initializeGL start";

    //s = new QWindow();
    //m_context->makeCurrent(s);
    makeCurrent();
    initializeOpenGLFunctions();
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
        qDebug()<<"GLWidget::build_program error: "<<ret;
        return ;//-2;
    }
    bInitial=true;
    qDebug()<<"GLWidget::initializeGL end";
}
#if 0
void GLWidget::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    qDebug()<<"GLWidget::initializeGL";
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, m_transparent ? 0 : 1);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_core ? fragmentShaderSourceCore : fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    // Setup our vertex buffer object.
    m_logoVbo.create();
    m_logoVbo.bind();
    m_logoVbo.allocate(m_logo.constData(), m_logo.count() * sizeof(GLfloat));

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs();

    // Our camera never changes in this example.
    m_camera.setToIdentity();
    m_camera.translate(0, 0, -1);

    // Light position is fixed.
    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

    m_program->release();
}
#endif

void GLWidget::paintGL()
{
    if(!bInitial)
    {
        return ;
    }
    qDebug()<<"GLWidget::paintGL start";
    qDebug()<<"GLWidget::paintGL currentthread: "<<QThread::currentThread()<<" belongthread: "<<context()->thread();
    QOpenGLContext * globalcon=QOpenGLContext::globalShareContext();
    if(globalcon && globalcon->isValid())
    {
        qDebug()<<"GLWidget::paintGL is valid globalcon: "<<globalcon;
    }
    else
    {
        qDebug()<<"GLWidget::paintGL isnot valid globalcon: "<<globalcon;
    }
    qDebug()<<"GLWidget::paintGL shareContext: "<<context()->shareContext();
    qDebug()<<"GLWidget::paintGL 9 glIsProgram: "<<glIsProgram(9);
    GLuint textureId0;
    glGenTextures(1, &textureId0);
    glBindTexture(GL_TEXTURE_2D, textureId0);
    qDebug()<<"GLWidget::paintGL textureId0: "<<textureId0;
#if 0
    glGenTextures(1, &textureId0);
    glBindTexture(GL_TEXTURE_2D, textureId0);
    qDebug()<<"GLWidget::paintGL textureId0: "<<textureId0;
#endif
    for(GLuint txtid=1; txtid<=15; txtid++)
    {
        //qDebug()<<"GLWidget::paintGL txtid: "<<txtid<<" glIsShader: "<<glIsShader(txtid);
    }

    QImage image;
    QString fileName="c:\\shareproject\\jpg\\512img001.jpg";
    image.load(fileName);
    if (image.isNull()) {
        qDebug()<<"error";
        return;
    }
    image = image.convertToFormat(QImage::Format_RGB888);
    const uchar* bits = image.bits();
    //openGLRenderer->render(image.bits());
    // 清除颜色缓冲区 重置为指定颜色
    //glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 这里填写场景绘制代码
    //glBindVertexArray(VAOId);
    {
        glGenBuffers(1, &VBOId);
        glBindBuffer(GL_ARRAY_BUFFER, VBOId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

        // Step4: 指定解析方式  并启用顶点属性
        // 顶点位置属性
        GLint loc = glGetAttribLocation(gs->program, "position");
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    //shader.use();
    glUseProgram(gs->program);
    // 启用多个纹理单元 绑定纹理对象
    //glActiveTexture(GL_TEXTURE0);
#if 0
    {
        // Step1 创建并绑定纹理对象
        glGenTextures(1, &gs->frame_tex);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gs->frame_tex);
        qDebug()<<"GLWidget::paintGL gs->frame_tex: "<<gs->frame_tex;
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
    GLint textureId1 = gs->frame_tex;
#endif
    GLint textureId1 = globalid;
    qDebug()<<"GLWidget::paintGL textureId1: "<<textureId1<<" glIsTexture: "<<glIsTexture(textureId1);
    // 启用多个纹理单元 绑定纹理对象
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    //glUniform1i(glGetUniformLocation(shader.programId, "tex1"), 0); // 设置纹理单元为0号
    glUniform1i(glGetUniformLocation(gs->program, "tex"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //glBindVertexArray(0);
    glUseProgram(0);
    qDebug()<<"GLWidget::paintGL end";
}
#if 0
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_world.setToIdentity();
    m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    glDrawArrays(GL_TRIANGLES, 0, m_logo.vertexCount());

    m_program->release();
}
#endif

void GLWidget::resizeGL(int w, int h)
{
}

