#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOffscreenSurface>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    opengl();
    return 0;//a.exec();
}
