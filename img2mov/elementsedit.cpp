#include <QPushButton>
#include <QPalette>
#include <QScrollBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QVector>
#include <QTime>
#include <QDebug>
#include "videoscene.h"
#include "videoplayer.h"

#include "elementsedit.h"
#include "element.h"
#include "musiclabel.h"
extern "C"{
#include "ffmpeg.h"
}
#include <QXmlStreamWriter>
#define DEBUG_FFMPEG
//! [1]
#define VIDEO_FILE_FORMAT "avi"
#define PROGRAM_TITLE "Film Maker"
ElementsEdit::ElementsEdit(QWidget *parent)
    : QWidget(parent)
      //, m_flowLayout(new FlowLayout(this))
      , m_flowLayout(NULL)
      , m_firstLayout(NULL)
      , m_firstLabel(NULL)
      , m_vecticalLine(new QFrame(this))
      , m_qFinalVideoSize(0,0)
      , m_lastSelectedElement(0)
      , m_idxCurrentElement(-1)
      , m_isFirstClick(true)
      , m_globalMusicAttr(new GlobalMusicAttr)
      , m_qsVideoFileFormat(VIDEO_FILE_FORMAT)
      , m_bCurrentProjectChanged(false)
{   
    m_globalContext = GlobalContext::instance();
    //m_tmpdir=QDir::currentPath().append(tr("/tmp"));

    setBackgroundRole(QPalette::Light);

    m_iTotalVideoDuration=0;
    m_imgWidth=0;
    m_signalImgWidth=0;
    m_imgHeight=0;
    m_lineWidth=10*m_globalContext->m_dFactorY;
    m_playPosition=0;

    m_imgPlayPosition = 0;

    m_outlen = m_outMaxLen = 10*1024*1024;
	m_pOutBuffer=(uint8_t *)malloc(m_outMaxLen);

    //m_textVideoOutLen = m_textVideoMaxOutLen = 10*1024*1024;
	//m_pTextVideoOutBuffer=(uint8_t *)malloc(m_textVideoMaxOutLen);
#if 1

    m_vecticalLine->setObjectName(QStringLiteral("line"));
    m_vecticalLine->setEnabled(true);
    //m_vecticalLine->setGeometry(QRect(300, 250, 10, 200));
    m_vecticalLine->setLineWidth(0);
    m_vecticalLine->setMidLineWidth(m_lineWidth);
    QPalette palette1;
    QBrush brush2(QColor(0, 0, 0, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Active, QPalette::Mid, brush2);
    palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
    palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
    m_vecticalLine->setPalette(palette1);
    m_vecticalLine->setFrameShape(QFrame::VLine);
    m_vecticalLine->setFrameShadow(QFrame::Sunken);

    initialFirstLayout();
#endif

    setWindowTitle(tr("Flow Layout"));
}
Element* ElementsEdit::currentElement()
{
    return qobject_cast<Element *>(m_lastSelectedElement);
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

/*
 * Start Image
 * */
void ElementsEdit::scaleImage(Element *element, QSize qScaleSize/*=QSize(0, 0)*/, bool bCreateVideoFile/*=false*/)
{
    if(qScaleSize.width()<=0)
    {
        qScaleSize=element->image()->globalImageAttr()->m_iScaledSize;
    }
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
    QString qsRotateVFilter = element->image()->rotateVFilter();
    qDebug()<< "ElementsEdit::scaleImage. qsRotateVFilter: "<<qsRotateVFilter<<" width: "<<qScaleSize.width()<<" height: "<<qScaleSize.height();
    vqsArgv.push_back(QString(tr("%1scale=%2:%3,setsar=1:1,setdar=4:3")).arg(qsRotateVFilter.isEmpty()?qsRotateVFilter:(qsRotateVFilter+",")).arg(qScaleSize.width()).arg(qScaleSize.height())); //512:384 256:192
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(QString(tr("mjpeg")));
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg(bCreateVideoFile?(size_t)&element->m_fbFileScaleFile:(size_t)&element->m_fbScaleFile));
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    if(bCreateVideoFile)
    {
        element->m_fbFileInputScaleFile.ptr=element->m_fbFileScaleFile.ptr;
        element->m_fbFileInputScaleFile.in_len=*element->m_fbFileScaleFile.out_len;
        element->m_fbFileInputScaleFile.out_len=NULL;
    }
    else
    {
        element->m_fbInputScaleFile.ptr=element->m_fbScaleFile.ptr;
        element->m_fbInputScaleFile.in_len=*element->m_fbScaleFile.out_len;
        element->m_fbInputScaleFile.out_len=NULL;
    }
}
//! [1]
void ElementsEdit::addImages()
{
    QWidget *currWidget = qobject_cast<QWidget *>(sender());
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"),
            /*QDir::currentPath()*/QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            tr("Photos (*.jpg *.png *.bmp *.dib *.rle *.gif *.ico *.icon *.jpeg *.jpe *.jfif *.exif *.tiff *.tif *wdp *.jxr)"));
    if (files.count() == 0)
        return;
    setCursor(QCursor(Qt::WaitCursor));
    createFlowLayout();
    int idx = -1;
    //只有image过来的insert事件，它的父类(element)才会是m_flowLayout的元素
    if((idx=m_flowLayout->indexOf(currWidget))>=0)
        idx++;
    int oldIdx = idx;
    QString qsFileType;
    for (int i = 0; i < files.count(); ++i) {
        // 1, 读取文件，生成image
#if 0
        Element *element=new Element(this, files[i], m_globalContext->m_scene);
        GlobalImageAttr* globalImageAttr = element->image()->globalImageAttr();
        if(m_qFinalVideoSize.width() < globalImageAttr->m_iSize.width())
        {
            m_qFinalVideoSize = globalImageAttr->m_iSize; //uncomplete. need same
        }
        //emit createVideoTextSignal((void*)element); 
        emit createVideoTextSignal(element); 
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
        createSimpleVideo(element);
        //4, 用于music的有效长度判断
        m_iTotalVideoDuration += element->globalVideoAttr()->m_iDuration;
#endif
        GlobalImageAttr  newGlobalImageAttr;
        newGlobalImageAttr.m_qsImageName = files[i];
        //initialImage(files[i], idx);
        if(! initialImage(newGlobalImageAttr, idx))
        {
        }
    }
    //4, 变更text的开始时间, 重新生成ass (前面增加一个图片、或者修改前面某个视频时长时)
    updateTextAttrAndAss(oldIdx+1);

    //5, 
    //5, 生成总视频
    //./ffmpeg_gr -y -f avi -i jpg/mi2.avi -f avi -i jpg/mm.avi -f avi jpg/all.avi
    //uncomplete
    createFinalVideo(false);
    //5, 显示music文件名和颜色
    musicAttrChanged();
    //6, 设置播放进度条
    //计算长度 for 进度条 ，需要在当前线程渲染之后，否则不对。放在duration信号槽内处理
    // 
    
    //7, 选中合适的image
    //oldIdx = oldIdx > 0? oldIdx: 0;
    //emit focusImageSignal();

    //8, 第一次选择
    if(!m_lastSelectedElement)
    {
        m_idxCurrentElement = 0;
        m_lastSelectedElement = qobject_cast<QWidget *>(m_flowLayout->itemAt(m_idxCurrentElement)->widget());
        (qobject_cast<Element *>(m_lastSelectedElement))->doFocusImage();
        emit displayVideoTextSignal(qobject_cast<Element *>(m_lastSelectedElement), true);
    }


    m_vecticalLine->raise(); // top level, Raises this widget to the top of the parent widget's stack.
    setCursor(QCursor(Qt::ArrowCursor));
}
void ElementsEdit::removeImage()
{
    QWidget *currWidget = qobject_cast<QWidget *>(sender());
    Element *currElement = qobject_cast<Element *>(sender());
    if(!currWidget || !currElement)
    {
        //error uncomple
        return ;
    }
    int idx = -1;
    if((idx=m_flowLayout->indexOf(currWidget))<0)
    {
        //error uncomplete
        qDebug()<< "error uncomplete. idx ElementsEdit::removeImage";
        return;
    }
    currElement->setValid(false); 
    //1, rm from flowlayout
    m_flowLayout->takeAt(idx);
    m_flowLayout->update();
    if(m_flowLayout->count()<=0)
    {
        currWidget->deleteLater();

        initialFirstLayout();
        return;
    }
    int newCurrIdx=-1;
    if(idx==m_flowLayout->count())
    {
        newCurrIdx=m_flowLayout->count()-1;
    }
    else
    {
        newCurrIdx = idx;
    }
    qDebug()<<"removeImages. idx: "<<idx<<" newCurrIdx: "<<newCurrIdx;
    //2, update textattr && ass
    updateTextAttrAndAss(newCurrIdx);
    //2, update m_iTotalVideoDuration
    m_iTotalVideoDuration -= currElement->globalVideoAttr()->m_iDuration;
    //3, create new video
    createFinalVideo(false);
    //3, 显示music文件名和颜色
    musicAttrChanged();
    //4, select new current image
    QWidget* theWidget = m_flowLayout->itemAt(newCurrIdx)->widget();
    selectedImage(theWidget);
    (qobject_cast<Element *>(theWidget))->doFocusImage();
    //5, delete object && delete object's member
    currWidget->deleteLater();
}
void ElementsEdit::selectedImage(QWidget* theWidget)
{
    QWidget* send = qobject_cast<QWidget *>(sender());
    Element* sendElement = qobject_cast<Element *>(sender());
    Element* theElement = qobject_cast<Element *>(theWidget);
    qDebug()<< "selectedImage. theElement: "<< (size_t)theElement<<" send: "<< (size_t)send;
    //QMessageBox::information(this, "info", QString(tr("selectedImage send: %1 m_lastSelectedElement: %2")).arg((size_t)send).arg((size_t)m_lastSelectedElement));
    //1, 计算播放位移
    //int imgWidth=0;
    int iStartDuration=0;
    for (int i = 0; i < m_flowLayout->count(); ++i)
    {
        Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            qDebug()<< "error uncomplete. ElementsEdit::selectedImage";
            continue;
        }
        if(send == element || theWidget == element)
        {
            break;
        }
        //QMessageBox::information(this, "info", QString(tr("imgWidth: %1")).arg(element->geometry().width()));
        //imgWidth+=element->geometry().width();
        iStartDuration += element->globalVideoAttr()->m_iDuration;
    }
    //if(m_imgWidth)
    {
        //m_imgPlayPosition = imgWidth*m_iTotalVideoDuration/m_imgWidth;
        m_imgPlayPosition = iStartDuration;
    }
    emit changePlayPosition(m_imgPlayPosition); 
    emit updatedVideoTimeTextSignal(m_imgPlayPosition, m_iTotalVideoDuration);

    // 2, 显示本次VideoText
    if(theWidget)
    {
        emit displayVideoTextSignal(theElement, true);
    }
    else
    {
        emit displayVideoTextSignal(sendElement, true);
    }
    if(!theWidget && send == m_lastSelectedElement)
        return;
    // 3, 移除上次高亮
    int iCurrentIdx=-1;
    if(( theWidget && (iCurrentIdx=m_flowLayout->indexOf(theWidget))>=0)
            || (send && (iCurrentIdx=m_flowLayout->indexOf(send))>=0)
      )
    {
        m_idxCurrentElement = iCurrentIdx;
    }
    if(m_lastSelectedElement)
    {
        QWidget *lastWidget = qobject_cast<QWidget *>(m_lastSelectedElement);
        int idx = m_flowLayout->indexOf(lastWidget);
        if(idx<0)
        {
            //QMessageBox::information(this, "error", QString(tr("can't find last widget")));
        }
        else
        {
            (qobject_cast<Element *>(m_lastSelectedElement))->unselectedImage();
            //移除上一次的VideoText的显示
            emit displayVideoTextSignal((qobject_cast<Element *>(m_lastSelectedElement)), false);
        }
    }
    // 4, 赋值
    m_lastSelectedElement = theWidget?theWidget:send;

    // 5, 赋值当前tab的值
    emit assignTabValueSignal();
}
//Element* ElementsEdit::initialImage(const QString& fileName, int& idx)
Element* ElementsEdit::initialImage(const GlobalImageAttr&  newGlobalImageAttr, int& idx)
{
    const QString& fileName = newGlobalImageAttr.m_qsImageName;
    QFileInfo fi(fileName);
    QString ext = fi.suffix();  // ext = "gz"
    if(m_qsFileType.isEmpty())
    {
        m_qsFileType = ext;
    }
    else if(m_qsFileType.compare(ext)!=0)
    {
        QMessageBox::information(this, "error", QString(tr("need same file type(%1). current file: %2")).arg(m_qsFileType, fileName));
        return NULL;
        //break;
    }
    Element *element=new Element(this, newGlobalImageAttr, m_globalContext->m_scene);
    GlobalImageAttr* globalImageAttr = element->image()->globalImageAttr();
    if(m_qFinalVideoSize.width() < globalImageAttr->m_iSize.width())
    {
        m_qFinalVideoSize = globalImageAttr->m_iSize; //uncomplete. need same
        qDebug()<< "ElementsEdit::initialImage. m_qFinalVideoSize w: "<<m_qFinalVideoSize.width()<<" h: "<< m_qFinalVideoSize.height();
    }
    //emit createVideoTextSignal((void*)element); 
    emit createVideoTextSignal(element); 
    m_flowLayout->insertWidget(idx, element);
    if(idx>=0)
        idx++;
    //2, 转化为小图片
    //QImage.scale: 1,是否支持内存读入文件内容。2，是否可以输出文件内容到buffer
    //or ffmpeg
    scaleImage(element);
    //3, 图片视频
    createSimpleVideo(element);
    //4, 用于music的有效长度判断
    m_iTotalVideoDuration += element->globalVideoAttr()->m_iDuration;

    return element;
}
/*
 * End Image
 * */

/*
 * Start Music 
 * */
void ElementsEdit::addMusic()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Add music"),
            /*QDir::currentPath()*/QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
            tr("Audio and Music (*.wma *.mp3 *.wav *.aif *.aiff *.m4a *.ogg)"));
    if (file.isEmpty() )
        return;
    //QFileInfo fi(file);
    //QString ext = fi.suffix();  // ext = "gz"
    m_musicMediaPlayer.setMedia(QUrl::fromLocalFile(file));
    connect(&m_musicMediaPlayer, SIGNAL(durationChanged(qint64)), this,
            SLOT(musicDurationChanged(qint64)));

    m_isFirstMusic = true;
    m_globalMusicAttr->m_qsMusicFullFilename = file;
    //m_globalMusicAttr->m_qsMusicFilename = fi.baseName();

    setCursor(QCursor(Qt::WaitCursor));
#if 0
    m_qsMusicFullFilename = file;
    m_iMusicStart = 0;
    m_iMusicDuration = 2;

    QVector<QString> vqsArgv;
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif
    //vqsArgv.push_back(QString(tr("-f")));
    //vqsArgv.push_back(QString(tr("%1")).arg(ext));
    vqsArgv.push_back(QString(tr("-ss")));
    vqsArgv.push_back(QString(tr("%1")).arg(m_iMusicStart));
    vqsArgv.push_back(QString(tr("-t")));
    vqsArgv.push_back(QString(tr("%1")).arg(m_iMusicDuration));
    vqsArgv.push_back(QString(tr("-i")));
    vqsArgv.push_back(m_qsMusicFullFilename);

    vqsArgv.push_back("-i");
    struct to_buffer sinbuffer;
    sinbuffer.ptr = (uint8_t*)m_pOutBuffer;
    sinbuffer.in_len = m_outlen;
    sinbuffer.out_len = NULL;
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&sinbuffer));
    //./ffmpeg_r.exe -y -i jpg/mp3.512.5.avi -vf ass=jpg/subtitle.ass jpg/subt.mp3.512.5.avi
    if(!m_qbAss.isEmpty())
    {
        vqsArgv.push_back(QString(tr("-vf")));
        struct to_buffer sintxtbuffer;
        sintxtbuffer.ptr = (uint8_t*)m_qbAss.data();
        sintxtbuffer.in_len = m_qbAss.length();
        sintxtbuffer.out_len = NULL;
        vqsArgv.push_back(QString(tr("ass=buffer|%1")).arg((size_t)&sintxtbuffer));
    }
#if 1
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(m_qsVideoFileFormat);
    struct to_buffer soutbuffer;
    soutbuffer.ptr = m_pTextVideoOutBuffer;
    soutbuffer.in_len = m_textVideoMaxOutLen;
    soutbuffer.out_len = &m_textVideoOutLen;
    vqsArgv.push_back(QString(tr("buffer:video/avi;nobase64,%1")).arg((size_t)&soutbuffer));
#else
    vqsArgv.push_back(QString(tr("txt.avi")));
#endif
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    setCursor(QCursor(Qt::ArrowCursor));
    //5, 播放视频
    QByteArray tmp=QByteArray((const char*)soutbuffer.ptr, (int)*soutbuffer.out_len);
    emit readyVideo("tmp.avi", tmp, m_imgPlayPosition);
    emit changePlayPosition(m_imgPlayPosition);
#endif
}
void ElementsEdit::selectedMusic()
{
    emit activeTabMusicSignal(NULL);
}
void ElementsEdit::musicAttrChanged()
{
    qint64 duration = m_globalMusicAttr->m_iEndPoint - m_globalMusicAttr->m_iStartPoint;
    int iStartDuration=0;
    bool bDrawMusicName=false;
    for (int i = 0; m_flowLayout && i < m_flowLayout->count(); ++i)
    {
        Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            qDebug()<< "error uncomplete. ElementsEdit::musicDurationChanged";
            continue;
        }
        if(iStartDuration >= m_globalMusicAttr->m_iStartTime + duration)
        {
            element->musicLabel()->updateMusicInfo(0, 0); //可能是中间插入一个图片,导致后面的music需要变更
            continue;
        }
        if(m_globalMusicAttr->m_iStartTime >= iStartDuration+element->globalVideoAttr()->m_iDuration)
        {
            iStartDuration+=element->globalVideoAttr()->m_iDuration;

            element->musicLabel()->updateMusicInfo(0, 0);
            continue;
        }
        QString qsMusicName;
        if(!bDrawMusicName)
        {
            // 只出现一次
            qsMusicName = m_globalMusicAttr->musicFileName();//m_qsMusicFilename;
            bDrawMusicName = true;
        }

        int iLeft=-1, iRight=-1;
        if(m_globalMusicAttr->m_iStartTime>=iStartDuration)
        {
            iLeft = element->musicLabel()->rect().width()*(m_globalMusicAttr->m_iStartTime - iStartDuration)/element->globalVideoAttr()->m_iDuration;
        }
        else
        {
            iLeft = 0;
        }
        if(iStartDuration + element->globalVideoAttr()->m_iDuration <= m_globalMusicAttr->m_iStartTime + duration)
        {
            // (m_globalMusicAttr->m_iStartTime - iStartDuration)/element->globalVideoAttr()->m_iDuration
            // == iLeft/element->musicLabel()->rect().width();
            iRight = element->musicLabel()->rect().width();
        }
        else
        {
            // iRight/element->musicLabel()->rect().width() 
            // == (duration+m_globalMusicAttr->m_iStartTime - iStartDuration)/element->globalVideoAttr()->m_iDuration
            iRight = element->musicLabel()->rect().width()*(duration+m_globalMusicAttr->m_iStartTime - iStartDuration)/element->globalVideoAttr()->m_iDuration;
        }
        element->musicLabel()->updateMusicInfo(iLeft, iRight, qsMusicName);
        qDebug()<<"iStartDuration: "<<iStartDuration<<" videoduration: "<<
            element->globalVideoAttr()->m_iDuration<<" music_duration: "<<duration<<
            " m_iStartTime: "<< m_globalMusicAttr->m_iStartTime <<
            " iLeft: "<< iLeft<<" iRight: "<<iRight<<" rect: "<<element->musicLabel()->rect().width();

        iStartDuration+=element->globalVideoAttr()->m_iDuration;
    }
}
//用于加载音乐获取duration
void ElementsEdit::musicDurationChanged(qint64 duration)
{
    if(m_isFirstMusic)
    {
        m_isFirstMusic = false;
        //1, create video
        m_globalMusicAttr->m_iMusicDuration = duration;
        m_globalMusicAttr->m_iEndPoint = duration;
        //createFinalVideoMusicTxt(false);
        createFinalVideo(false);

        //2, 显示music文件名和颜色
        musicAttrChanged();
#if 0
        int iStartDuration=0;
        for (int i = 0; m_flowLayout && i < m_flowLayout->count(); ++i)
        {
            Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
            if (!element)
            {
                // err uncomplete
                qDebug()<< "error uncomplete. ElementsEdit::musicDurationChanged";
                continue;
            }
            if(iStartDuration >= duration)
            {
                break;
            }
            QString qsMusicName = (i==0?m_globalMusicAttr->m_qsMusicFilename:"");
            int iRight=-1;
            if(iStartDuration + element->globalVideoAttr()->m_iDuration <= duration)
            {
                //element->musicLabel()->updateMusicInfo(iRight, qsMusicName);
            }
            else
            {
                iRight=element->musicLabel()->rect().width()*(duration-iStartDuration)/element->globalVideoAttr()->m_iDuration;
                //element->musicLabel()->updateMusicInfo(iRight, qsMusicName);
            }
            qDebug()<<"iStartDuration: "<<iStartDuration<<" videoduration: "<<
                element->globalVideoAttr()->m_iDuration<<" music_duration: "<<duration<<
                " iRight: "<<iRight<<" rect: "<<element->musicLabel()->rect().width();

            iStartDuration+=element->globalVideoAttr()->m_iDuration;
        }
#endif

        //3, update tabwidget
        emit activeTabMusicSignal(m_globalMusicAttr);
    }
}
/*
 * End Music
 * */
/*
 * Start Text
 * */
void ElementsEdit::addText()
{
    Element* element = currentElement();
    if(!element)
    {
        //uncomplete
        qDebug()<< "error uncomplete. ElementsEdit::addText";
        return;
    }
    element->lineEdit()->addTextByTabCaption();
}
void ElementsEdit::selectedText(const QString& oritxt)
{
    //只能先选定image，才能对text操作
    QWidget* send = qobject_cast<QWidget *>(sender());
    Element* currElement = qobject_cast<Element *>(sender());
    //QMessageBox::information(this, "info", QString(tr("selected send: %1 m_lastSelectedElement: %2")).arg((size_t)send).arg((size_t)m_lastSelectedElement));
    if(send == m_lastSelectedElement && send)
    {
        //QMessageBox::information(this, "info", QString(tr("send")));
        //QWidget *lastWidget = qobject_cast<QWidget *>(m_lastSelectedElement);
        //int idx = m_flowLayout->indexOf(lastWidget);
        if(!oritxt.isEmpty()) //uncomplete judge globaltext->text && globaltext->start && globaltext->duration
        {
            int iStartTime=0;
            for (int i = 0; i < m_flowLayout->count(); ++i)
            {
                Element* element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
                if(element == currElement)
                {
                    currElement->globalTextAttr()->m_iStartTimeText=iStartTime;
                    currElement->globalTextAttr()->m_iDurationText=element->globalVideoAttr()->m_iDuration;
                    break;
                }
                if(element && element->globalVideoAttr())
                {
                    iStartTime += element->globalVideoAttr()->m_iDuration;
                }
                else
                {
                    //uncomplete
                    qDebug()<< "error uncomplete. ElementsEdit::selectedText";
                }
            }
            //first time

        }
        //1, 激活对应VideoText
        emit activeVideoTextSignal(currElement, oritxt);
        //2, 激活对应tabTextTool
        emit activeTabTextSignal((void*)send);
    }
}
//void ElementsEdit::updatedText(stTextAttr* stTextAttr, const QString& qsText)
void ElementsEdit::updatedText(const QString& qsAss, const QString& qsText)
{
    Element* element = currentElement();
    if(element)
    {
        element->lineEdit()->updateTextByVideo(qsText);
    }
    else
    {
        //uncomplete
        qDebug()<< "error uncomplete. ElementsEdit::addText";
    }

    m_qbAss = qsAss.toUtf8();
    //createFinalVideoMusicTxt(false);
    createFinalVideo(false);
#if 0
    QVector<QString> vqsArgv;
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif
    if(!m_qsMusicFullFilename.isEmpty())
    {
        //vqsArgv.push_back(QString(tr("-f")));
        //vqsArgv.push_back(QString(tr("%1")).arg(ext));
        vqsArgv.push_back(QString(tr("-ss")));
        vqsArgv.push_back(QString(tr("%1")).arg(m_iMusicStart));
        vqsArgv.push_back(QString(tr("-t")));
        vqsArgv.push_back(QString(tr("%1")).arg(m_iMusicDuration));
        vqsArgv.push_back(QString(tr("-i")));
        vqsArgv.push_back(m_qsMusicFullFilename);
    }

    vqsArgv.push_back("-i");
    struct to_buffer sinbuffer;
    sinbuffer.ptr = (uint8_t*)m_pOutBuffer;
    sinbuffer.in_len = m_outlen;
    sinbuffer.out_len = NULL;
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&sinbuffer));
    //./ffmpeg_r.exe -y -i jpg/mp3.512.5.avi -vf ass=jpg/subtitle.ass jpg/subt.mp3.512.5.avi
    vqsArgv.push_back(QString(tr("-vf")));
    //QByteArray qsInTxt = createAss(stTextAttr, qsText);
    m_qbAss = qsAss.toUtf8();
    //QMessageBox::information(this, "info", QString(tr("qsAss: %1")).arg(qsAss));
    struct to_buffer sintxtbuffer;
    sintxtbuffer.ptr = (uint8_t*)m_qbAss.data();
    sintxtbuffer.in_len = m_qbAss.length();
    sintxtbuffer.out_len = NULL;
    vqsArgv.push_back(QString(tr("ass=buffer|%1")).arg((size_t)&sintxtbuffer));
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(m_qsVideoFileFormat);
#if 1
    struct to_buffer soutbuffer;
    soutbuffer.ptr = m_pTextVideoOutBuffer;
    soutbuffer.in_len = m_textVideoMaxOutLen;
    soutbuffer.out_len = &m_textVideoOutLen;
    vqsArgv.push_back(QString(tr("buffer:video/avi;nobase64,%1")).arg((size_t)&soutbuffer));
#else
    vqsArgv.push_back(QString(tr("txt.avi")));
#endif
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    //5, 播放视频
    QByteArray tmp=QByteArray((const char*)soutbuffer.ptr, (int)*soutbuffer.out_len);
    emit readyVideo("tmp.avi", tmp, m_imgPlayPosition);
    emit changePlayPosition(m_imgPlayPosition);
    //if(bPlay)
    {
        //emit playVideo();
    }
#endif
}
void ElementsEdit::updateTextAttrAndAss(int iStartIdx)
{
    int iStartTime=0;
    bool isChanged=false;
    for (int i = 0; i < m_flowLayout->count(); ++i)
    {
        Element* element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());

        //if(i>=iStartIdx && element->globalTextAttr() && !element->globalTextAttr()->m_qsStyle.isEmpty()) //uncomplete: m_qsStyle->m_qsText
        if(i>=iStartIdx && element->lineEdit() && !element->lineEdit()->videoText().isEmpty())
        {
            isChanged=true;
            element->globalTextAttr()->m_iStartTimeText = iStartTime;
        }

        if(element && element->globalVideoAttr())
        {
            iStartTime += element->globalVideoAttr()->m_iDuration;
        }
        else
        {
            //uncomplete
        }

    }
    if(isChanged && m_globalContext && m_globalContext->m_scene)
    {
        m_qbAss = m_globalContext->m_scene->createTotalAssInfo().toUtf8();
    }
}
/*
 * End Text
 * */
/*
 * Start Animation
 * */
void ElementsEdit::selectedTransition(const QString& animation)
{
#if 1
    currentElement()->globalAnimationAttr()->m_qsTransitionName = animation;
    createSingleVideo(m_idxCurrentElement);
#else
    if(m_idxCurrentElement >= 1)
    {
        Element *firstElement = qobject_cast<Element *>(m_flowLayout->itemAt(m_idxCurrentElement-1)->widget());
        Element *secondElement = qobject_cast<Element *>(m_flowLayout->itemAt(m_idxCurrentElement)->widget());
        if (firstElement && secondElement && secondElement->globalAnimationAttr()
                && animation.compare(secondElement->globalAnimationAttr()->m_qsTransitionName)!=0 )
        {
            if(createAnimation(firstElement, secondElement ,animation))
            {
                secondElement->globalAnimationAttr()->m_qsTransitionName = animation;
            }
            else
            {
                // uncomplete
                return;
            }
        }
        else
        {
            // uncomplete
            return;
        }
        //4, 生成总视频
        createFinalVideo(true);
    }
#endif
    //4, 生成总视频
    createFinalVideo(true);
}
#if 0
bool ElementsEdit::createAnimation(Element *firstElement, Element *secondElement
        , const QString& animationName)
#else
bool ElementsEdit::createAnimation(Element *firstElement, Element *secondElement, bool isFromPanzoom, bool bCreateVideoFile/*=false*/)
#endif
{
    qDebug()<< "createAnimation";
    //const QString& firstImageName = firstElement->getImageName();
    //const QString& secondImageName = secondElement->getImageName();
    //char **charlist;
    GlobalVideoAttr* globalVideoAttr = secondElement->globalVideoAttr();
    GlobalAnimationAttr* globalAnimationAttr = secondElement->globalAnimationAttr();
    QString duration = QString::number((float)globalVideoAttr->m_iDuration/1000, 'f', 2);
    QString animationDuration = QString::number((float)globalAnimationAttr->m_iTransitionDuration/1000, 'f', 2);

    QVector<QString> vqsArgv;
    // blend="  crossfade";gdb --args ./ffmpeg_g   -y -framerate 25 -loop 1  -t 2 -i jpg/512img001.jpg -loop 1  -t 2 -i jpg/512img003.jpg     -filter_complex "blend=all_expr='if( $blend,1.5,2)'"    -pix_fmt yuv420p   jpg/mi.avi
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif

#if 0
    vqsArgv.push_back("-framerate");
    vqsArgv.push_back(QString(tr("%1")).arg(globalVideoAttr->m_iFramerate));
    vqsArgv.push_back("-loop");
    vqsArgv.push_back("1");
    vqsArgv.push_back("-t");
    vqsArgv.push_back(QString(tr("%1")).arg(duration));
    vqsArgv.push_back("-i");
    //vqsArgv.push_back(firstImageName);
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&firstElement->m_fbInputScaleFile));//uncomplete
#endif
    vqsArgv.push_back("-i");
    if(isFromPanzoom)
    {
        vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg(bCreateVideoFile?(size_t)&firstElement->m_fbFileInputPanzoomVideo:(size_t)&firstElement->m_fbInputPanzoomVideo));
    }
    else
    {
        vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg(bCreateVideoFile?(size_t)&firstElement->m_fbFileInputScaledVideo:(size_t)&firstElement->m_fbInputScaledVideo));
    }

#if 0
    vqsArgv.push_back("-loop");
    vqsArgv.push_back("1");
    vqsArgv.push_back("-t");
    vqsArgv.push_back(QString(tr("%1")).arg(duration));
    vqsArgv.push_back("-i");
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&secondElement->m_fbInputScaleFile));//uncomplete
#endif
    vqsArgv.push_back("-i");
    if(isFromPanzoom)
    {
        vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg(bCreateVideoFile?(size_t)&secondElement->m_fbFileInputPanzoomVideo:(size_t)&secondElement->m_fbInputPanzoomVideo));
    }
    else
    {
        vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg(bCreateVideoFile?(size_t)&secondElement->m_fbFileInputScaledVideo:(size_t)&secondElement->m_fbInputScaledVideo));
    }
    //vqsArgv.push_back(secondImageName);
    //ffmpeg -y -framerate 25 -loop 1 -t 1 -i img001.jpg -loop 1 -t 1 -i img002.jpg -filter_complex "blend=all_expr='if(gte(N*SW*50+X,W),B,A)'" -pix_fmt yuv420p  out.mkv
    vqsArgv.push_back("-filter_complex");
    //vqsArgv.push_back("blend=all_expr='if(gte(N*SW*50+X,W),B,A)'");
    vqsArgv.push_back(QString(tr("blend=all_expr='if(%1,%2,%3)'")).arg(globalAnimationAttr->m_qsTransitionName).arg(animationDuration).arg(duration));
    vqsArgv.push_back("-pix_fmt");
    vqsArgv.push_back("yuv420p");
#if 0
    QString vfileName("C:\\qtproject\\img2mov\\debug\\transition.avi");
    vqsArgv.push_back(vfileName);
#else
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(m_qsVideoFileFormat);
#if 0
    secondElement->m_fbScaledVideo.in_len = 10*1024*1024;
    *secondElement->m_fbScaledVideo.out_len = 10*1024*1024;
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&secondElement->m_fbScaledVideo));
#endif
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg(bCreateVideoFile?(size_t)&secondElement->m_fbFileTransitionVideo:(size_t)&secondElement->m_fbTransitionVideo));
#endif
    // -y -framerate 1 -i "C:\QtProjects\qtmovie\jpg\img%3d.jpg" myoutput.avi
    //QTime startTime = QTime::currentTime();
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
#if 0
    secondElement->m_fbInputScaledVideo.ptr=secondElement->m_fbScaledVideo.ptr;
    secondElement->m_fbInputScaledVideo.in_len=*secondElement->m_fbScaledVideo.out_len;
    secondElement->m_fbInputScaledVideo.out_len=NULL;
#endif
    if(bCreateVideoFile)
    {
        secondElement->m_fbFileInputTransitionVideo.ptr=secondElement->m_fbFileTransitionVideo.ptr;
        secondElement->m_fbFileInputTransitionVideo.in_len=*secondElement->m_fbFileTransitionVideo.out_len;
        secondElement->m_fbFileInputTransitionVideo.out_len=NULL;
    }
    else
    {
        secondElement->m_fbInputTransitionVideo.ptr=secondElement->m_fbTransitionVideo.ptr;
        secondElement->m_fbInputTransitionVideo.in_len=*secondElement->m_fbTransitionVideo.out_len;
        secondElement->m_fbInputTransitionVideo.out_len=NULL;
    }
    //int dt = startTime.msecsTo(QTime::currentTime());
    //qDebug()<< "ffmpeg waste: " << dt;
    //QMessageBox::information(this, "info", QString(tr("ffmpeg waste: %1")).arg(dt));
    //emit playVideo(vfileName);
    return true;
}
void ElementsEdit::createPanzoomVideo(Element *element, int framerate, const QString& duration, const QString& panzoom, bool bCreateVideoFile/*=false*/)
{

    qDebug()<< "createPanzoomVideo";
    QVector<QString> vqsArgv;
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif
    vqsArgv.push_back("-framerate");
    vqsArgv.push_back(QString(tr("%1")).arg(framerate));
    vqsArgv.push_back("-i");
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg(bCreateVideoFile?(size_t)&element->m_fbFileInputScaleFile:(size_t)&element->m_fbInputScaleFile));
    //-vf "zoompan=z='zoom+0.001':s=512x384" -t 2
    vqsArgv.push_back(QString(tr("-vf")));
    vqsArgv.push_back(QString(tr("%1")).arg(panzoom));
    vqsArgv.push_back(QString(tr("-t")));
    vqsArgv.push_back(QString(tr("%1")).arg(duration));
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(m_qsVideoFileFormat);
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg(bCreateVideoFile?(size_t)&element->m_fbFilePanzoomVideo:(size_t)&element->m_fbPanzoomVideo));
    //vqsArgv.push_back(QString(tr("pic%1.avi")).arg(i));
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    if(bCreateVideoFile)
    {
        element->m_fbFileInputPanzoomVideo.ptr=element->m_fbFilePanzoomVideo.ptr;
        element->m_fbFileInputPanzoomVideo.in_len=*element->m_fbFilePanzoomVideo.out_len;
        element->m_fbFileInputPanzoomVideo.out_len=NULL;
    }
    else
    {
        element->m_fbInputPanzoomVideo.ptr=element->m_fbPanzoomVideo.ptr;
        element->m_fbInputPanzoomVideo.in_len=*element->m_fbPanzoomVideo.out_len;
        element->m_fbInputPanzoomVideo.out_len=NULL;
    }
}
// pan & zoom
//./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img003.jpg -vf "zoompan=z='zoom+0.001':s=512x384"  -t 2 jpg/zoom.512.3.avi;
//./ffmpeg_r.exe -y -i jpg/zoom.512.3.avi -i jpg/zoom.512.2.avi -filter_complex     "blend=all_expr='if(crossfade,1.5,2)'"  jpg/transition_zoom.avi
bool ElementsEdit::createAnimationPanzoom(Element *firstElement, Element *secondElement, bool bCreateVideoFile/*=false*/)
{
    qDebug()<< "createAnimationPanzoom";
    GlobalVideoAttr* globalVideoAttr = secondElement->globalVideoAttr();
    GlobalAnimationAttr* globalAnimationAttr = secondElement->globalAnimationAttr();
    QString duration = QString::number((float)globalVideoAttr->m_iDuration/1000, 'f', 2);
    //float animationDuration = QString::number((float)globalAnimationAttr->m_iTransitionDuration/1000, 'f', 2);

    createPanzoomVideo(firstElement, globalVideoAttr->m_iFramerate, duration, globalAnimationAttr->m_qsPanZoom, bCreateVideoFile);
    createPanzoomVideo(secondElement, globalVideoAttr->m_iFramerate, duration, globalAnimationAttr->m_qsPanZoom, bCreateVideoFile);

    createAnimation(firstElement, secondElement, true, bCreateVideoFile);

    return true;
}
/*
 * End Animation
 * */
/*
 * Start Video
 * */
void ElementsEdit::createFinalVideo(bool bPlay, QByteArray qbAss/*=""*/, const QString& qsFinalVideoFile/*=""*/, bool bCreateVideoFile/*=false*/)
//void ElementsEdit::createFinalVideo(bool bPlay)
{
    m_bCurrentProjectChanged = true;
    if(qbAss.isEmpty() && !m_qbAss.isEmpty())
    {
        qbAss = m_qbAss;
    }
    QVector<QString> vqsArgv;
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif
    //vqsArgv.push_back("-framerate");
    //vqsArgv.push_back("25"); //uncomplete
    struct to_buffer sinbuffer;
    QByteArray sinString;
    for (int i = 0; i < m_flowLayout->count(); ++i)
    {
        Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            qDebug()<< "error uncomplete. ElementsEdit::createFinalVideo elemnet";
            continue;
        }
        if(!element->m_fbInputScaledVideo.ptr)
        {
            // err uncomplete
            qDebug()<< "error uncomplete. ElementsEdit::createFinalVideo m_fbInputScaledVideo";
            continue;
        }
        if(element->m_fbInputTransitionVideo.ptr)
        {
            sinString.append(QString(tr("file buffer:video/avi;nobase64,%1\n")).arg(bCreateVideoFile?(size_t)&element->m_fbFileInputTransitionVideo:(size_t)&element->m_fbInputTransitionVideo));
        }
        else if(element->m_fbInputPanzoomVideo.ptr)
        {
            sinString.append(QString(tr("file buffer:video/avi;nobase64,%1\n")).arg(bCreateVideoFile?(size_t)&element->m_fbFileInputPanzoomVideo:(size_t)&element->m_fbInputPanzoomVideo));
        }
        else
        {
        //snprintf(in_buffer, len, "file buffer:video/avi;nobase64,%zu\n
            sinString.append(QString(tr("file buffer:video/avi;nobase64,%1\n")).arg(bCreateVideoFile?(size_t)&element->m_fbFileInputScaledVideo:(size_t)&element->m_fbInputScaledVideo));
        }
    }
    sinbuffer.ptr = (uint8_t*)sinString.data();
    sinbuffer.in_len = sinString.length();
    sinbuffer.out_len = NULL;
    vqsArgv.push_back("-f");
    vqsArgv.push_back("concat"); //uncomplete
    vqsArgv.push_back(QString(tr("-i")));
    vqsArgv.push_back(QString(tr("buffer:video/avi;nobase64,%1")).arg((size_t)&sinbuffer));
    if(m_globalMusicAttr->m_qsMusicFullFilename.isEmpty() && qbAss.isEmpty())
    {
        vqsArgv.push_back(QString(tr("-c")));
        vqsArgv.push_back(QString(tr("copy")));
    }
    else
    {
        if(!m_globalMusicAttr->m_qsMusicFullFilename.isEmpty())
        {
            //vqsArgv.push_back(QString(tr("-f")));
            //vqsArgv.push_back(QString(tr("%1")).arg(ext));
            if(m_globalMusicAttr->m_iStartTime)
            {
                vqsArgv.push_back(QString(tr("-f")));
                vqsArgv.push_back(QString(tr("lavfi")));
                vqsArgv.push_back(QString(tr("-i")));
                vqsArgv.push_back(QString(tr("anullsrc=channel_layout=5.1:sample_rate=48000")));
                vqsArgv.push_back(QString(tr("-i")));
                vqsArgv.push_back(m_globalMusicAttr->m_qsMusicFullFilename);
                vqsArgv.push_back(QString(tr("-filter_complex")));
                int iRealLength = m_globalMusicAttr->m_iEndPoint - m_globalMusicAttr->m_iStartPoint+m_globalMusicAttr->m_iStartTime;
                /*QMessageBox::information(NULL, "info", QString(tr("vduration: %1 starttime: %2 startpoint: %3 endpoint: %4")).
                  arg(m_iTotalVideoDuration).arg(m_globalMusicAttr->m_iStartTime).
                  arg(m_globalMusicAttr->m_iStartPoint).arg(m_globalMusicAttr->m_iEndPoint));*/
                vqsArgv.push_back(QString(tr("[1:a]atrim=0:%1,asetpts=PTS-STARTPTS[aud1];"
                                "[2:a]atrim=%2:%3,asetpts=PTS-STARTPTS[aud2];"
                                "[aud1][aud2]concat=n=2:v=0:a=1[aout]")).
                        arg((float)m_globalMusicAttr->m_iStartTime/1000).
                        arg((float)m_globalMusicAttr->m_iStartPoint/1000).
                        arg(iRealLength<=m_iTotalVideoDuration?((float)m_globalMusicAttr->m_iEndPoint/1000):((float)(m_iTotalVideoDuration-m_globalMusicAttr->m_iStartTime+m_globalMusicAttr->m_iStartPoint)/1000)));
                vqsArgv.push_back(QString(tr("-map")));
                vqsArgv.push_back(QString(tr("0:v")));
                vqsArgv.push_back(QString(tr("-map")));
                vqsArgv.push_back(QString(tr("[aout]")));
            }
            else
            {
                qDebug()<< "ElementsEdit::createFinalVideo. m_iStartPoint: " << 
                    m_globalMusicAttr->m_iStartPoint<<" m_iEndPoint: "<<m_globalMusicAttr->m_iEndPoint<<
                    " m_iTotalVideoDuration: "<< m_iTotalVideoDuration;
                vqsArgv.push_back(QString(tr("-ss")));
                vqsArgv.push_back(QString(tr("%1")).arg((float)m_globalMusicAttr->m_iStartPoint/1000));
                vqsArgv.push_back(QString(tr("-t")));
                int iRealDuration = m_globalMusicAttr->m_iEndPoint - m_globalMusicAttr->m_iStartPoint;
                vqsArgv.push_back(QString(tr("%1")).arg(iRealDuration<m_iTotalVideoDuration?((float)iRealDuration/1000):(float)(m_iTotalVideoDuration/1000)));
                vqsArgv.push_back(QString(tr("-i")));
                vqsArgv.push_back(m_globalMusicAttr->m_qsMusicFullFilename);
            }
        }

        //./ffmpeg_r.exe -y -i jpg/mp3.512.5.avi -vf ass=jpg/subtitle.ass jpg/subt.mp3.512.5.avi
        if(!qbAss.isEmpty())
        {
            vqsArgv.push_back(QString(tr("-vf")));
            struct to_buffer sintxtbuffer;
            sintxtbuffer.ptr = (uint8_t*)qbAss.data();
            sintxtbuffer.in_len = qbAss.length();
            sintxtbuffer.out_len = NULL;
            vqsArgv.push_back(QString(tr("ass=buffer|%1")).arg((size_t)&sintxtbuffer));
        }
    }
    //printf("video input. stbuf: %p buf.ptr: %p in_len: %zu\n", &sinbuffer, sinbuffer.ptr, sinbuffer.in_len);

    //解决音频可能导致时长加长几十毫秒的问题.
    //http://stackoverflow.com/questions/21420296/how-to-extract-time-accurate-video-segments-with-ffmpeg
    //./ffmpeg_r.exe -y -ss 00:00:10 -t 00:00:03 -i jpg/lovechina1.mp3 -i jpg/loop.512.5.avi jpg/mp3.512.5.avi
    //./ffmpeg_r.exe -y -ss 00:00:10 -t 00:00:03 -i jpg/lovechina1.mp3 -i jpg/loop.512.5.avi -an -shortest jpg/mp3.512.5.avi
    //vqsArgv.push_back(QString(tr("-an")));
    //vqsArgv.push_back(QString(tr("-shortest")));

    if(qsFinalVideoFile.isEmpty())
    {
        uint8_t* out_buffer = m_pOutBuffer;
        m_outlen = m_outMaxLen;

        struct to_buffer soutbuffer;
        soutbuffer.ptr = out_buffer;
        soutbuffer.in_len = m_outMaxLen;
        soutbuffer.out_len = &m_outlen;
        vqsArgv.push_back(QString(tr("-f")));
        vqsArgv.push_back(m_qsVideoFileFormat);
        vqsArgv.push_back(QString(tr("buffer:video/avi;nobase64,%1")).arg((size_t)&soutbuffer));
        //vqsArgv.push_back(QString(tr("mm.avi")));
        //printf("video out. stbuf: %p buf.ptr: %p in_len: %zu out_len: %zu\n", &soutbuffer, soutbuffer.ptr, soutbuffer.in_len, *soutbuffer.out_len);
        int ret;
        if((ret=callFfmpeg(vqsArgv)))
        {
            QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
        }

        qDebug()<<"ElementsEdit::createFinalVideo m_outlen: "<<m_outlen<<" m_imgPlayPosition: "<<m_imgPlayPosition<<" m_outMaxLen: "<<m_outMaxLen;

        setCursor(QCursor(Qt::ArrowCursor));
        //5, 播放视频
        QByteArray tmp=QByteArray((const char*)soutbuffer.ptr, (int)m_outlen);
        emit readyVideo("tmp.avi", tmp, m_imgPlayPosition);
        //emit changePlayPosition(m_imgPlayPosition);
        if(bPlay)
        {
            emit playVideo();
        }
    }
    else
    {
        vqsArgv.push_back(qsFinalVideoFile);
        int ret;
        if((ret=callFfmpeg(vqsArgv)))
        {
            QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
        }
    }
}
#if 0
void ElementsEdit::createFinalVideo(bool bPlay)
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
        if(!element->m_fbInputScaledVideo.ptr)
        {
            // err uncomplete
            continue;
        }
        //snprintf(in_buffer, len, "file buffer:video/avi;nobase64,%zu\n
        sinString.append(QString(tr("file buffer:video/avi;nobase64,%1\n")).arg((size_t)&element->m_fbInputScaledVideo));
    }
    sinbuffer.ptr = (uint8_t*)sinString.data();
    sinbuffer.in_len = sinString.length();
    sinbuffer.out_len = NULL;
    vqsArgv.push_back("-f");
    vqsArgv.push_back("concat"); //uncomplete
    vqsArgv.push_back(QString(tr("-i")));
    vqsArgv.push_back(QString(tr("buffer:video/avi;nobase64,%1")).arg((size_t)&sinbuffer));
    vqsArgv.push_back(QString(tr("-c")));
    vqsArgv.push_back(QString(tr("copy")));
    //printf("video input. stbuf: %p buf.ptr: %p in_len: %zu\n", &sinbuffer, sinbuffer.ptr, sinbuffer.in_len);

    uint8_t* out_buffer = m_pOutBuffer;
    m_outlen = m_outMaxLen;

    struct to_buffer soutbuffer;
    soutbuffer.ptr = out_buffer;
    soutbuffer.in_len = m_outMaxLen;
    soutbuffer.out_len = &m_outlen;
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(m_qsVideoFileFormat);
    vqsArgv.push_back(QString(tr("buffer:video/avi;nobase64,%1")).arg((size_t)&soutbuffer));
    //vqsArgv.push_back(QString(tr("mm.avi")));
    //printf("video out. stbuf: %p buf.ptr: %p in_len: %zu out_len: %zu\n", &soutbuffer, soutbuffer.ptr, soutbuffer.in_len, *soutbuffer.out_len);
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    if(m_globalMusicAttr->m_qsMusicFullFilename.isEmpty() && m_qbAss.isEmpty())
    {
        //5, 播放视频
        QByteArray tmp=QByteArray((const char*)soutbuffer.ptr, (int)m_outlen);
        emit readyVideo("tmp.avi", tmp, m_imgPlayPosition);
        emit changePlayPosition(m_imgPlayPosition);
    }
    else
    {
        createFinalVideoMusicTxt(false);
    }

    if(bPlay)
    {
        emit playVideo();
    }
}
#endif
//每次重新生成视频时都会调用此函数. (生成动画时不能初始化m_vecticalLine的位置)
void ElementsEdit::durationChanged(qint64 duration)
{
    if(duration)
    {
        m_iTotalVideoDuration=duration;
    }
    qDebug()<<"ElementsEdit::durationChanged m_iTotalVideoDuration: "<<m_iTotalVideoDuration;
    initialProgress(); //需要在addImages 图片渲染之后被调用，否则element的长宽不对
}
void ElementsEdit::createSingleVideo(int idxElement, bool bCreateSimpleVideo/*=true*/, bool bCreateVideoFile/*=false*/)
{
    if(idxElement<0)
    {
        //error uncomplete 
        return;
    }
    Element *secondElement = qobject_cast<Element *>(m_flowLayout->itemAt(idxElement)->widget());
    if(!secondElement)
    {
        //error uncomplete 
        return;
    }

    //transition, transition&zoom
    GlobalAnimationAttr * globalAnimationAttr = secondElement->globalAnimationAttr();
    GlobalVideoAttr* globalVideoAttr = secondElement->globalVideoAttr();
    QString duration = QString::number((float)globalVideoAttr->m_iDuration/1000, 'f', 2);
    if(idxElement >= 1 && globalAnimationAttr && !globalAnimationAttr->m_qsTransitionName.isEmpty())
    {
        Element *firstElement = qobject_cast<Element *>(m_flowLayout->itemAt(idxElement-1)->widget());
        if(firstElement)
        {
            if(globalAnimationAttr->m_qsPanZoom.isEmpty())
            {
                //./ffmpeg_r.exe -y -i jpg/512.3.avi -i jpg/512.2.avi -filter_complex     "blend=all_expr='if(crossfade,1.5,2)'"  jpg/transition.avi
                //createPanzoomVideo(secondElement,globalVideoAttr->m_iFramerate, duration,globalAnimationAttr->m_qsPanZoom);
                createAnimation(firstElement, secondElement, false, bCreateVideoFile);
            }
            else
            {
            // pan & zoom
                //./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img003.jpg -vf "zoompan=z='zoom+0.001':s=512x384"  -t 2 jpg/zoom.512.3.avi;
                //./ffmpeg_r.exe -y -i jpg/zoom.512.3.avi -i jpg/zoom.512.2.avi -filter_complex     "blend=all_expr='if(crossfade,1.5,2)'"  jpg/transition_zoom.avi
                createAnimationPanzoom(firstElement, secondElement, bCreateVideoFile);
            }
        }
        else
        {
            //error uncomplete 
            return;
        }
    }
    else if(!globalAnimationAttr->m_qsPanZoom.isEmpty())
    {
        //./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img003.jpg -vf "zoompan=z='zoom+0.001':s=512x384"  -t 2 jpg/zoom.512.3.avi;
        createPanzoomVideo(secondElement, globalVideoAttr->m_iFramerate, duration, globalAnimationAttr->m_qsPanZoom, bCreateVideoFile);
    }
    else if(bCreateSimpleVideo)
    {
        //./ffmpeg_r.exe -y  -framerate 24 -loop 1 -t 2 -i jpg/512img003.jpg -f avi jpg/loop.512.3.avi;
        createSimpleVideo(secondElement, bCreateVideoFile);
    }
}
void ElementsEdit::createSimpleVideo(Element *element, bool bCreateVideoFile /*=false*/)
{
    GlobalVideoAttr* globalVideoAttr = element->globalVideoAttr();
    QString duration = QString::number((float)globalVideoAttr->m_iDuration/1000, 'f', 2);
    qDebug()<<"ElementsEdit::createSimpleVideo globalVideoAttr->m_iDuration: "<<globalVideoAttr->m_iDuration;

    QVector<QString> vqsArgv;
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif
    vqsArgv.push_back("-framerate");
    vqsArgv.push_back(QString(tr("%1")).arg(globalVideoAttr->m_iFramerate));
    vqsArgv.push_back("-loop");
    vqsArgv.push_back("1");
    vqsArgv.push_back("-t");
    vqsArgv.push_back(QString(tr("%1")).arg(duration));
    vqsArgv.push_back("-i");
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg(bCreateVideoFile?(size_t)&element->m_fbFileInputScaleFile:(size_t)&element->m_fbInputScaleFile));//uncomplete
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(m_qsVideoFileFormat);
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg(bCreateVideoFile?(size_t)&element->m_fbFileScaledVideo:(size_t)&element->m_fbScaledVideo));
    //vqsArgv.push_back(QString(tr("pic%1.avi")).arg(i));
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    if(bCreateVideoFile)
    {
        element->m_fbFileInputScaledVideo.ptr=element->m_fbFileScaledVideo.ptr;
        element->m_fbFileInputScaledVideo.in_len=*element->m_fbFileScaledVideo.out_len;
        element->m_fbFileInputScaledVideo.out_len=NULL;
    }
    else
    {
        element->m_fbInputScaledVideo.ptr=element->m_fbScaledVideo.ptr;
        element->m_fbInputScaledVideo.in_len=*element->m_fbScaledVideo.out_len;
        element->m_fbInputScaledVideo.out_len=NULL;
    }
}
void ElementsEdit::videoStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        //hide videotext
        for (int i = 0; i < m_flowLayout->count(); ++i)
        {
            Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
            if (!element)
            {
                // err uncomplete
                continue;
            }
            emit displayVideoTextSignal(element, false);
        }
        break;
    case QMediaPlayer::StoppedState:
    default:
        break;
    }

}
void ElementsEdit::saveVideo()
{
    //1, open write file
    QFileDialog fileDialog(this, tr("Save as..."));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
#if 0
    QStringList mimeTypes;
    mimeTypes << "application/vnd.oasis.opendocument.text" << "text/html" << "text/plain";
    fileDialog.setMimeTypeFilters(mimeTypes);
#endif
    QStringList filters;
    filters << "MPEG-4/H.264 Video file (*.mp4)"
        << "Windows Media Video file (*.wmv)"
        << "Audio Video Interleaved Video file (*.avi)"
        ;
    fileDialog.setNameFilters(filters);

    fileDialog.setDefaultSuffix("mp4");
    if (fileDialog.exec() != QDialog::Accepted)
    {
        //error uncomplete
        return ;
    }
    const QString fn = fileDialog.selectedFiles().first();
    QFileInfo fi(fn);
#if 0
    //设置视频格式
    m_qsVideoFileFormat = fi.suffix();  
    if(m_qsVideoFileFormat=="wmv")
    {
        m_qsVideoFileFormat="wmv2";
    }
#endif
    // 2, 显示进度
    ProgressDialog *progressDialog = new ProgressDialog(fi.fileName() , this);
    progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    //connect(progressDialog, &QProgressDialog::canceled, this, &HttpWindow::cancelDownload);
    connect(this, &ElementsEdit::saveMoiveProgress, progressDialog, &ProgressDialog::saveMoiveProgress);
    connect(this, &ElementsEdit::saveMoiveFinish, progressDialog, &ProgressDialog::hide);
    //progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->show();
    //progressDialog->exec();

    // 2, create video
#define B_CREATE_FINAL_VIDEO_FILE true 
    for (int i = 0; i < m_flowLayout->count(); ++i)
    {
        Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            qDebug()<< "error uncomplete. ElementsEdit::createFinalVideo elemnet";
            continue;
        }
        qApp->processEvents();
        //2.1 single video
        element->initialMemoryForVideoFile();

        scaleImage(element, m_qFinalVideoSize, B_CREATE_FINAL_VIDEO_FILE);

        createSimpleVideo(element, B_CREATE_FINAL_VIDEO_FILE);
#define NO_CREATE_SIMPLE_VIDEO false
        createSingleVideo(i, NO_CREATE_SIMPLE_VIDEO, B_CREATE_FINAL_VIDEO_FILE);
        emit saveMoiveProgress(i+1, m_flowLayout->count()+1);
    }
    //2.2 final video
    QByteArray qbAss = m_globalContext->m_scene->createTotalAssInfo(m_qFinalVideoSize).toUtf8();
    createFinalVideo(false, qbAss, fn, B_CREATE_FINAL_VIDEO_FILE);
    emit saveMoiveProgress(m_flowLayout->count()+1, m_flowLayout->count()+1);
    //3, clean memory
    for (int i = 0; i < m_flowLayout->count(); ++i)
    {
        Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            qDebug()<< "error uncomplete. ElementsEdit::createFinalVideo elemnet";
            continue;
        }
        //clean scaledfile, scaledvideo, panzoomvideo, transitionvideo
        element->freeMemoryForVideoFile();
    }
    emit saveMoiveFinish();
    progressDialog->deleteLater();
#if 0
    //4, 还原视频格式
    m_qsVideoFileFormat = VIDEO_FILE_FORMAT;
#endif
}
void ElementsEdit::publishVideo()
{
}
/*
 * End Video
 * */
//当前element的属性变化，包括：
//动画
//视屏参数：时长
//文字
void ElementsEdit::elementAttrChanged(int attrType, bool bPlay)
{
    //0
    if(attrType & ATTR_ROTATE)
    {
        scaleImage(currentElement());
        createSimpleVideo(currentElement());
        createSingleVideo(m_idxCurrentElement, false);
    }
    //1, 生成单独的视频
    if(attrType & ATTR_VIDEO)
    {
        createSingleVideo(m_idxCurrentElement);
    }

    //2, 
    if(attrType & ATTR_MUSIC)
    {
        musicAttrChanged();
    }

    //3, 合成声音、文字
    createFinalVideo(bPlay);
}
#if 0
void ElementsEdit::createFinalVideoMusicTxt(bool bPlay)
{
    QVector<QString> vqsArgv;
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif
    vqsArgv.push_back("-i");
    struct to_buffer sinbuffer;
    sinbuffer.ptr = (uint8_t*)m_pOutBuffer;
    sinbuffer.in_len = m_outlen;
    sinbuffer.out_len = NULL;
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&sinbuffer));

    if(!m_globalMusicAttr->m_qsMusicFullFilename.isEmpty())
    {
        //vqsArgv.push_back(QString(tr("-f")));
        //vqsArgv.push_back(QString(tr("%1")).arg(ext));
        if(m_globalMusicAttr->m_iStartTime)
        {
            vqsArgv.push_back(QString(tr("-f")));
            vqsArgv.push_back(QString(tr("lavfi")));
            vqsArgv.push_back(QString(tr("-i")));
            vqsArgv.push_back(QString(tr("anullsrc=channel_layout=5.1:sample_rate=48000")));
            vqsArgv.push_back(QString(tr("-i")));
            vqsArgv.push_back(m_globalMusicAttr->m_qsMusicFullFilename);
            vqsArgv.push_back(QString(tr("-filter_complex")));
            int iRealLength = m_globalMusicAttr->m_iEndPoint - m_globalMusicAttr->m_iStartPoint+m_globalMusicAttr->m_iStartTime;
            /*QMessageBox::information(NULL, "info", QString(tr("vduration: %1 starttime: %2 startpoint: %3 endpoint: %4")).
                    arg(m_iTotalVideoDuration).arg(m_globalMusicAttr->m_iStartTime).
                    arg(m_globalMusicAttr->m_iStartPoint).arg(m_globalMusicAttr->m_iEndPoint));*/
            vqsArgv.push_back(QString(tr("[1:a]atrim=0:%1,asetpts=PTS-STARTPTS[aud1];"
                            "[2:a]atrim=%2:%3,asetpts=PTS-STARTPTS[aud2];"
                            "[aud1][aud2]concat=n=2:v=0:a=1[aout]")).
                    arg((float)m_globalMusicAttr->m_iStartTime/1000).
                    arg((float)m_globalMusicAttr->m_iStartPoint/1000).
                    arg(iRealLength<=m_iTotalVideoDuration?((float)m_globalMusicAttr->m_iEndPoint/1000):((float)(m_iTotalVideoDuration-m_globalMusicAttr->m_iStartTime+m_globalMusicAttr->m_iStartPoint)/1000)));
            vqsArgv.push_back(QString(tr("-map")));
            vqsArgv.push_back(QString(tr("0:v")));
            vqsArgv.push_back(QString(tr("-map")));
            vqsArgv.push_back(QString(tr("[aout]")));
        }
        else
        {
            vqsArgv.push_back(QString(tr("-ss")));
            vqsArgv.push_back(QString(tr("%1")).arg((float)m_globalMusicAttr->m_iStartPoint/1000));
            vqsArgv.push_back(QString(tr("-t")));
            int iRealDuration = m_globalMusicAttr->m_iEndPoint - m_globalMusicAttr->m_iStartPoint;
            vqsArgv.push_back(QString(tr("%1")).arg(iRealDuration<m_iTotalVideoDuration?((float)iRealDuration/1000):(float)(m_iTotalVideoDuration/1000)));
            vqsArgv.push_back(QString(tr("-i")));
            vqsArgv.push_back(m_globalMusicAttr->m_qsMusicFullFilename);
        }
    }

    //./ffmpeg_r.exe -y -i jpg/mp3.512.5.avi -vf ass=jpg/subtitle.ass jpg/subt.mp3.512.5.avi
    if(!m_qbAss.isEmpty())
    {
        vqsArgv.push_back(QString(tr("-vf")));
        struct to_buffer sintxtbuffer;
        sintxtbuffer.ptr = (uint8_t*)m_qbAss.data();
        sintxtbuffer.in_len = m_qbAss.length();
        sintxtbuffer.out_len = NULL;
        vqsArgv.push_back(QString(tr("ass=buffer|%1")).arg((size_t)&sintxtbuffer));
    }
#if 1
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(m_qsVideoFileFormat);
    struct to_buffer soutbuffer;
    soutbuffer.ptr = m_pTextVideoOutBuffer;
    soutbuffer.in_len = m_textVideoMaxOutLen;
    soutbuffer.out_len = &m_textVideoOutLen;
    vqsArgv.push_back(QString(tr("buffer:video/avi;nobase64,%1")).arg((size_t)&soutbuffer));
#else
    vqsArgv.push_back(QString(tr("txt.avi")));
#endif
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    setCursor(QCursor(Qt::ArrowCursor));
    //5, 播放视频
    QByteArray tmp=QByteArray((const char*)soutbuffer.ptr, (int)*soutbuffer.out_len);
    emit readyVideo("tmp.avi", tmp, m_imgPlayPosition);
    emit changePlayPosition(m_imgPlayPosition);

    if(bPlay)
    {
        //emit playVideo();
    }
}
#endif
void ElementsEdit::mousePressEvent(QMouseEvent *event)
{
    if(m_isFirstClick)
    {
        addImages();
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
    addImages();
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
//视频播放时被调用，m_vecticalLine也需要跟着变化
void ElementsEdit::positionChanged(qint64 position)
{
    m_playPosition=position;
    emit updatedVideoTimeTextSignal(position, m_iTotalVideoDuration);
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
    if(!m_imgPlayPosition && m_flowLayout && m_flowLayout->count()>0 )
    //for (int i = 0; i < m_flowLayout->count(); ++i)
    {
        //获取每个element的(x, y, width, height)
        //Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(0)->widget());
        //const QRect &rect=element->geometry();
        //QMessageBox::information(this, "info", QString(tr("initail m_vecticalLine")));
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
#if 0
    if(m_iTotalVideoDuration && m_signalImgWidth)
    {
        int xMax=(geometry().width()/m_signalImgWidth)*m_signalImgWidth;
        //QMessageBox::information(this, "info", QString(tr("xMax: %1 width: %2 signalwidth: %3")).arg(xMax).arg(geometry().width()).arg(m_signalImgWidth));
        int xOri=m_playPosition*m_imgWidth/m_iTotalVideoDuration;
        int x=xOri%xMax;
        int y=(xOri/xMax)*m_imgHeight;
        //if(!x)
        //    QMessageBox::information(this, "info", QString(tr("xMax: %1 xOri: %2 x: %3 y: %4 posi: %5 duration: %6 m_imgWidth: %7")).arg(xMax).arg(xOri).arg(x).arg(y).arg(m_playPosition).arg(m_iTotalVideoDuration).arg(m_imgWidth));
            //QMessageBox::information(this, "info", QString(tr("assign m_vecticalLine")));
        m_vecticalLine->setGeometry(QRect(x, y, m_lineWidth, m_imgHeight)); //uncomplete
    }
#endif
    if(m_iTotalVideoDuration && m_signalImgWidth)
    {
        int xMaxImgCount = geometry().width()/m_signalImgWidth; 
        int iStartDuration=0;
        int xImgIdx=0, yImgIdx=0;
        for (int i = 0; m_flowLayout && i < m_flowLayout->count(); ++i)
        {
            Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
            if (!element)
            {
                // err uncomplete
                qDebug()<< "error uncomplete. element";
                continue;
            }
            int iCurrDuration = element->globalVideoAttr()->m_iDuration;
            if(iCurrDuration && iStartDuration + iCurrDuration >= m_playPosition)
            {
                int x=m_signalImgWidth*xImgIdx + m_signalImgWidth*(m_playPosition - iStartDuration)/iCurrDuration;
                int y=yImgIdx*m_imgHeight;
                m_vecticalLine->setGeometry(QRect(x, y, m_lineWidth, m_imgHeight)); //uncomplete
                break;
            }
            if(!iCurrDuration)
            {
                //error uncomplete
                qDebug()<< "error uncomplete. iCurrDuration: "<<iCurrDuration<<" m_iDuration: "<<element->globalVideoAttr()->m_iDuration;
            }
            //m_playPosition
            iStartDuration += iCurrDuration;
            if((++xImgIdx)>=xMaxImgCount)
            {
                xImgIdx = 0;
                yImgIdx++;
            }
        }
    }
}
void ElementsEdit::initialFirstLayout()
{
    //1, elements layout
    setCursor(QCursor(Qt::PointingHandCursor));
    m_vecticalLine->hide();
    m_isFirstClick = true;
    if(m_flowLayout)
    {
        delete m_flowLayout;
        m_flowLayout = NULL;
    }
    if(m_firstLayout)
    {
        delete m_firstLayout;
    }
    if(m_firstLabel)
    {
        delete m_firstLabel;
    }
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

    //2, video layout
    if(m_globalContext && m_globalContext->m_player)
    {
        m_globalContext->m_player->initial();
    }

    //3, tab layout
    if(m_globalContext && m_globalContext->m_tabWidget)
    {
        //uncomplete
    }
}
#define COMFIRM_SAVE
QMessageBox::StandardButton ElementsEdit::confirmSaveProject()
{
    if(!m_bCurrentProjectChanged)
    {
        return QMessageBox::No;
    }
    QFileInfo fi(m_qsProjectFile);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr(PROGRAM_TITLE),
            QString(tr("Do you want to save changes to %1")).arg(fi.fileName()),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes)
    {
        saveProject();
    }
    else if (reply == QMessageBox::No)
    {
    }
    else
    {
    }
    return reply;
}
void ElementsEdit::newProject()
{
    //1, save current project
    if(!m_qsProjectFile.isEmpty())
    {
        // whether save ?
        if(QMessageBox::Cancel == confirmSaveProject())
        {
            return;
        }
    }
    //2, new project
    m_qsProjectFile="";

    cleanProject();

    initialFirstLayout();
}
void ElementsEdit::cleanProject()
{
    createFlowLayout();
    //4, delete current element
    for (int i = m_flowLayout->count()-1; i>=0; --i)
    {
        QWidget *element = qobject_cast<QWidget *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            qDebug()<< "error uncomplete. ElementsEdit::selectedImage";
            continue;
        }
        //(qobject_cast<Element *>element)->setValid(false); 
        m_flowLayout->takeAt(i);
        element->deleteLater();
    }
    m_globalMusicAttr->initialMusicAttr();
}
void ElementsEdit::openProject()
{
    //1, whether changed?
    //if(m_bCurrentProjectChanged && !m_qsProjectFile.isEmpty())
    if(!m_qsProjectFile.isEmpty())
    {
        // whether save ?
        if(QMessageBox::Cancel == confirmSaveProject())
        {
            return;
        }
    }

    //2, open project
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
            tr("Film Maker Project (*.fmp)"));
    if (fileName.isEmpty() || fileName.compare(m_qsProjectFile)==0)
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, "error", tr("can't read file: %1").arg(fileName));
        return;
    }
    cleanProject();
#if 0
    //3, 
    createFlowLayout();
    //4, delete current element
    for (int i = m_flowLayout->count()-1; i>=0; --i)
    {
        QWidget *element = qobject_cast<QWidget *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            qDebug()<< "error uncomplete. ElementsEdit::selectedImage";
            continue;
        }
        //(qobject_cast<Element *>element)->setValid(false); 
        m_flowLayout->takeAt(i);
        element->deleteLater();
    }
    //initialFirstLayout();
#endif
    //5, read xml && new Element: /c/QtProjects/QtExamples/widgets/animation/sub-attaq/graphicsscene.cpp
    QTextStream errorStream(stderr);
    QXmlStreamReader reader(&file);
    Element *element = NULL;
    m_iTotalVideoDuration = 0;
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.tokenType() == QXmlStreamReader::StartElement 
                && reader.name() == "ImageItem") 
        {
            qDebug()<< "ImageItem id: " << reader.attributes().value("id").toString().toInt();
            qDebug()<< "ImageItem filePath: " << reader.attributes().value("filePath").toString();
            if(!reader.attributes().hasAttribute("filePath"))
            {
                //error uncomplete
                continue;
            }
            GlobalImageAttr  newGlobalImageAttr;
            newGlobalImageAttr.m_qsImageName = reader.attributes().value("filePath").toString();
            newGlobalImageAttr.m_iRotateLeft = reader.attributes().value("rotateLeft").toString().toInt();
            newGlobalImageAttr.m_iRotateRight = reader.attributes().value("rotateRight").toString().toInt();
            int idx=m_flowLayout->count();
            if(!(element = initialImage(newGlobalImageAttr, idx)))
            {
                continue;
            }
            GlobalImageAttr * globalImageAttr = element->image()->globalImageAttr();
            GlobalTextAttr * globalTextAttr = element->globalTextAttr();
            GlobalAnimationAttr * globalAnimationAttr = element->globalAnimationAttr();
            GlobalVideoAttr * globalVideoAttr = element->globalVideoAttr();

            // uncomplete rotate
            while (!reader.atEnd()) {
                reader.readNext();
                qDebug()<< "ImageItem element: " << reader.name();
                if (reader.tokenType() == QXmlStreamReader::StartElement
                        && reader.name() == "TitleClip") 
                {
                    while (!reader.atEnd()) {
                        reader.readNext();
                        qDebug()<< "TitleClip element: " << reader.name();
                        if (reader.tokenType() == QXmlStreamReader::StartElement
                                && reader.name() == "BoundProperties") 
                        {
                            while (!reader.atEnd()) {
                                reader.readNext();
                                qDebug()<< "BoundProperties element: " << reader.name();
                                if (reader.tokenType() == QXmlStreamReader::StartElement
                                        && ( reader.name() == "BoundPropertyString"
                                            || reader.name() == "BoundPropertyInt"
                                            || reader.name() == "BoundPropertyFloat"
                                            ) )
                                {
                                    QString qsName=reader.attributes().value("Name").toString();
                                    QString qsValue=reader.attributes().value("Value").toString();
                                    qDebug()<< qsName<<" "<<qsValue;
                                    if(qsName.compare("family")==0)
                                    {
                                        globalTextAttr->m_qfont.setFamily(qsValue);
                                    }
                                    else if(qsName.compare("size")==0)
                                    {
                                        globalTextAttr->m_qfont.setPointSize(qsValue.toInt());
                                    }
                                    else if(qsName.compare("style")==0)
                                    {
                                        if(qsValue.compare("Bold")==0)
                                            globalTextAttr->m_qfont.setWeight(QFont::Bold);
                                        else if(qsValue.compare("Italic")==0)
                                            globalTextAttr->m_qfont.setItalic(true);
                                        else if(qsValue.compare("Underline")==0)
                                            globalTextAttr->m_qfont.setUnderline(true);
                                    }
                                    if(qsName.compare("color")==0)
                                    {
                                        globalTextAttr->m_fontColor=QColor(qsValue);
                                    }
                                    if(qsName.compare("alignment")==0)
                                    {
                                        globalTextAttr->m_textAlign = qsValue.compare("left")==0?Qt::AlignLeft:(qsValue.compare("center")==0?Qt::AlignHCenter:Qt::AlignRight);
                                    }
                                    if(qsName.compare("startTime")==0)
                                    {
                                        globalTextAttr->m_iStartTimeText = qsValue.toInt();
                                    }
                                    if(qsName.compare("duration")==0)
                                    {
                                        globalTextAttr->m_iDurationText = qsValue.toInt();
                                    }
                                    if(qsName.compare("effectIndex")==0)
                                    {
                                        globalTextAttr->m_idxEffects = qsValue.toInt();
                                    }
                                    if(qsName.compare("pos_x")==0)
                                    {
                                        globalTextAttr->m_pfPos.setX(qsValue.toFloat());
                                    }
                                    if(qsName.compare("pos_y")==0)
                                    {
                                        globalTextAttr->m_pfPos.setY(qsValue.toFloat());
                                    }
                                    if(qsName.compare("text")==0)
                                    {
                                        element->lineEdit()->updateTextByVideo(qsValue);
                                        //globalTextAttr->m_pfPos.setY(qsValue.toFloat());
                                    }
                                }
                                else if (reader.tokenType() == QXmlStreamReader::EndElement
                                        && reader.name() == "BoundProperties") 
                                {
                                    QString qsVideoText = element->lineEdit()->videoText();
                                    emit activeVideoTextSignal(element, qsVideoText);
                                    break;
                                }
                            }
                        }
                        else if (reader.tokenType() == QXmlStreamReader::StartElement
                                && reader.name() == "Transitions") 
                        {
                        }
                        else if (reader.tokenType() == QXmlStreamReader::EndElement
                                && reader.name() == "TitleClip") 
                        {
                            break;
                        }
                    }
                }
                else if (reader.tokenType() == QXmlStreamReader::StartElement
                        && reader.name() == "Animation") 
                {
                    while (!reader.atEnd()) {
                        reader.readNext();
                        //qDebug()<< "TitleClip element: " << reader.name();
                        if (reader.tokenType() == QXmlStreamReader::StartElement
                                && reader.name() == "Transitions") 
                        {
                            globalAnimationAttr->m_qsTransitionName = 
                                reader.attributes().value("effectTemplateID").toString();
                            globalAnimationAttr->m_iTransitionDuration = 
                                reader.attributes().value("duration").toString().toInt();
                        }
                        else if (reader.tokenType() == QXmlStreamReader::StartElement
                                && reader.name() == "PanAndZoomShapeEffect") 
                        {
                            globalAnimationAttr->m_qsPanZoom = 
                                reader.attributes().value("effectTemplateID").toString();
                        }
                        else if (reader.tokenType() == QXmlStreamReader::EndElement
                                && reader.name() == "Animation") 
                        {
                            createSingleVideo(m_flowLayout->count()-1, false);
                            break;
                        }
                    }
                }
                else if (reader.tokenType() == QXmlStreamReader::StartElement
                        && reader.name() == "VideoClip") 
                {
                    globalVideoAttr->m_iDuration = 
                        reader.attributes().value("duration").toString().toInt();
                }
                else if (reader.tokenType() == QXmlStreamReader::EndElement
                        && reader.name() == "ImageItem") 
                {
                    element = NULL;
                    break;
                }
            }
        }
        else if (reader.tokenType() == QXmlStreamReader::StartElement 
                && reader.name() == "AudioItem") 
        {
            m_globalMusicAttr->m_qsMusicFullFilename = reader.attributes().value("filePath").toString();
            m_globalMusicAttr->m_iStartTime = reader.attributes().value("startTime").toString().toInt();
            m_globalMusicAttr->m_iStartPoint = reader.attributes().value("startPoint").toString().toInt();
            m_globalMusicAttr->m_iEndPoint = reader.attributes().value("endPoint").toString().toInt();
            m_globalMusicAttr->m_iMusicDuration = reader.attributes().value("duration").toString().toInt();
        }
    }
    file.close();
    //4, 变更text的开始时间, 重新生成ass (前面增加一个图片、或者修改前面某个视频时长时)
    updateTextAttrAndAss(0);
    //5, 生成总视频
    createFinalVideo(false);
    //5, 显示music文件名和颜色
    musicAttrChanged();
    //8, 第一次选择
    if(!m_lastSelectedElement)
    {
        m_idxCurrentElement = 0;
        m_lastSelectedElement = qobject_cast<QWidget *>(m_flowLayout->itemAt(m_idxCurrentElement)->widget());
        (qobject_cast<Element *>(m_lastSelectedElement))->doFocusImage();
        emit displayVideoTextSignal(qobject_cast<Element *>(m_lastSelectedElement), true);
    }


    m_vecticalLine->raise(); // top level, Raises this widget to the top of the parent widget's stack.

    m_bCurrentProjectChanged = false;

    m_qsProjectFile = fileName;
}
void ElementsEdit::saveProject()
{
    //1, 
    QString qsFile;
    if(m_qsProjectFile.isEmpty())
    {
        //open save file
        //m_qsProjectFile = ;
        QFileDialog fileDialog(this, tr("Open Project"));
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        QStringList filters;
        filters << "Film Maker Project (*.fmp)"
            << "All File (*.*)"
            ;
        fileDialog.setNameFilters(filters);
        if (fileDialog.exec() != QDialog::Accepted)
        {
            //error uncomplete
            return ;
        }
        m_qsProjectFile = fileDialog.selectedFiles().first();
    }

    //2, write xml
    QFile file(m_qsProjectFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::information(this, "error", tr("can't write file: %1").arg(m_qsProjectFile));
        return;
    }
    //QString qsProject;
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("Project");
    stream.writeAttribute("name", "wmm_project");
    stream.writeAttribute("version", "1");
    for (int i = 0, id = 1; i < m_flowLayout->count(); ++i)
    {
        Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            qDebug()<< "error uncomplete. ElementsEdit::selectedImage";
            continue;
        }

#define WRITE_XML_ELEMENT(type, name, value) do{ \
        stream.writeStartElement("BoundProperty"#type); \
        stream.writeAttribute("Name", name); \
        stream.writeAttribute("Value", value); \
        stream.writeEndElement();  \
    } \
    while(0)
        GlobalImageAttr * globalImageAttr = element->image()->globalImageAttr();
        GlobalTextAttr * globalTextAttr = element->globalTextAttr();
        GlobalAnimationAttr * globalAnimationAttr = element->globalAnimationAttr();
        GlobalVideoAttr * globalVideoAttr = element->globalVideoAttr();
        QString qsVideoText = element->lineEdit()->videoText();

        stream.writeStartElement("ImageItem");
        stream.writeAttribute("id", QString::number(id));
        stream.writeAttribute("filePath", globalImageAttr->m_qsImageName);
        stream.writeAttribute("rotateLeft", QString::number(globalImageAttr->m_iRotateLeft));
        stream.writeAttribute("rotateRight", QString::number(globalImageAttr->m_iRotateRight));
        {
            stream.writeStartElement("TitleClip");
            //if(!qsVideoText.isEmpty())
            {
                stream.writeStartElement("BoundProperties");
                {
                    WRITE_XML_ELEMENT(String, "family", globalTextAttr->m_qfont.family());
#if 0
                    stream.writeStartElement("BoundPropertyString");
                    stream.writeAttribute("name", "family");
                    stream.writeAttribute("Value", globalTextAttr->m_qfont.family());
                    stream.writeEndElement(); 
#endif
                    WRITE_XML_ELEMENT(String, "size", QString::number(globalTextAttr->m_qfont.pointSize()));
                    if(globalTextAttr->m_qfont.weight()==QFont::Bold)
                    {
                        WRITE_XML_ELEMENT(String, "style", "Bold");
                    }
                    if(globalTextAttr->m_qfont.italic())
                    {
                        WRITE_XML_ELEMENT(String, "style", "Italic");
                    }
                    if(globalTextAttr->m_qfont.underline())
                    {
                        WRITE_XML_ELEMENT(String, "style", "Underline");
                    }
                    //WRITE_XML_ELEMENT(String, "color", ); //uncomplete
                    WRITE_XML_ELEMENT(String, "alignment", globalTextAttr->m_textAlign == Qt::AlignLeft?"left":(globalTextAttr->m_textAlign == Qt::AlignHCenter?"center":"right")); //uncomplete
                    WRITE_XML_ELEMENT(Int, "startTime", QString::number(globalTextAttr->m_iStartTimeText));
                    WRITE_XML_ELEMENT(Int, "duration", QString::number(globalTextAttr->m_iDurationText));
                    WRITE_XML_ELEMENT(Int, "effectIndex", QString::number(globalTextAttr->m_idxEffects));
                    WRITE_XML_ELEMENT(Float, "pos_x", QString::number(globalTextAttr->m_pfPos.x()));
                    WRITE_XML_ELEMENT(Float, "pos_y", QString::number(globalTextAttr->m_pfPos.y()));
                    WRITE_XML_ELEMENT(String, "text", qsVideoText); //uncomple
#if 0
                    stream.writeStartElement("BoundPropertyStringSet");
                    stream.writeAttribute("Name", "text");
                    WRITE_XML_ELEMENT(String, "Value", qsVideoText); //uncomple \n
                    stream.writeEndElement(); 
#endif
                }
                stream.writeEndElement(); // BoundProperties
            }
            {
                stream.writeStartElement("Transitions");
                stream.writeEndElement(); // Transitions 
            }
            stream.writeEndElement(); // TitleClip 
        }
        {
            stream.writeStartElement("Animation");
            if(!globalAnimationAttr->m_qsTransitionName.isEmpty())
            {
                stream.writeStartElement("Transitions");
                stream.writeAttribute("effectTemplateID", globalAnimationAttr->m_qsTransitionName);
                stream.writeAttribute("duration", QString::number(globalAnimationAttr->m_iTransitionDuration));
                stream.writeEndElement(); // Transitions 
            }
            if(!globalAnimationAttr->m_qsPanZoom.isEmpty())
            {
                stream.writeStartElement("PanAndZoomShapeEffect");
                stream.writeAttribute("effectTemplateID", globalAnimationAttr->m_qsPanZoom);
                stream.writeEndElement(); // PanAndZoomShapeEffect
            }
            stream.writeEndElement(); // Animation
        }
        {
            stream.writeStartElement("VideoClip");
            stream.writeAttribute("duration", QString::number(globalVideoAttr->m_iDuration));
            stream.writeEndElement(); // VideoClip
        }
        stream.writeEndElement(); // ImageItem 

        id ++;
    }
    {
        stream.writeStartElement("AudioItem");
        stream.writeAttribute("id", "1");
        stream.writeAttribute("filePath", m_globalMusicAttr->m_qsMusicFullFilename);
        stream.writeAttribute("startTime", QString::number(m_globalMusicAttr->m_iStartTime));
        stream.writeAttribute("startPoint", QString::number(m_globalMusicAttr->m_iStartPoint));
        stream.writeAttribute("endPoint", QString::number(m_globalMusicAttr->m_iEndPoint));
        stream.writeAttribute("duration", QString::number(m_globalMusicAttr->m_iMusicDuration));
        stream.writeEndElement(); // AudioItem
    }

    stream.writeEndElement(); // Project 

    stream.writeEndDocument();
    file.close();

    m_bCurrentProjectChanged = false;
}
void ElementsEdit::createFlowLayout()
{
    if(m_isFirstClick)
    {
        m_isFirstClick=false;

        //qDeleteAll(children());
        delete m_firstLayout;
        m_firstLayout = NULL;
        delete m_firstLabel;
        m_firstLabel = NULL;

        m_flowLayout = new FlowLayout(this);
        setLayout(m_flowLayout);
    }
}
ProgressDialog::ProgressDialog(const QString &qsFileName, QWidget *parent)
  : QProgressDialog(parent)
{
    setWindowTitle(tr(PROGRAM_TITLE));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setLabelText(tr("Saving Movie %1.").arg(qsFileName));
    setMinimum(0);
    setValue(0);
    setMinimumDuration(0);
}

void ProgressDialog::saveMoiveProgress(qint64 completed, qint64 total)
{
    qDebug()<< "saveMoiveProgress.  completed: " << completed<< " total: "<< total;
    setMaximum(total);
    setValue(completed);
}
