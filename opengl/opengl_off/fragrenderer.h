#ifndef FRAGRENDERER_H
#define FRAGRENDERER_H
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

typedef struct {
    //const AVClass *class;
    GLuint        program;
    GLuint        frame_tex;
    //GLFWwindow    *window;
    GLuint        pos_buf;
    int w;
    int h;
} GenericShaderContext;

class FragRenderer : protected QOpenGLFunctions
{
public:
    FragRenderer();
    ~FragRenderer();
    int render(const uchar*);
    int initialize(const QString& effectid, int width, int height);
    void readPixels(uchar** );
private:
    GLuint build_shader(const GLchar *shader_source, GLenum type);
    void vbo_setup(GenericShaderContext *gs);
    void tex_setup(GenericShaderContext *gs);
    int build_program(GenericShaderContext *gs, const QString& fragSource);
private:
    GenericShaderContext* gs;
};
#endif
