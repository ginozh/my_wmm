#include "myopengl.h"
#include <QApplication>
#include <QtWidgets>
#include <QImage>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString fileName="c:\\shareproject\\jpg\\512img004.jpg";
    QImage image(fileName);
    if (image.isNull()) {
        return -1;
    }
    image = image.convertToFormat(QImage::Format_RGB888);
    QImage imagefrag(512,384,QImage::Format_RGB888);
    unsigned char *pixels = (unsigned char *) imagefrag.bits();

    int iret=opengl(image.bits(), pixels, 512, 384);
    qDebug()<<"iret: "<<iret;
    imagefrag.save("c:\\shareproject\\jpg\\512img004_frag.jpg");
    return 0;//a.exec();
}
