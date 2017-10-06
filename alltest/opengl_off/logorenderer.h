#ifndef LOGORENDERER_H
#define LOGORENDERER_H

#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qopenglshaderprogram.h>
#include <QtGui/qopenglfunctions.h>

#include <QTime>
#include <QVector>
typedef struct {
    //const AVClass *class;
    GLuint        program;
    GLuint        frame_tex;
    //GLFWwindow    *window;
    GLuint        pos_buf;
    int w;
    int h;
} GenericShaderContext;

class LogoRenderer : protected QOpenGLFunctions
{
public:
    LogoRenderer();
    ~LogoRenderer();

    void render();
    void initialize();
    GLuint build_shader(const GLchar *shader_source, GLenum type);
    void vbo_setup(GenericShaderContext *gs);
    void tex_setup(GenericShaderContext *gs);
    int build_program(GenericShaderContext *gs);

private:

    qreal   m_fAngle;
    qreal   m_fScale;

    void paintQtLogo();
    void createGeometry();
    void quad(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4);
    void extrude(qreal x1, qreal y1, qreal x2, qreal y2);

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QOpenGLShaderProgram program1;
    int vertexAttr1;
    int normalAttr1;
    int matrixUniform1;

    GenericShaderContext* gs;
};
#endif
