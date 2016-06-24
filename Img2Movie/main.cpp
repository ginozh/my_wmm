#include "mainwindow.h"
#include <QApplication>
#include <QVector>
#include <QString>
extern "C"{
#include "ffmpeg.h"
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
#if 1 
	char **charlist;
    QVector<QString> vqsArgv;
    vqsArgv.push_back(argv[0]);
    vqsArgv.push_back("-y");
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
    vqsArgv.push_back("-framerate");
    vqsArgv.push_back("1");
    vqsArgv.push_back("-i");
    vqsArgv.push_back("C:\\QtProjects\\qtmovie\\jpg\\img%3d.jpg");
    vqsArgv.push_back("myoutput.avi");
    // -y -framerate 1 -i "C:\QtProjects\qtmovie\jpg\img%3d.jpg" myoutput.avi
	int charlist_size=vqsArgv.size();
	charlist=(char **)malloc(charlist_size*sizeof(char *));
	for(int i=0;i<charlist_size;i++){
		int strlen=vqsArgv[i].size()+1;
		charlist[i]=(char *)malloc(strlen);
        memset(charlist[i], 0, strlen);
		//charlist[i]=vqsArgv[i].toStdString().c_str();
        //snprintf(charlist[i],strlen, vqsArgv[i].toStdString().c_str());
		strcpy(charlist[i],vqsArgv[i].toStdString().c_str());
    }
    qt_ffmpeg(charlist_size, charlist);
#else
    qt_ffmpeg(argc, argv);
#endif

    w.show();

    return a.exec();
}
