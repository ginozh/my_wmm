//#include <QtCore>
#include <QApplication>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
//#include <GL/wglew.h>
#endif

#include <GLFW/glfw3.h>

#include <QDebug>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QImage>
#include <memory>
#include <QFile>
#include <QTime>


#include <fstream>
#include <iostream>
using namespace std;

typedef struct {
    //const AVClass *class;
    GLuint        program;
    GLuint        frame_tex;
    GLFWwindow    *window;
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

static const GLchar *f_shader_source =
"vec4 INPUT(vec2 tc);\n"
"\n"
"vec4 rgb_to_xyz(vec4 color) {\n"
"float var_R = (color.r); //R from 0.0 to 255.0\n"
"float var_G = (color.g); //G from 0.0 to 255.0\n"
"float var_B = (color.b); //B from 0.0 to 255.0\n"
"\n"
"if (var_R > 0.04045) {\n"
"var_R = pow(((var_R + 0.055) / 1.055), 2.4);\n"
"} else {\n"
"var_R = var_R / 12.92;\n"
"}\n"
"if (var_G > 0.04045) {\n"
"var_G = pow(((var_G + 0.055) / 1.055), 2.4);\n"
"} else {\n"
"var_G = var_G / 12.92;\n"
"}\n"
"\n"
"if (var_B > 0.04045) {\n"
"var_B = pow(((var_B + 0.055) / 1.055), 2.4);\n"
"} else {\n"
"var_B = var_B / 12.92;\n"
"}\n"
"\n"
"var_R = var_R * 100.0;\n"
"var_G = var_G * 100.0;\n"
"var_B = var_B * 100.0;\n"
"\n"
"//Observer. = 2.0°, Illuminant = D65\n"
"float X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;\n"
"float Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;\n"
"float Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;\n"
"\n"
"return vec4(X, Y, Z, color.a);\n"
"}\n"
"\n"
"vec4 xyz_to_lab(vec4 color) {\n"
"\n"
"float ref_X = 95.047; //Observer= 2.0°, Illuminant= D65\n"
"float ref_Y = 100.000;\n"
"float ref_Z = 108.883;\n"
"\n"
"float var_X = color.r / ref_X;\n"
"float var_Y = color.g / ref_Y;\n"
"float var_Z = color.b / ref_Z;\n"
"\n"
"if (var_X > 0.008856) {\n"
"var_X = pow(var_X, (1.0 / 3.0));\n"
"} else {\n"
"var_X = (7.787 * var_X) + (16.0 / 116.0);\n"
"}\n"
"if (var_Y > 0.008856) {\n"
"var_Y = pow(var_Y, (1.0 / 3.0));\n"
"} else {\n"
"var_Y = (7.787 * var_Y) + (16.0 / 116.0);\n"
"}\n"
"if (var_Z > 0.008856) {\n"
"var_Z = pow(var_Z, (1.0 / 3.0));\n"
"} else {\n"
"var_Z = (7.787 * var_Z) + (16.0 / 116.0);\n"
"}\n"
"\n"
"float L = (116.0 * var_Y) - 16.0;\n"
"float a = 500.0 * (var_X - var_Y);\n"
"float b = 200.0 * (var_Y - var_Z);\n"
"\n"
"return vec4(L, a, b, color.a);\n"
"}\n"
"\n"
"vec4 lab_to_lch(vec4 color) {\n"
"\n"
"const float MPI = 3.14159265359;\n"
"\n"
"float var_H = atan(color.b, color.g); //in GLSL this is atan2\n"
"\n"
"if (var_H > 0.0) {\n"
"var_H = (var_H / MPI) * 180.0;\n"
"} else {\n"
"var_H = 360.0 - (abs(var_H) / MPI) * 180.0;\n"
"}\n"
"\n"
"float C = sqrt(pow(color.g, 2.0) + pow(color.b, 2.0));\n"
"float H = var_H;\n"
"\n"
"return vec4(color.r, C, H, color.a);\n"
"}\n"
"// ---------------------------\n"
"vec4 lch_to_lab(vec4 color) {\n"
"float a = cos(radians(color.b)) * color.g;\n"
"float b = sin(radians(color.b)) * color.g;\n"
"\n"
"return vec4(color.r, a, b, color.a);\n"
"}\n"
"\n"
"vec4 lab_to_xyz(vec4 color) {\n"
"float var_Y = (color.r + 16.0) / 116.0;\n"
"float var_X = color.g / 500.0 + var_Y;\n"
"float var_Z = var_Y - color.b / 200.0;\n"
"\n"
"if (pow(var_Y, 3.0) > 0.008856) {\n"
"var_Y = pow(var_Y, 3.0);\n"
"} else {\n"
"var_Y = (var_Y - 16.0 / 116.0) / 7.787;\n"
"}\n"
"if (pow(var_X, 3.0) > 0.008856) {\n"
"var_X = pow(var_X, 3.0);\n"
"} else {\n"
"var_X = (var_X - 16.0 / 116.0) / 7.787;\n"
"}\n"
"if (pow(var_Z, 3.0) > 0.008856) {\n"
"var_Z = pow(var_Z, 3.0);\n"
"} else {\n"
"var_Z = (var_Z - 16.0 / 116.0) / 7.787;\n"
"}\n"
"\n"
"float ref_X = 95.047; //Observer= 2.0 degrees, Illuminant= D65\n"
"float ref_Y = 100.000;\n"
"float ref_Z = 108.883;\n"
"\n"
"float X = ref_X * var_X;\n"
"float Y = ref_Y * var_Y;\n"
"float Z = ref_Z * var_Z;\n"
"\n"
"return vec4(X, Y, Z, color.a);\n"
"}\n"
"\n"
"vec4 xyz_to_rgb(vec4 color) {\n"
"float var_X = color.r / 100.0; //X from 0.0 to  95.047      (Observer = 2.0 degrees, Illuminant = D65);\n"
"float var_Y = color.g / 100.0; //Y from 0.0 to 100.000;\n"
"float var_Z = color.b / 100.0; //Z from 0.0 to 108.883;\n"
"\n"
"float var_R = var_X * 3.2406 + var_Y * -1.5372 + var_Z * -0.4986;\n"
"float var_G = var_X * -0.9689 + var_Y * 1.8758 + var_Z * 0.0415;\n"
"float var_B = var_X * 0.0557 + var_Y * -0.2040 + var_Z * 1.0570;\n"
"\n"
"if (var_R > 0.0031308) {\n"
"var_R = 1.055 * pow(var_R, (1.0 / 2.4)) - 0.055;\n"
"} else {\n"
"var_R = 12.92 * var_R;\n"
"}\n"
"if (var_G > 0.0031308) {\n"
"var_G = 1.055 * pow(var_G, (1.0 / 2.4)) - 0.055;\n"
"} else {\n"
"var_G = 12.92 * var_G;\n"
"}\n"
"if (var_B > 0.0031308) {\n"
"var_B = 1.055 * pow(var_B, (1.0 / 2.4)) - 0.055;\n"
"} else {\n"
"var_B = 12.92 * var_B;\n"
"}\n"
"\n"
"float R = var_R;\n"
"float G = var_G;\n"
"float B = var_B;\n"
"\n"
"return vec4(R, G, B, color.a);\n"
"}\n"
"\n"
"\n"
"vec4 rgb_to_lch(vec4 color) {\n"
"vec4 xyz = rgb_to_xyz(color);\n"
"vec4 lab = xyz_to_lab(xyz);\n"
"vec4 lch = lab_to_lch(lab);\n"
"return lch;\n"
"}\n"
"\n"
"\n"
"vec4 lch_to_rgb(vec4 color) {\n"
"vec4 lab = lch_to_lab(color);\n"
"vec4 xyz = lab_to_xyz(lab);\n"
"vec4 rgb = xyz_to_rgb(xyz);\n"
"return rgb;\n"
"}\n"
"\n"
"vec4 rgb_to_lab(vec4 color) {\n"
"vec4 xyz = rgb_to_xyz(color);\n"
"vec4 lab = xyz_to_lab(xyz);\n"
"return lab;\n"
"}\n"
"\n"
"vec4 lab_to_rgb(vec4 color) {\n"
"vec4 xyz = lab_to_xyz(color);\n"
"vec4 rgb = xyz_to_rgb(xyz);\n"
"return rgb;\n"
"}\n"
"\n"
"float thrdB(float l)\n"
"{\n"
"if (l < 10.)\n"
"return 0.;\n"
"float valIn = (l - 15.) / 85.;\n"
"float valOut = 1. - pow(1.-valIn,1.36);\n"
"return valOut * 100.;\n"
"}\n"
"\n"
"\n"
"vec4 FUNCNAME(vec2 tc)\n"
"{\n"
"vec4 col = INPUT(tc);\n"
"vec4 lab = rgb_to_lab(col);\n"
"lab.r = thrdB(lab.r);\n"
"return lab_to_rgb(lab);\n"
"}\n"
"\n"
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

#if 1
static GLuint build_shader(const GLchar *shader_source, GLenum type) {
    GLuint shader = glCreateShader(type);
    if (!shader || !glIsShader(shader)) {
        return 0;
    }
    glShaderSource(shader, 1, &shader_source, 0);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        int length;
        char *log;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        //log =(char*) av_malloc(length);//new char[length];
        //glGetShaderInfoLog(shader, length, &length, log);
        //av_log(NULL, AV_LOG_ERROR, "build_shader error: %s\n", log);
        //av_free  (log);
    }
    return status == GL_TRUE ? shader : 0;
}

static void vbo_setup(GenericShaderContext *gs) {
    glGenBuffers(1, &gs->pos_buf);
    glBindBuffer(GL_ARRAY_BUFFER, gs->pos_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    GLint loc = glGetAttribLocation(gs->program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

static void tex_setup(GenericShaderContext *gs) {

    glGenTextures(1, &gs->frame_tex);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, gs->frame_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gs->w, gs->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, NULL);

    glUniform1i(glGetUniformLocation(gs->program, "tex"), 0);
}
int render(GenericShaderContext *gs, const uchar* bits){
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    // 清除颜色缓冲区 重置为指定颜色
    glClearColor(0.18f, 0.04f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 这里填写场景绘制代码
    //glBindVertexArray(VAOId);
    {
        glGenBuffers(1, &gs->pos_buf);
        glBindBuffer(GL_ARRAY_BUFFER, gs->pos_buf);
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
#endif
class OpenGLRenderer 
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
        //initializeOpenGLFunctions();
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
        //QImage imagefrag(512,384,QImage::Format_RGB888);
        //unsigned char *pixels = (unsigned char *) imagefrag.bits();
        glReadPixels(0, 0, 512, 384, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
        //imagefrag.save("c:\\shareproject\\jpg\\512img005_frag.jpg");
#endif
    }
private:
	GLuint VAOId, VBOId;
	GLuint programId;
    GenericShaderContext* gs;
};
#if 0
void opengl()
{
#if 0
    GenericShaderContext* gs = new GenericShaderContext;
    gs->w=512;
    gs->h=384;
#endif
    int width=512;
    int height=512;
    GLFWwindow    *window;
    if (!glfwInit())
    {
        qDebug()<<"glfwInit. error ";
    }
    glfwWindowHint(GLFW_VISIBLE, 0);
    if(!(window = glfwCreateWindow(width, height, "", NULL, NULL)))
    {
        qDebug()<<"glfwCreateWindow. error ";
    }


    glfwMakeContextCurrent(window);
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        //av_log(NULL, AV_LOG_ERROR, "glewInit. error: %s\n", glewGetErrorString(err));
        qDebug()<<"glewInit. error: "<<glewGetErrorString(err);
    }
    //av_log(NULL, AV_LOG_ERROR, "Fragment 2\n");
#endif

    glViewport(0, 0, width, height);
    OpenGLRenderer* openGLRenderer = new OpenGLRenderer();
    openGLRenderer->initialize("Aibao", width, height);
    {
        //m_renderFbo->bind();
        QImage image;
        QString fileName="c:\\shareproject\\jpg\\512img001.jpg";
        image.load(fileName);
        if (image.isNull()) {
            qDebug()<<"error";
            return;
        }
        image = image.convertToFormat(QImage::Format_RGB888);
        openGLRenderer->render(image.bits());
        glFlush();
        QImage imagefrag(width,height,QImage::Format_RGB888);
        unsigned char *pixels = (unsigned char *) imagefrag.bits();
        openGLRenderer->readPixels(pixels);
        imagefrag.save("c:\\shareproject\\jpg\\512img004_frag.jpg");
        //m_renderFbo->bindDefault();
    }
}
#endif
#if 1
void opengl()
{
    GenericShaderContext* gs = new GenericShaderContext;
    gs->w=512;
    gs->h=384;
    if (!glfwInit())
    {
        qDebug()<<"glfwInit. error ";
    }
    glfwWindowHint(GLFW_VISIBLE, 0);
    if(!(gs->window = glfwCreateWindow(gs->w, gs->h, "", NULL, NULL)))
    {
        qDebug()<<"glfwCreateWindow. error ";
    }


    QTime startTime = QTime::currentTime();

    glfwMakeContextCurrent(gs->window);
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        //av_log(NULL, AV_LOG_ERROR, "glewInit. error: %s\n", glewGetErrorString(err));
        qDebug()<<"glewInit. error: "<<glewGetErrorString(err);
    }
    //av_log(NULL, AV_LOG_ERROR, "Fragment 2\n");
#endif

    int dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<< " waste_time glfwMakeContextCurrent. time: " << dt
        <<" start_time: "<<startTime.toString();

    glViewport(0, 0, gs->w, gs->h);

    // build_program
    GLuint v_shader, f_shader;
    v_shader = build_shader(v_shader_source, GL_VERTEX_SHADER);
    //av_log(NULL, AV_LOG_ERROR, "Fragment 2.1\n");
    f_shader = build_shader(f_shader_source, GL_FRAGMENT_SHADER);
    if (!(v_shader && f_shader )) {
        // av_log(NULL, AV_LOG_ERROR, "build_shader error\n");
        // return -1;
        qDebug()<<"build_shader error";
    }
    //av_log(NULL, AV_LOG_ERROR, "Fragment 3\n");

    gs->program = glCreateProgram();
    glAttachShader(gs->program, v_shader);
    glAttachShader(gs->program, f_shader);
    glLinkProgram(gs->program);

    GLint status;
    glGetProgramiv(gs->program, GL_LINK_STATUS, &status);
    //end build_program
    //av_log(NULL, AV_LOG_ERROR, "Fragment 4\n");

    glUseProgram(gs->program);
    vbo_setup(gs);
    //av_log(NULL, AV_LOG_ERROR, "Fragment 5\n");
    tex_setup(gs);
    //av_log(NULL, AV_LOG_ERROR, "Fragment 6\n");
    qDebug()<<"Fragment 6";

    {
        QString fileName="c:\\shareproject\\jpg\\512img001.jpg";
        QImage image(fileName);
        if (image.isNull()) {
            qDebug()<<"image.isNull";
            return;
        }

        //image = image.convertToFormat(QImage::Format_RGBA8888);
        image = image.convertToFormat(QImage::Format_RGB888);
        unsigned char *pixels = (unsigned char *) image.bits();
        //f->glReadPixels(0,0,gs->w,gs->h,GL_RGBA,GL_UNSIGNED_BYTE, imagefrag.bits());
        render(gs, pixels);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gs->w, gs->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, pixels);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glFlush();
        QImage imagefrag(gs->w,gs->h,QImage::Format_RGB888);
        unsigned char *mask_pixels = (unsigned char *) imagefrag.bits();
        glReadPixels(0, 0, gs->w, gs->h, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid *)mask_pixels);
        //glReadPixels(0, 0, gs->w, gs->h, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
        imagefrag.save("c:\\shareproject\\jpg\\512img001_frag.jpg");
        //screenshot("c:\\shareproject\\jpg\\512img005_frag.bmp");
    }
    {
        QString fileName="c:\\shareproject\\jpg\\512img002.jpg";
        QImage image(fileName);
        if (image.isNull()) {
            qDebug()<<"image.isNull";
            return;
        }

        //image = image.convertToFormat(QImage::Format_RGBA8888);
        image = image.convertToFormat(QImage::Format_RGB888);
        unsigned char *pixels = (unsigned char *) image.bits();
        //f->glReadPixels(0,0,gs->w,gs->h,GL_RGBA,GL_UNSIGNED_BYTE, imagefrag.bits());
        render(gs, pixels);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gs->w, gs->h, 0, PIXEL_FORMAT, GL_UNSIGNED_BYTE, pixels);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glFlush();
        QImage imagefrag(gs->w,gs->h,QImage::Format_RGB888);
        unsigned char *mask_pixels = (unsigned char *) imagefrag.bits();
        glReadPixels(0, 0, gs->w, gs->h, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid *)mask_pixels);
        //glReadPixels(0, 0, gs->w, gs->h, PIXEL_FORMAT, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
        imagefrag.save("c:\\shareproject\\jpg\\512img002_frag.jpg");
        //screenshot("c:\\shareproject\\jpg\\512img005_frag.bmp");
    }
}
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    opengl();
    return 0;//a.exec();
}
