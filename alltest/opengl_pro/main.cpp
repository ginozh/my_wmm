#include <QtCore>
#include <QtWidgets>
#include <QtOpenGL>
#include <QDebug>
#include "glextensions.h"

void opengl()
{
    QGLShader* m_vertexShader = new QGLShader(QGLShader::Vertex);
    m_vertexShader->compileSourceFile(QLatin1String("basic.vsh"));

    QGLShader* shader = new QGLShader(QGLShader::Fragment);
    shader->compileSourceFile("Aibao.frag");

    QGLShaderProgram *program = new QGLShaderProgram;
    program->addShader(m_vertexShader);
    program->addShader(shader);
    if (!program->link()) {
        qWarning("Failed to compile and link shader program");
        qWarning("Vertex shader log:");
        qWarning() << m_vertexShader->log();
        qWarning() << "Fragment shader log ( file =):";
        qWarning() << shader->log();
        qWarning("Shader program log:");
        qWarning() << program->log();

        delete shader;
        delete program;
    }
    program->bind();
    program->setUniformValue("tex", GLint(0));
    //1, 
    GLuint m_texture;
    glGenTextures(1, &m_texture);

    QString fileName="c:\\QtProjects\\qtmovie\\jpg\\1.jpg";
    QImage image(fileName);
    if (image.isNull()) {
        return;
    }

    image = image.convertToFormat(QImage::Format_ARGB32);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0,
        GL_BGRA, GL_UNSIGNED_BYTE, image.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, 0);

    //2
    ////glActiveTexture(GL_TEXTURE0);
    ///glBindTexture(GL_TEXTURE_2D, m_texture);

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    opengl();
    return 0;//a.exec();
}
