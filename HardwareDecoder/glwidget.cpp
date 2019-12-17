#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <QTime>
#include <QDebug>

bool GLWidget::m_transparent = false;
#define OUTPUT_WASTE
GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_xRot(0),
      m_yRot(0),
      m_zRot(0),
      m_program(0)
{
    m_core = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    if (m_transparent) {
        QSurfaceFormat fmt = format();
        fmt.setAlphaBufferSize(8);
        setFormat(fmt);
    }
}

GLWidget::~GLWidget()
{
    cleanup();
}

void GLWidget::setFrame(VideoFramePtr frame)
{
    m_frame = frame;
    update();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void GLWidget::cleanup()
{
    if (m_program == nullptr)
        return;
    makeCurrent();
    m_logoVbo.destroy();
    delete m_program;
    m_program = 0;
    doneCurrent();
}

static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec4 vertex;\n"
    "in vec3 normal;\n"
    "out vec3 vert;\n"
    "out vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "in highp vec3 vert;\n"
    "in highp vec3 vertNormal;\n"
    "out highp vec4 fragColor;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   fragColor = vec4(col, 1.0);\n"
    "}\n";

static const char *vertexShaderSource =
    "attribute vec4 vertex;\n"
    "attribute vec3 normal;\n"
    "varying vec3 vert;\n"
    "varying vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying highp vec3 vert;\n"
    "varying highp vec3 vertNormal;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   gl_FragColor = vec4(col, 1.0);\n"
    "}\n";

void GLWidget::initializeGL()
{
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();
#if 0
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
#endif
    initShaderProgram();
    initGeometry();

    glClearColor(0.f, 0.f, 0.f, 1.0f);

    glGenTextures(1,&m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void GLWidget::setupVertexAttribs()
{
    m_logoVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_logoVbo.release();
}

void GLWidget::paintGL()
{
    static int count=0;
    qDebug()<<"GLWidget::paintGL count: "<<(++count);
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    glDepthMask(GL_TRUE);

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    if (!m_frame.isNull())
        m_frame->map(m_texture);
#ifdef OUTPUT_WASTE
    int64_t wasteTime = startTime.msecsTo(QTime::currentTime());
    qInfo()<< "GLWidget::createHWVideoFrame waste: " << wasteTime;
#endif

    GLuint texture = m_texture;
#if defined(Q_OS_WIN)
    texture = 0;
#endif
    m_shaderProgram.setUniformValue("frameTexture", texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_shaderProgram.bind();

    m_shaderProgram.enableAttributeArray(m_in_tc);
    m_shaderProgram.enableAttributeArray(m_in_pos);
    m_shaderProgram.setAttributeArray(m_in_pos, m_vertices.constData());
    m_shaderProgram.setAttributeArray(m_in_tc, m_normals.constData());

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

    m_shaderProgram.disableAttributeArray(m_in_tc);
    m_shaderProgram.disableAttributeArray(m_in_pos);

    m_shaderProgram.release();

    if (!m_frame.isNull())
        m_frame->unmap();

    glFlush();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
#ifdef OUTPUT_WASTE
    int64_t wasteTimeAll = startTime.msecsTo(QTime::currentTime());
    qInfo()<< "GLWidget::paintGL waste_map: " << wasteTime<<" waste_all: "<<wasteTimeAll;
#endif
}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
    glvieww=w;glviewh=h;
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();
}

#include "d3d9decoder.h"
#include "surfaced3d9.h"
#if 0
VideoFrame* GLWidget::createHWVideoFrame(const AVFrame *frame)
{
    qDebug()<<"GLWidget::createHWVideoFrame";
    IDirect3DSurface9 *d3d9surface = (IDirect3DSurface9*)frame->data[3];
    SurfaceD3D9* videoSurface = new SurfaceD3D9(d3d9surface, frame->width, frame->height);
    return new VideoFrame(videoSurface);
}
#endif
void* GLWidget::createHWVideoFrame(const void *f)
{
    qDebug()<<"GLWidget::createHWVideoFrame";
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    const AVFrame *frame = (AVFrame*)f;
    IDirect3DSurface9 *d3d9surface = (IDirect3DSurface9*)frame->data[3];
    SurfaceD3D9* videoSurface = new SurfaceD3D9(d3d9surface, frame->width, frame->height);
#ifdef OUTPUT_WASTE
    int64_t wasteTime = startTime.msecsTo(QTime::currentTime());
    qInfo()<< "GLWidget::createHWVideoFrame waste: " << wasteTime;
#endif
    return (void*)new VideoFrame(videoSurface);
}
void GLWidget::initShaderProgram()
{
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, &m_shaderProgram);
    const char *vsrc =
            "varying mediump vec2 interp_tc;\n"
            "attribute mediump vec4 in_pos;\n"
            "attribute mediump vec4 in_tc;\n"
            "\n"
            "void main() {\n"
            "    interp_tc = in_tc.xy;\n"
            "    gl_Position = in_pos;\n"
            "}\n";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, &m_shaderProgram);
    const char *fsrc =
            "uniform sampler2D frameTexture; \n"
            "varying mediump vec2 interp_tc;\n"
            "void main() \n"
            "{ \n"
            "    gl_FragColor = texture2D(frameTexture, interp_tc);\n"
            "}\n";
    fshader->compileSourceCode(fsrc);

    m_shaderProgram.addShader(vshader);
    m_shaderProgram.addShader(fshader);
    m_shaderProgram.link();

    m_in_pos = m_shaderProgram.attributeLocation("in_pos");
    m_in_tc = m_shaderProgram.attributeLocation("in_tc");
}
void GLWidget::initGeometry()
{
    m_vertices.clear();
    m_vertices.clear();

    m_vertices << QVector3D(-1.0, -1.0, 0);
    m_vertices << QVector3D(1.0, -1.0, 0);
    m_vertices << QVector3D(-1.0, 1.0, 0);

    m_vertices << QVector3D(-1.0, 1.0, 0);
    m_vertices << QVector3D(1.0, -1.0, 0);
    m_vertices << QVector3D(1.0, 1.0, 0);

    m_normals << QVector3D(0.0, 0.0, 0);
    m_normals << QVector3D(1.0, 0.0, 0);
    m_normals << QVector3D(0.0, 1.0, 0);

    m_normals << QVector3D(0.0, 1.0, 0);
    m_normals << QVector3D(1.0, 0.0, 0);
    m_normals << QVector3D(1.0, 1.0, 0);
}
