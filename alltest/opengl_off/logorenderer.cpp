#include "logorenderer.h"
#include <QPainter>
#include <QPaintEngine>
#include <math.h>

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
GLuint LogoRenderer::build_shader(const GLchar *shader_source, GLenum type) {
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
void LogoRenderer::vbo_setup(GenericShaderContext *gs) {
    glGenBuffers(1, &gs->pos_buf);
    glBindBuffer(GL_ARRAY_BUFFER, gs->pos_buf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    GLint loc = glGetAttribLocation(gs->program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
}
void LogoRenderer::tex_setup(GenericShaderContext *gs) {

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

int LogoRenderer::build_program(GenericShaderContext *gs) {
    GLuint v_shader, f_shader;

    if (!((v_shader = build_shader(v_shader_source, GL_VERTEX_SHADER)) &&
                (f_shader = build_shader(f_shader_source, GL_FRAGMENT_SHADER)))) {
        //av_log(NULL, AV_LOG_ERROR, "build_shader error\n");
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

LogoRenderer::LogoRenderer()
{
}

LogoRenderer::~LogoRenderer()
{
}


void LogoRenderer::paintQtLogo()
{
#if 0
    GLuint m_texture;
    glGenTextures(1, &m_texture);
    QString fileName="c:\\shareproject\\jpg\\512img005.jpg";
    QImage image(fileName);
    if (image.isNull()) {
        return;
    }

    image = image.convertToFormat(QImage::Format_RGB888);


    qDebug()<<"5 width: "<<image.width()<<" height: "<<image.height();
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0,
        GL_RGB, GL_UNSIGNED_BYTE, image.bits());
        //GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, image.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, 0);
#endif
#if 0
    program1.enableAttributeArray(normalAttr1);
    program1.enableAttributeArray(vertexAttr1);
    program1.setAttributeArray(vertexAttr1, vertices.constData());
    program1.setAttributeArray(normalAttr1, normals.constData());
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    program1.disableAttributeArray(normalAttr1);
    program1.disableAttributeArray(vertexAttr1);
#endif
}


void LogoRenderer::initialize()
{
    initializeOpenGLFunctions();
#if 1
    gs = new GenericShaderContext;
    gs->w=512;
    gs->h=384;

    //glViewport(0, 0, gs->w, gs->h);
    int ret;
    if((ret = build_program(gs)) < 0) {
        qDebug()<<"build_program error: "<<ret;
        return ;
    }

    glUseProgram(gs->program);
    vbo_setup(gs);
    tex_setup(gs);
#endif

#if 0
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    const char *vsrc1 =
        "attribute highp vec4 vertex;\n"
        "attribute mediump vec3 normal;\n"
        "uniform mediump mat4 matrix;\n"
        "varying mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    vec3 toLight = normalize(vec3(0.0, 0.3, 1.0));\n"
        "    float angle = max(dot(normal, toLight), 0.0);\n"
        "    vec3 col = vec3(0.40, 1.0, 0.0);\n"
        "    color = vec4(col * 0.2 + col * 0.8 * angle, 1.0);\n"
        "    color = clamp(color, 0.0, 1.0);\n"
        "    gl_Position = matrix * vertex;\n"
        "}\n";

    const char *fsrc1 =
        "varying mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = color;\n"
        "}\n";

    program1.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vsrc1);
    program1.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fsrc1);
    program1.link();

    vertexAttr1 = program1.attributeLocation("vertex");
    normalAttr1 = program1.attributeLocation("normal");
    matrixUniform1 = program1.uniformLocation("matrix");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    m_fAngle = 0;
    m_fScale = 1;
    createGeometry();
#endif
}

void LogoRenderer::render()
{
#if 1
    QString fileName="c:\\shareproject\\jpg\\512img004.jpg";
    QImage image(fileName);
    if (image.isNull()) {
        qDebug()<<"image.isNull";
        return;
    }

    image = image.convertToFormat(QImage::Format_RGB888);
    //image = image.convertToFormat(QImage::Format_ARGB32);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0,
        //GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, image.bits());
        GL_RGB, GL_UNSIGNED_BYTE, image.bits());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    QImage imagefrag(512,384,QImage::Format_RGB888);
    unsigned char *pixels = (unsigned char *) imagefrag.bits();
    glReadPixels(0, 0, 512, 384, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
    imagefrag.save("c:\\shareproject\\jpg\\512img005_frag2.jpg");
#endif
#if 0
    glDepthMask(true);

    glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    QMatrix4x4 modelview;
    modelview.rotate(m_fAngle, 0.0f, 1.0f, 0.0f);
    modelview.rotate(m_fAngle, 1.0f, 0.0f, 0.0f);
    modelview.rotate(m_fAngle, 0.0f, 0.0f, 1.0f);
    modelview.scale(m_fScale);
    modelview.translate(0.0f, -0.2f, 0.0f);

    program1.bind();
    program1.setUniformValue(matrixUniform1, modelview);
    paintQtLogo();
    program1.release();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    m_fAngle += 1.0f;
#endif
}

void LogoRenderer::createGeometry()
{
    vertices.clear();
    normals.clear();

    qreal x1 = +0.06f;
    qreal y1 = -0.14f;
    qreal x2 = +0.14f;
    qreal y2 = -0.06f;
    qreal x3 = +0.08f;
    qreal y3 = +0.00f;
    qreal x4 = +0.30f;
    qreal y4 = +0.22f;

    quad(x1, y1, x2, y2, y2, x2, y1, x1);
    quad(x3, y3, x4, y4, y4, x4, y3, x3);

    extrude(x1, y1, x2, y2);
    extrude(x2, y2, y2, x2);
    extrude(y2, x2, y1, x1);
    extrude(y1, x1, x1, y1);
    extrude(x3, y3, x4, y4);
    extrude(x4, y4, y4, x4);
    extrude(y4, x4, y3, x3);

    const qreal Pi = 3.14159f;
    const int NumSectors = 100;

    for (int i = 0; i < NumSectors; ++i) {
        qreal angle1 = (i * 2 * Pi) / NumSectors;
        qreal x5 = 0.30 * sin(angle1);
        qreal y5 = 0.30 * cos(angle1);
        qreal x6 = 0.20 * sin(angle1);
        qreal y6 = 0.20 * cos(angle1);

        qreal angle2 = ((i + 1) * 2 * Pi) / NumSectors;
        qreal x7 = 0.20 * sin(angle2);
        qreal y7 = 0.20 * cos(angle2);
        qreal x8 = 0.30 * sin(angle2);
        qreal y8 = 0.30 * cos(angle2);

        quad(x5, y5, x6, y6, x7, y7, x8, y8);

        extrude(x6, y6, x7, y7);
        extrude(x8, y8, x5, y5);
    }

    for (int i = 0;i < vertices.size();i++)
        vertices[i] *= 2.0f;
}

void LogoRenderer::quad(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4)
{
    vertices << QVector3D(x1, y1, -0.05f);
    vertices << QVector3D(x2, y2, -0.05f);
    vertices << QVector3D(x4, y4, -0.05f);

    vertices << QVector3D(x3, y3, -0.05f);
    vertices << QVector3D(x4, y4, -0.05f);
    vertices << QVector3D(x2, y2, -0.05f);

    QVector3D n = QVector3D::normal
        (QVector3D(x2 - x1, y2 - y1, 0.0f), QVector3D(x4 - x1, y4 - y1, 0.0f));

    normals << n;
    normals << n;
    normals << n;

    normals << n;
    normals << n;
    normals << n;

    vertices << QVector3D(x4, y4, 0.05f);
    vertices << QVector3D(x2, y2, 0.05f);
    vertices << QVector3D(x1, y1, 0.05f);

    vertices << QVector3D(x2, y2, 0.05f);
    vertices << QVector3D(x4, y4, 0.05f);
    vertices << QVector3D(x3, y3, 0.05f);

    n = QVector3D::normal
        (QVector3D(x2 - x4, y2 - y4, 0.0f), QVector3D(x1 - x4, y1 - y4, 0.0f));

    normals << n;
    normals << n;
    normals << n;

    normals << n;
    normals << n;
    normals << n;
}

void LogoRenderer::extrude(qreal x1, qreal y1, qreal x2, qreal y2)
{
    vertices << QVector3D(x1, y1, +0.05f);
    vertices << QVector3D(x2, y2, +0.05f);
    vertices << QVector3D(x1, y1, -0.05f);

    vertices << QVector3D(x2, y2, -0.05f);
    vertices << QVector3D(x1, y1, -0.05f);
    vertices << QVector3D(x2, y2, +0.05f);

    QVector3D n = QVector3D::normal
        (QVector3D(x2 - x1, y2 - y1, 0.0f), QVector3D(0.0f, 0.0f, -0.1f));

    normals << n;
    normals << n;
    normals << n;

    normals << n;
    normals << n;
    normals << n;
}
