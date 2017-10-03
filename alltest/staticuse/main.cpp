#include <QCoreApplication>
#include <QDebug>
#include "staticlib.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Staticlib* b=new Staticlib;
    qDebug()<<"b: "<<b;

    //return a.exec();
    return 0;
}
