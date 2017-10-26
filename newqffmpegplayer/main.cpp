/**
* Copyright(C) 2009-2012
* @author Jing HUANG   matrixvis.cn
* @file main.cpp
* @brief
* @date 1/2/2017
*/

#include "mainwindow.h"
#include <QApplication>
#undef main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
