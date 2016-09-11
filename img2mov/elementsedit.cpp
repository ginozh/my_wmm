#include <QPushButton>
#include <QPalette>
#include <QScrollBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDir>
#include <QVector>
#include <QTime>
#include <QDebug>

#include "elementsedit.h"
#include "element.h"
extern "C"{
#include "ffmpeg.h"
}
//! [1]
ElementsEdit::ElementsEdit(QWidget *parent)
    : QWidget(parent)
      , m_flowLayout(new FlowLayout(this))
      , m_lastSelectedImage(0)
      , m_idxCurrentImage(-1)
{   
    m_tmpdir=QDir::currentPath().append(tr("/tmp"));

    setBackgroundRole(QPalette::Dark);
#if 0
    m_flowLayout->addWidget(new QPushButton(tr("Short")));
    m_flowLayout->addWidget(new QPushButton(tr("Longer")));
    m_flowLayout->addWidget(new QPushButton(tr("Different text")));
    m_flowLayout->addWidget(new QPushButton(tr("More text")));
    m_flowLayout->addWidget(new QPushButton(tr("Even longer button text")));
#endif
    //m_flowLayout->setSpacing(0);
    //m_flowLayout->setMargin(0);
    //m_flowLayout->setContentsMargins(0,0,0,0);
#if 1
    /*
    m_flowLayout->addWidget(new Element());
    m_flowLayout->addWidget(new Element());
    m_flowLayout->addWidget(new Element());
    m_flowLayout->addWidget(new Element());
    m_flowLayout->addWidget(new Element());
    */
#if 1
    //test
    m_flowLayout->addWidget(new Element(this, tr("C:\\QtProjects\\qtmovie\\jpg\\img001.jpg")));
    m_flowLayout->addWidget(new Element(this, tr("C:\\QtProjects\\qtmovie\\jpg\\img002.jpg")));
    m_flowLayout->addWidget(new Element(this, tr("C:\\QtProjects\\qtmovie\\jpg\\img003.jpg")));
#endif
    setLayout(m_flowLayout);
#else
    QVBoxLayout *layout    = new QVBoxLayout(this);
    layout->addWidget(new Element());
    layout->addWidget(new Element());
    layout->addWidget(new Element());
    layout->addWidget(new Element());
    layout->addWidget(new Element());
    setLayout(layout);
#endif


#if 0
    setVisible(true);
    //adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    QScrollBar *scrollBar = horizontalScrollBar();
    double factor = 1.0;
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
#endif 
    setWindowTitle(tr("Flow Layout"));
}
//! [1]
void ElementsEdit::load()
{
    QWidget *currWidget = qobject_cast<QWidget *>(sender());
    //只有image过来的insert事件，它的父类(element)才会是m_flowLayout的元素
    int idx = -1;
    if((idx=m_flowLayout->indexOf(currWidget))>=0)
        idx++;
    //QString qstring = QString(tr("index is %1")).arg(idx);
    //QMessageBox::information(this, "Error Opening Picture", qstring);
    //qtconcurrent/imagescaling
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"),
            /*QDir::currentPath()*/QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            "*.jpg *.png");
    if (files.count() == 0)
        return;
    QString qsFileType;
    for (int i = 0; i < files.count(); ++i) {
        //m_flowLayout->addWidget(new Element(this, files[i]));
        m_flowLayout->insertWidget(idx, new Element(this, files[i]));
        QFileInfo fi(files[i]);
        QString ext = fi.suffix();  // ext = "gz"
        if(qsFileType.isEmpty())
        {
            qsFileType = ext;
        }
        else if(qsFileType.compare(ext)!=0)
        {
            QMessageBox::information(this, "error", QString(tr("need same file type(%1). current file: %2")).arg(qsFileType, files[i]));
            break;
        }
        if(idx>=0)
            idx++;
        //m_flowLayout->addWidget(new Element(this, files[i]), 1, 1);
    }
#if 0
    //1, delete old images
    QString curtmpdir, deltmpdir;
    if(m_tmpdir.compare(QDir::currentPath().append(tr("/tmp")))==0)
    {
        curtmpdir = m_tmpdir;
        deltmpdir = QDir::currentPath().append(tr("/tmp1"));
    }
    else
    {
        curtmpdir = m_tmpdir;
        deltmpdir = QDir::currentPath().append(tr("/tmp"));
    }
    m_tmpdir = deltmpdir;

    QDir dirdel(deltmpdir), dircur(curtmpdir);
    if(!dircur.exists())
    {
        bool isok = QDir::current().mkdir(curtmpdir);
        if(!isok)
            QMessageBox::information(this, "error", QString(tr("can't make dir: %1")).arg(curtmpdir));
    }
    if(!dirdel.exists())
    {
        bool isok = QDir::current().mkdir(deltmpdir);
        if(!isok)
            QMessageBox::information(this, "error", QString(tr("can't make dir: %1")).arg(deltmpdir));
    }
    else
    {
#if 0
        //bool isok = QDir::current().rmdir(deltmpdir);
        bool isok = dirdel.removeRecursively();
        if(!isok)
            QMessageBox::information(this, "error", QString(tr("can't rm dir: %1")).arg(deltmpdir));
        //QMessageBox::information(this, "info", QString(tr("dir %1 is exist")).arg(deltmpdir));
#else
        //multimedia/spectrum/app/engine.cpp
		const QStringList files = dirdel.entryList(QDir::Files);
		QString file;
		foreach (file, files)
			dirdel.remove(file);
        //dirdel.refresh();
#if 0
        bool isok = QDir::current().rmdir(deltmpdir);
        if(!isok)
            QMessageBox::information(this, "error", QString(tr("can't rm dir: %1")).arg(deltmpdir));
        //dirdel.refresh();
#endif

#endif
    }
    //QMessageBox::information(this, "info", QString(tr("remkdir: %1")).arg(curtmpdir));

    //2, copy image files
    QString fileNames, fileCopyNames;
    for (int i = 0; i < m_flowLayout->count(); ++i)
    {
        Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
        if (element != NULL)
        {
            const QString& imageName = element->getImageName();
            fileNames.append(imageName).append(tr(";\n"));
            size_t BufSize=5;
            char buf[BufSize];
            ::snprintf(buf, BufSize, "%.3d", i+1);
            QString no = QString::fromUtf8(buf);

            QString fileName = QString(tr("%1/img%2.%3")).arg(curtmpdir).arg(no).arg(qsFileType);
            fileCopyNames.append(fileName).append(tr(";\n"));
            //QMessageBox::information(this, "info", QString(tr("copy file: %1 to %2")).arg(imageName).arg(fileName));
            bool isok=QFile::copy(imageName , fileName);
            if(!isok)
                QMessageBox::information(this, "info", QString(tr("can't copy file: %1 to %2")).arg(imageName).arg(fileName));
        }
    }
#endif
    //QMessageBox::information(this, "info", QString(tr("files: %1. copyto: %2")).arg(fileNames).arg(fileCopyNames));

    //3. create mp4 file
    //QString vfileName("C:\\QtProjects\\qtmovie\\first.avi");
    //QString vfileName = QString(tr("%1/first.avi")).arg(curtmpdir);
    QString vfileName = QString(tr("%1/first.avi")).arg("tmp");
	char **charlist;
    QVector<QString> vqsArgv;
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
    vqsArgv.push_back("-framerate");
    vqsArgv.push_back("3");
    vqsArgv.push_back("-loop");
    vqsArgv.push_back("1");
    vqsArgv.push_back("-t");
    vqsArgv.push_back("1");
    vqsArgv.push_back("-i");
    //QString infileNames = QString(tr("%1/img%3d.jpg")).arg(curtmpdir);
    //vqsArgv.push_back(infileNames);
    //./ffmpeg_g.exe -y -framerate 25 -loop 1 -t 1 -i jpg/img001.jpg -pix_fmt yuv420p  -v
    //codec mpeg4 -f avi jpg/out.avi
    QString infileNames = QString(tr("C:\\QtProjects\\qtmovie\\jpg\\img001.jpg"));
        QFile file(infileNames);
        if (!file.open(QFile::ReadOnly)) {
            QMessageBox::warning(this, tr("Codecs"),
                                 tr("Cannot read file %1:\n%2")
                                 .arg(infileNames)
                                 .arg(file.errorString()));
            return;
        }
        m_playData = file.readAll();
        file.close();
    struct to_buffer sinbuffer, soutbuffer;
    sinbuffer.ptr = (uint8_t*)m_playData.data();
    sinbuffer.in_len = m_playData.size();
    sinbuffer.out_len = NULL;
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&sinbuffer));
    //vqsArgv.push_back("C:\\QtProjects\\qtmovie\\jpg\\img001.jpg");
    //vqsArgv.push_back(vfileName);
    uint8_t* out_buffer;
    size_t out_len = 10*1024*1024;
	out_buffer=(uint8_t *)malloc(out_len);

    soutbuffer.ptr = out_buffer;
    soutbuffer.in_len = out_len;
    soutbuffer.out_len = &out_len;
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(QString(tr("avi")));
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&soutbuffer));

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
    //QMessageBox::information(this, "info", QString(tr("start fmpeg")));
    QTime startTime = QTime::currentTime();
    qt_ffmpeg(charlist_size, charlist);

    int dt = startTime.msecsTo(QTime::currentTime());
    //qDebug()<< "ffmpeg waste: " << dt;
    //QMessageBox::information(this, "info", QString(tr("fileName: %1 waste: %2")).arg(vfileName).arg(dt));
    //4. let videoplay play 
    //emit playVideo(vfileName);
    QByteArray tmp=QByteArray((const char*)soutbuffer.ptr, (int)out_len);
#if 0
        QFile ofile(vfileName);
        if (!ofile.open(QFile::WriteOnly)) {
            QMessageBox::warning(this, tr("Codecs"),
                                 tr("Cannot read file %1:\n%2")
                                 .arg(vfileName)
                                 .arg(ofile.errorString()));
            return;
        }
        ofile.write((const char*)soutbuffer.ptr, (int)out_len);
        ofile.close();
    QMessageBox::information(this, "info", QString(tr("ofile: %1 len: %2")).arg(vfileName).arg(out_len));
#endif
    emit playVideo(tmp);
}
void ElementsEdit::selectedImage()
{
    QWidget* send = qobject_cast<QWidget *>(sender());
    if(send == m_lastSelectedImage)
        return;
    int iCurrentIdx=-1;
    if(send && (iCurrentIdx=m_flowLayout->indexOf(send))>=0)
    {
        m_idxCurrentImage = iCurrentIdx;
    }
    if(m_lastSelectedImage)
    {
        QWidget *lastWidget = qobject_cast<QWidget *>(m_lastSelectedImage);
        int idx = m_flowLayout->indexOf(lastWidget);
        if(idx<0)
        {
            QMessageBox::information(this, "error", QString(tr("can't find last widget")));
        }
        else
        {
            (qobject_cast<Element *>(m_lastSelectedImage))->unselectedImage();
        }
    }
    m_lastSelectedImage=send;
}
void ElementsEdit::selectedTransition()
{
    if(m_idxCurrentImage >= 1)
    {
        Element *firstElement = qobject_cast<Element *>(m_flowLayout->itemAt(m_idxCurrentImage-1)->widget());
        Element *secondElement = qobject_cast<Element *>(m_flowLayout->itemAt(m_idxCurrentImage)->widget());
        if (firstElement && secondElement)
        {
            const QString& firstImageName = firstElement->getImageName();
            const QString& secondImageName = secondElement->getImageName();
            char **charlist;
            QVector<QString> vqsArgv;
            vqsArgv.push_back("ffmpeg");
            vqsArgv.push_back("-y");
            vqsArgv.push_back("-v");
            vqsArgv.push_back("debug");
            vqsArgv.push_back("-framerate");
            vqsArgv.push_back("25");
            vqsArgv.push_back("-loop");
            vqsArgv.push_back("1");
            vqsArgv.push_back("-t");
            vqsArgv.push_back("1");
            vqsArgv.push_back("-i");
            vqsArgv.push_back(firstImageName);
            vqsArgv.push_back("-loop");
            vqsArgv.push_back("1");
            vqsArgv.push_back("-t");
            vqsArgv.push_back("1");
            vqsArgv.push_back("-i");
            vqsArgv.push_back(secondImageName);
            //ffmpeg -y -framerate 25 -loop 1 -t 1 -i img001.jpg -loop 1 -t 1 -i img002.jpg -filter_complex "blend=all_expr='if(gte(N*SW*50+X,W),B,A)'" -pix_fmt yuv420p  out.mkv
            vqsArgv.push_back("-filter_complex");
            vqsArgv.push_back("blend=all_expr='if(gte(N*SW*50+X,W),B,A)'");
            vqsArgv.push_back("-pix_fmt");
            vqsArgv.push_back("yuv420p");
            //QString vfileName = QString(tr("tmp/out.mkv"));
            QString vfileName("C:\\qtproject\\img2mov\\debug\\transition.avi");
            vqsArgv.push_back(vfileName);
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
            //QMessageBox::information(this, "info", QString(tr("start fmpeg")));
            QTime startTime = QTime::currentTime();
            qt_ffmpeg(charlist_size, charlist);
            int dt = startTime.msecsTo(QTime::currentTime());
            //qDebug()<< "ffmpeg waste: " << dt;
            QMessageBox::information(this, "info", QString(tr("vfileName: %1 ffmpeg waste: %2")).arg(vfileName).arg(dt));
            //emit playVideo(vfileName);
        }
    }
}
#if 0
void ElementsEdit::handleContextMenuRequested(const QPoint &pos)
{
    //sender indexOf
    QWidget *currWidget = qobject_cast<QWidget *>(sender());
    int idx = m_flowLayout->indexOf(currWidget);
    QString qstring = QString(tr("handleContextMenuRequested index is %1")).arg(idx);
    load();
}
#endif
ElementsEdit::~ElementsEdit()
{
    QString curtmpdir(QDir::currentPath().append(tr("/tmp"))), deltmpdir(QDir::currentPath().append(tr("/tmp1")));
    //QMessageBox::information(this, "info", QString(tr("~ElementsEdit. curtmpdir: %1 deltmpdir: %2")).arg(curtmpdir).arg(deltmpdir));
    QDir dirdel(deltmpdir), dircur(curtmpdir);
    if(dircur.exists())
    {
		const QStringList files = dircur.entryList(QDir::Files);
		QString file;
		foreach (file, files)
			dircur.remove(file);
    }
    if(dirdel.exists())
    {
		const QStringList files = dirdel.entryList(QDir::Files);
		QString file;
		foreach (file, files)
			dirdel.remove(file);
    }
}
