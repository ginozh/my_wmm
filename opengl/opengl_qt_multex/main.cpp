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

typedef struct {
    //const AVClass *class;
    GLuint        program;
    GLuint        frame_tex;
    //GLFWwindow    *window;
    GLuint        pos_buf;
    int w;
    int h;
} GenericShaderContext;
#define PIXEL_FORMAT GL_RGB
static const float position[12] = {
    -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
static const GLchar *v_shader_source =
"attribute vec2 position;\n"
"varying vec2 texCoord;\n"
"void main(void) {\n"
"  gl_Position = vec4(position, 0, 1);\n"
"  texCoord = position;\n"
"}\n";

//class OpenGLRenderer : protected QOpenGLExtraFunctions
class OpenGLRenderer : protected QOpenGLFunctions
{
public:
    OpenGLRenderer()
        :programId(0)
    {
    }
    ~OpenGLRenderer(){
		if (this->programId)
		{
			glDeleteProgram(this->programId);
		}
    }
    int initialize(const QString& effectid, int width, int height){
        initializeOpenGLFunctions();
        gs = new GenericShaderContext;
        gs->w=width;
        gs->h=height;
#if 0
        // Step1: 创建并绑定VAO对象
        //glGenVertexArrays(1, &VAOId);
        //glBindVertexArray(VAOId);
        // Step2: 创建并绑定VBO 对象 传送数据
        glGenBuffers(1, &VBOId);
        glBindBuffer(GL_ARRAY_BUFFER, VBOId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

        // Step4: 指定解析方式  并启用顶点属性
        // 顶点位置属性
        GLint loc = glGetAttribLocation(gs->program, "position");
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindVertexArray(0);
#endif
        // Section2 准备着色器程序
        //Shader shader("triangle.vertex", "triangle.frag");
        QString filePathPre=".";
        QString fileName=filePathPre+"/"+effectid+".frag";
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qInfo()<<"error can't read theme file: "<<fileName;
            return -1;
        }
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
        file.close();
        int ret;
        if((ret = build_program(gs, fragSource)) < 0) {
            qDebug()<<"build_program error: "<<ret;
            return -2;
        }
        return 0;
    }
    GLuint build_shader(const GLchar *shader_source, GLenum type) {
        GLuint shader = glCreateShader(type);
        if (!shader || !glIsShader(shader)) {
            return 0;
        }
        glShaderSource(shader, 1, &shader_source, 0);
        glCompileShader(shader);
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE) {
        }
        return status == GL_TRUE ? shader : 0;
    }
    int build_program(GenericShaderContext *gs, const QString& fragSource) {
        GLuint v_shader, f_shader;
        v_shader = build_shader(v_shader_source, GL_VERTEX_SHADER);
        //f_shader = build_shader(f_shader_source, GL_FRAGMENT_SHADER);
        f_shader = build_shader(fragSource.toLocal8Bit().constData(), GL_FRAGMENT_SHADER);
        if (!(v_shader && f_shader )) {
            qDebug()<<"build_shader error";
            return -1;
        }

        gs->program = glCreateProgram();
        glAttachShader(gs->program, v_shader);
        glAttachShader(gs->program, f_shader);
        glLinkProgram(gs->program);

        GLint status;
        glGetProgramiv(gs->program, GL_LINK_STATUS, &status);
        return status == GL_TRUE ? 0 : -1;
    }
    int render(const uchar* bits){
#ifdef OUTPUT_WASTE
        QTime startTime = QTime::currentTime();
#endif
        // 清除颜色缓冲区 重置为指定颜色
        glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
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
        {
            // Step1 创建并绑定纹理对象
            glGenTextures(1, &gs->frame_tex);
            //glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gs->frame_tex);
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
        // 启用多个纹理单元 绑定纹理对象
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId1);
        //glUniform1i(glGetUniformLocation(shader.programId, "tex1"), 0); // 设置纹理单元为0号
        glUniform1i(glGetUniformLocation(gs->program, "tex"), 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //glBindVertexArray(0);
        glUseProgram(0);

#ifdef OUTPUT_WASTE
        int dt = startTime.msecsTo(QTime::currentTime());
        qDebug()<< " waste_time frag. time: " << dt
            <<" start_time: "<<startTime.toString();
#endif
        return 0;
    }
    void readPixels(uchar* pixels){
#if 1
#ifdef OUTPUT_WASTE
        QTime startTime = QTime::currentTime();
#endif
        //QImage imagefrag(512,384,QImage::Format_RGB888);
        //unsigned char *pixels = (unsigned char *) imagefrag.bits();
        glReadPixels(0, 0, 512, 384, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
        //imagefrag.save("c:\\shareproject\\jpg\\512img005_frag.jpg");
#ifdef OUTPUT_WASTE
        int dt = startTime.msecsTo(QTime::currentTime());
        qDebug()<< "glReadPixels frag. time: " << dt
            <<" start_time: "<<startTime.toString();
#endif
#endif
    }
private:
	GLuint VAOId, VBOId;
	GLuint programId;
    GenericShaderContext* gs;
};
void opengl()
{
    int width=512;
    int height=384;
    QSize m_size(width,height);

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setSamples(4);
    //fmt.setVersion(3, 2);
    fmt.setProfile(QSurfaceFormat::CoreProfile);

    // 创建不可见窗口
    QOffscreenSurface *surface;
    surface = new QOffscreenSurface();
    surface->setFormat(fmt);
    surface->create();

    //创建上下文
    QOpenGLContext *context;
    context = new QOpenGLContext();
    context->setFormat(fmt);
    //context->setShareContext(current);
    context->create();


	// 创建的窗口的context指定为当前context
    if(!context->makeCurrent(surface))
    {
        qDebug()<<"error";
        return;
    }
	// 设置视口参数
	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    context->functions()->glViewport(0, 0, m_size.width(), m_size.height());

    // Initialize the buffers and renderer
    QOpenGLFramebufferObject *m_renderFbo=0;
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    m_renderFbo = new QOpenGLFramebufferObject(m_size, format);

    OpenGLRenderer* openGLRenderer = new OpenGLRenderer();
    openGLRenderer->initialize("Aibao", width, height);
    {
        m_renderFbo->bind();
        QImage image;
        QString fileName="c:\\shareproject\\jpg\\512img001.jpg";
        image.load(fileName);
        if (image.isNull()) {
            qDebug()<<"error";
            return;
        }
        image = image.convertToFormat(QImage::Format_RGB888);
        openGLRenderer->render(image.bits());
        context->functions()->glFlush();
        QImage imagefrag(width,height,QImage::Format_RGB888);
        unsigned char *pixels = (unsigned char *) imagefrag.bits();
        openGLRenderer->readPixels(pixels);
        imagefrag.save("c:\\shareproject\\jpg\\512img001_frag.jpg");
        m_renderFbo->bindDefault();
    }
    {
        m_renderFbo->bind();
        QImage image;
        QString fileName="c:\\shareproject\\jpg\\512img002.jpg";
        image.load(fileName);
        if (image.isNull()) {
            qDebug()<<"error";
            return;
        }
        image = image.convertToFormat(QImage::Format_RGB888);
        openGLRenderer->render(image.bits());
        context->functions()->glFlush();
        QImage imagefrag(width,height,QImage::Format_RGB888);
        unsigned char *pixels = (unsigned char *) imagefrag.bits();
        openGLRenderer->readPixels(pixels);
        imagefrag.save("c:\\shareproject\\jpg\\512img002_frag.jpg");
        m_renderFbo->bindDefault();
    }
}
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    opengl();
    return 0;//a.exec();
}
