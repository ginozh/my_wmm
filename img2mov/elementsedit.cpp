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
//#define DEBUG_FFMPEG
//! [1]
ElementsEdit::ElementsEdit(QWidget *parent)
    : QWidget(parent)
      //, m_flowLayout(new FlowLayout(this))
      , m_flowLayout(NULL)
      , m_vecticalLine(new QFrame(this))
      , m_lastSelectedImage(0)
      , m_idxCurrentImage(-1)
      , m_isFirstClick(true)
{   
    //m_tmpdir=QDir::currentPath().append(tr("/tmp"));

    setBackgroundRole(QPalette::Midlight);
    setCursor(QCursor(Qt::PointingHandCursor));

    m_duration=0;
    m_imgWidth=0;
    m_signalImgWidth=0;
    m_imgHeight=0;
    m_lineWidth=10;
    m_playPosition=0;
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
#if 0
    //test
    m_flowLayout->addWidget(new Element(this, tr("C:\\QtProjects\\qtmovie\\jpg\\img001.jpg")));
    m_flowLayout->addWidget(new Element(this, tr("C:\\QtProjects\\qtmovie\\jpg\\img002.jpg")));
    m_flowLayout->addWidget(new Element(this, tr("C:\\QtProjects\\qtmovie\\jpg\\img003.jpg")));
#endif
    //setLayout(m_flowLayout);

    m_vecticalLine->setObjectName(QStringLiteral("line"));
    m_vecticalLine->setEnabled(true);
    m_vecticalLine->hide();
    m_vecticalLine->setGeometry(QRect(300, 250, 10, 200));
    m_vecticalLine->setLineWidth(0);
    m_vecticalLine->setMidLineWidth(10);
    QPalette palette1;
    QBrush brush2(QColor(0, 0, 0, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Active, QPalette::Mid, brush2);
    palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
    palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
    m_vecticalLine->setPalette(palette1);
    m_vecticalLine->setFrameShape(QFrame::VLine);
    m_vecticalLine->setFrameShadow(QFrame::Sunken);

#if 0
    QSize iconSize(200, 200);
    QToolButton *m_addPhotos = new QToolButton();
    m_addPhotos->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_addPhotos->setIcon(QIcon("images/addphotos.png"));
    m_addPhotos->setIconSize(iconSize);
    m_addPhotos->setText("Add photos");
#endif
    m_firstLabel=new QLabel(this);
    m_firstLabel->setText(tr("Click here to browse for photos"));
    //label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    QFont font = m_firstLabel->font();
    font.setPointSize(25);
    font.setBold(true);
    m_firstLabel->setFont(font);
    //m_firstLabel->setAlignment(Qt::AlignCenter);
    m_firstLayout = new QVBoxLayout(this);
    //layout->addWidget(m_addPhotos, 0, Qt::AlignCenter);
    m_firstLayout->addWidget(m_firstLabel, 0, Qt::AlignCenter);
    setLayout(m_firstLayout);
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
int ElementsEdit::callFfmpeg(const QVector<QString>& vqsArgv)
{
    int ret;
	int charlist_size=vqsArgv.size();
    char **charlist;
	charlist=(char **)malloc(charlist_size*sizeof(char *));
//#define OUTPUT_WASTE
#ifdef OUTPUT_WASTE
    QString input;
#endif
	for(int i=0;i<charlist_size;i++){
		int strlen=vqsArgv[i].size()+1;
		charlist[i]=(char *)malloc(strlen);
        memset(charlist[i], 0, strlen);
		//charlist[i]=vqsArgv[i].toStdString().c_str();
        //snprintf(charlist[i],strlen, vqsArgv[i].toStdString().c_str());
		strcpy(charlist[i],vqsArgv[i].toStdString().c_str());
#ifdef OUTPUT_WASTE
        input.append(charlist[i]);
#endif
    }
    //QMessageBox::information(this, "info", input);
#ifdef OUTPUT_WASTE
    QTime startTime = QTime::currentTime();
#endif
    ret=qt_ffmpeg(charlist_size, charlist);

#ifdef OUTPUT_WASTE
    int dt = startTime.msecsTo(QTime::currentTime());
    qDebug()<< "ffmpeg waste: " << dt<< " ret: "<< ret;
    QMessageBox::information(this, "info", QString(tr("waste: %1 input: %2")).arg(dt).arg(input));
#endif
    return ret;
}
void ElementsEdit::createVideo(Element *element)
{
    QVector<QString> vqsArgv;
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif
    vqsArgv.push_back("-framerate");
    vqsArgv.push_back(QString(tr("25"))); //uncomplete 
    vqsArgv.push_back("-loop");
    vqsArgv.push_back("1");
    vqsArgv.push_back("-t");
    vqsArgv.push_back(QString(tr("2"))); //uncomplete 2(s)
    vqsArgv.push_back("-i");
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&element->m_fbInputScaleFile));//uncomplete
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(QString(tr("avi")));
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&element->m_fbScaleAniVideo));
    //vqsArgv.push_back(QString(tr("pic%1.avi")).arg(i));
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    element->m_fbInputAniVideo.ptr=element->m_fbScaleAniVideo.ptr;
    element->m_fbInputAniVideo.in_len=*element->m_fbScaleAniVideo.out_len;
    element->m_fbInputAniVideo.out_len=NULL;
}
void ElementsEdit::scaleImage(Element *element)
{
    //./ffmpeg -y -i 'jpg/img001.jpg' -vf scale=512:384 jpg/512img001.jpg
    QVector<QString> vqsArgv;
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif
    vqsArgv.push_back("-i");
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&element->m_fbOriFile));//uncomplete
    vqsArgv.push_back("-vf");
    vqsArgv.push_back(QString(tr("scale=%1")).arg("256:192")); //512:384
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(QString(tr("mjpeg")));
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&element->m_fbScaleFile));
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    element->m_fbInputScaleFile.ptr=element->m_fbScaleFile.ptr;
    element->m_fbInputScaleFile.in_len=*element->m_fbScaleFile.out_len;
    element->m_fbInputScaleFile.out_len=NULL;
}
//! [1]
void ElementsEdit::load()
{
    QWidget *currWidget = qobject_cast<QWidget *>(sender());
    //只有image过来的insert事件，它的父类(element)才会是m_flowLayout的元素
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"),
            /*QDir::currentPath()*/QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            "*.jpg *.png");
    if (files.count() == 0)
        return;
    setCursor(QCursor(Qt::WaitCursor));
    if(m_isFirstClick)
    {
        m_isFirstClick=false;

        //qDeleteAll(children());
        delete m_firstLayout;
        m_flowLayout = new FlowLayout(this);
        setLayout(m_flowLayout);
        delete m_firstLabel;
    }
    int idx = -1;
    if((idx=m_flowLayout->indexOf(currWidget))>=0)
        idx++;
    QString qsFileType;
    for (int i = 0; i < files.count(); ++i) {
        // 1, 读取文件，生成image
        Element *element=new Element(this, files[i]);
        m_flowLayout->insertWidget(idx, element);
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
        //2, 转化为小图片
        //QImage.scale: 1,是否支持内存读入文件内容。2，是否可以输出文件内容到buffer
        //or ffmpeg
        scaleImage(element);
        //3, 图片视频
        createVideo(element);
    }
    //4, 生成总视频
    //./ffmpeg_gr -y -f avi -i jpg/mi2.avi -f avi -i jpg/mm.avi -f avi jpg/all.avi
    //uncomplete
    createFinalVideoAndPlay();
    //6, 设置播放进度条
    //计算长度 for 进度条 ，需要在当前线程渲染之后，否则不对。放在duration信号槽内处理
    // 
    
    
    setCursor(QCursor(Qt::ArrowCursor));
}
void ElementsEdit::selectedImage()
{
    QWidget* send = qobject_cast<QWidget *>(sender());
    //1, 计算播放位移
    int imgWidth=0;
    for (int i = 0; i < m_flowLayout->count(); ++i)
    {
        Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            continue;
        }
        if(send == element)
        {
            break;
        }
        //QMessageBox::information(this, "info", QString(tr("imgWidth: %1")).arg(element->geometry().width()));
        imgWidth+=element->geometry().width();
    }
    emit verticalLineMoved(imgWidth*m_duration/m_imgWidth);

    // 2, 移除上次高亮
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
            if(!createAnimation(firstElement, secondElement ,"heart"))
            {
                // uncomplete
                return;
            }
        }
    }
    //4, 生成总视频
    createFinalVideoAndPlay();
}
bool ElementsEdit::createAnimation(Element *firstElement, Element *secondElement
        , const QString& animationName)
{
    //const QString& firstImageName = firstElement->getImageName();
    //const QString& secondImageName = secondElement->getImageName();
    //char **charlist;
    int iSecond=2;
    QVector<QString> vqsArgv;
    // blend="  crossfade";gdb --args ./ffmpeg_g   -y -framerate 25 -loop 1  -t 2 -i jpg/512img001.jpg -loop 1  -t 2 -i jpg/512img003.jpg     -filter_complex "blend=all_expr='if( $blend,1.5,2)'"    -pix_fmt yuv420p   jpg/mi.avi
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif
    vqsArgv.push_back("-framerate");
    vqsArgv.push_back("25");
    vqsArgv.push_back("-loop");
    vqsArgv.push_back("1");
    vqsArgv.push_back("-t");
    vqsArgv.push_back(QString(tr("%1")).arg(iSecond));
    vqsArgv.push_back("-i");
    //vqsArgv.push_back(firstImageName);
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&firstElement->m_fbInputScaleFile));//uncomplete
    vqsArgv.push_back("-loop");
    vqsArgv.push_back("1");
    vqsArgv.push_back("-t");
    vqsArgv.push_back(QString(tr("%1")).arg(iSecond));
    vqsArgv.push_back("-i");
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&secondElement->m_fbInputScaleFile));//uncomplete
    //vqsArgv.push_back(secondImageName);
    //ffmpeg -y -framerate 25 -loop 1 -t 1 -i img001.jpg -loop 1 -t 1 -i img002.jpg -filter_complex "blend=all_expr='if(gte(N*SW*50+X,W),B,A)'" -pix_fmt yuv420p  out.mkv
    vqsArgv.push_back("-filter_complex");
    //vqsArgv.push_back("blend=all_expr='if(gte(N*SW*50+X,W),B,A)'");
    vqsArgv.push_back(QString(tr("blend=all_expr='if(%1,1.5,%2)'")).arg(animationName).arg(iSecond));
    vqsArgv.push_back("-pix_fmt");
    vqsArgv.push_back("yuv420p");
#if 0
    QString vfileName("C:\\qtproject\\img2mov\\debug\\transition.avi");
    vqsArgv.push_back(vfileName);
#else
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(QString(tr("avi")));
    secondElement->m_fbScaleAniVideo.in_len = 10*1024*1024;
    *secondElement->m_fbScaleAniVideo.out_len = 10*1024*1024;
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&secondElement->m_fbScaleAniVideo));
#endif
    // -y -framerate 1 -i "C:\QtProjects\qtmovie\jpg\img%3d.jpg" myoutput.avi
    //QTime startTime = QTime::currentTime();
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    secondElement->m_fbInputAniVideo.ptr=secondElement->m_fbScaleAniVideo.ptr;
    secondElement->m_fbInputAniVideo.in_len=*secondElement->m_fbScaleAniVideo.out_len;
    secondElement->m_fbInputAniVideo.out_len=NULL;
    //int dt = startTime.msecsTo(QTime::currentTime());
    //qDebug()<< "ffmpeg waste: " << dt;
    //QMessageBox::information(this, "info", QString(tr("ffmpeg waste: %1")).arg(dt));
    //emit playVideo(vfileName);
    return true;
}
void ElementsEdit::createFinalVideoAndPlay()
{
    QVector<QString> vqsArgv;
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif
    //vqsArgv.push_back("-framerate");
    //vqsArgv.push_back("25"); //uncomplete
    vqsArgv.push_back("-f");
    vqsArgv.push_back("concat"); //uncomplete
    struct to_buffer sinbuffer;
    QByteArray sinString;
    for (int i = 0; i < m_flowLayout->count(); ++i)
    {
        Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            continue;
        }
        if(!element->m_fbInputAniVideo.ptr)
        {
            // err uncomplete
            continue;
        }
        //snprintf(in_buffer, len, "file buffer:video/avi;nobase64,%zu\n
        sinString.append(QString(tr("file buffer:video/avi;nobase64,%1\n")).arg((size_t)&element->m_fbInputAniVideo));
    }
    sinbuffer.ptr = (uint8_t*)sinString.data();
    sinbuffer.in_len = sinString.length();
    sinbuffer.out_len = NULL;
    vqsArgv.push_back(QString(tr("-i")));
    vqsArgv.push_back(QString(tr("buffer:video/avi;nobase64,%1")).arg((size_t)&sinbuffer));
    vqsArgv.push_back(QString(tr("-c")));
    vqsArgv.push_back(QString(tr("copy")));
    //printf("video input. stbuf: %p buf.ptr: %p in_len: %zu\n", &sinbuffer, sinbuffer.ptr, sinbuffer.in_len);

    uint8_t* out_buffer;
    size_t out_len = 10*1024*1024;
	out_buffer=(uint8_t *)malloc(out_len);

    struct to_buffer soutbuffer;
    soutbuffer.ptr = out_buffer;
    soutbuffer.in_len = out_len;
    soutbuffer.out_len = &out_len;
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(QString(tr("avi")));
    vqsArgv.push_back(QString(tr("buffer:video/avi;nobase64,%1")).arg((size_t)&soutbuffer));
    //vqsArgv.push_back(QString(tr("mm.avi")));
    //printf("video out. stbuf: %p buf.ptr: %p in_len: %zu out_len: %zu\n", &soutbuffer, soutbuffer.ptr, soutbuffer.in_len, *soutbuffer.out_len);
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    //5, 播放视频
    QByteArray tmp=QByteArray((const char*)soutbuffer.ptr, (int)out_len);
    emit playVideo("tmp.avi", tmp);
}
void ElementsEdit::mousePressEvent(QMouseEvent *event)
{
    if(m_isFirstClick)
    {
        load();
    }
    QWidget::mousePressEvent(event);
}
void ElementsEdit::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    assignProgress();
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
void ElementsEdit::durationChanged(qint64 duration)
{
    m_duration=duration;
    initialProgress(); //需要在load渲染之后被调用
}
void ElementsEdit::positionChanged(qint64 position)
{
    m_playPosition=position;
    assignProgress();
}
void ElementsEdit::initialProgress()
{
    m_imgWidth=0;
    m_imgHeight=0;
    for (int i = 0; m_flowLayout && i < m_flowLayout->count(); ++i)
    {
        Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            continue;
        }
        //QMessageBox::information(this, "info", QString(tr("imgWidth: %1")).arg(element->geometry().width()));
        if(m_imgHeight<element->geometry().height())
            m_imgHeight=element->geometry().height();
        if(m_signalImgWidth<element->geometry().width())
            m_signalImgWidth=element->geometry().width();
        m_imgWidth+=element->geometry().width();
    }
    //QMessageBox::information(this, "info", QString(tr("m_imgWidth: %1 imgHeight: %2")).arg(m_imgWidth).arg(imgHeight));
    if(m_flowLayout && m_flowLayout->count()>0)
    //for (int i = 0; i < m_flowLayout->count(); ++i)
    {
        //获取每个element的(x, y, width, height)
        //Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(0)->widget());
        //const QRect &rect=element->geometry();
        m_vecticalLine->setGeometry(QRect(0, 0, m_lineWidth, m_imgHeight)); //uncomplete
        m_vecticalLine->raise(); // top level, Raises this widget to the top of the parent widget's stack.
        //m_vecticalLine->setGeometry(QRect(rect.x(), rect.y(), 160, rect.height()));
        //m_vecticalLine->setWindowFlags(Qt::WindowStaysOnTopHint);
        m_vecticalLine->show();
        //QMessageBox::information(this, "info", QString(tr("m_lineWidth: %1 m_imgHeight: %2")).arg(m_lineWidth).arg(m_imgHeight));
    }
}
void ElementsEdit::assignProgress()
{
    if(m_duration && m_signalImgWidth)
    {
        int xMax=(geometry().width()/m_signalImgWidth)*m_signalImgWidth;
        //QMessageBox::information(this, "info", QString(tr("xMax: %1 width: %2 signalwidth: %3")).arg(xMax).arg(geometry().width()).arg(m_signalImgWidth));
        int xOri=m_playPosition*m_imgWidth/m_duration;
        int x=xOri%xMax;
        int y=(xOri/xMax)*m_imgHeight;
        //QMessageBox::information(this, "info", QString(tr("xMax: %1 xOri: %2 x: %3 y: %4 posi: %5 duration: %6")).arg(xMax).arg(xOri).arg(x).arg(y).arg(m_playPosition).arg(m_duration));
        m_vecticalLine->setGeometry(QRect(x, y, m_lineWidth, m_imgHeight)); //uncomplete
    }
}
