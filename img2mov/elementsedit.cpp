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
ElementsEdit::ElementsEdit(QWidget *parent, GlobalContext* globalContext)
    : QWidget(parent)
      //, m_flowLayout(new FlowLayout(this))
      , m_flowLayout(NULL)
      , m_vecticalLine(new QFrame(this))
      , m_lastSelectedElement(0)
      , m_idxCurrentElement(-1)
      , m_isFirstClick(true)
      , m_globalMusicAttr(new GlobalMusicAttr)
{   
    m_globalContext = globalContext;
    //m_tmpdir=QDir::currentPath().append(tr("/tmp"));

    setBackgroundRole(QPalette::Light);
    setCursor(QCursor(Qt::PointingHandCursor));

    m_duration=0;
    m_imgWidth=0;
    m_signalImgWidth=0;
    m_imgHeight=0;
    m_lineWidth=10;
    m_playPosition=0;

    m_imgPlayPosition = 0;

    m_outlen = m_outMaxLen = 10*1024*1024;
	m_pOutBuffer=(uint8_t *)malloc(m_outMaxLen);

    m_textVideoOutLen = m_textVideoMaxOutLen = 10*1024*1024;
	m_pTextVideoOutBuffer=(uint8_t *)malloc(m_textVideoMaxOutLen);
#if 1

    m_vecticalLine->setObjectName(QStringLiteral("line"));
    m_vecticalLine->setEnabled(true);
    m_vecticalLine->hide();
    //m_vecticalLine->setGeometry(QRect(300, 250, 10, 200));
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
    vqsArgv.push_back(QString(tr("scale=%1")).arg("512:384")); //512:384 256:192
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
void ElementsEdit::addImages()
{
    QWidget *currWidget = qobject_cast<QWidget *>(sender());
    //只有image过来的insert事件，它的父类(element)才会是m_flowLayout的元素
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"),
            /*QDir::currentPath()*/QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            tr("Photos (*.jpg *.png *.bmp *.dib *.rle *.gif *.ico *.icon *.jpeg *.jpe *.jfif *.exif *.tiff *.tif *wdp *.jxr)"));
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
    int oldIdx = idx;
    QString qsFileType;
    for (int i = 0; i < files.count(); ++i) {
        // 1, 读取文件，生成image
        Element *element=new Element(this, files[i], m_globalContext->m_scene);
        emit createTextSignal((void*)element); 
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
    }
    //4, 生成总视频
    //./ffmpeg_gr -y -f avi -i jpg/mi2.avi -f avi -i jpg/mm.avi -f avi jpg/all.avi
    //uncomplete
    createFinalVideo(false);
    //5, 设置播放进度条
    //计算长度 for 进度条 ，需要在当前线程渲染之后，否则不对。放在duration信号槽内处理
    // 
    
    //6, 选中合适的image
    //oldIdx = oldIdx > 0? oldIdx: 0;
    //emit focusImageSignal();

    //7, 第一次选择
    if(!m_lastSelectedElement)
    {
        m_idxCurrentElement = 0;
        m_lastSelectedElement = qobject_cast<QWidget *>(m_flowLayout->itemAt(m_idxCurrentElement)->widget());
        (qobject_cast<Element *>(m_lastSelectedElement))->doSelectImage();
        emit displayTextSignal((void*)m_lastSelectedElement, true);
    }


    m_vecticalLine->raise(); // top level, Raises this widget to the top of the parent widget's stack.
    setCursor(QCursor(Qt::ArrowCursor));
}
void ElementsEdit::addMusic()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Add music"),
            /*QDir::currentPath()*/QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
            tr("Audio and Music (*.wma *.mp3 *.wav *.aif *.aiff *.m4a *.ogg)"));
    if (file.isEmpty() )
        return;
    //QFileInfo fi(file);
    //QString ext = fi.suffix();  // ext = "gz"
    m_audioMediaPlayer.setMedia(QUrl::fromLocalFile(file));
    connect(&m_audioMediaPlayer, SIGNAL(durationChanged(qint64)), this,
            SLOT(audioDurationChanged(qint64)));

    m_isFirstMusic = true;
    m_globalMusicAttr->m_qsAudioFilename = file;

    setCursor(QCursor(Qt::WaitCursor));
#if 0
    m_qsAudioFilename = file;
    m_iAudioStart = 0;
    m_iAudioDuration = 2;

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
    vqsArgv.push_back(QString(tr("%1")).arg(m_iAudioStart));
    vqsArgv.push_back(QString(tr("-t")));
    vqsArgv.push_back(QString(tr("%1")).arg(m_iAudioDuration));
    vqsArgv.push_back(QString(tr("-i")));
    vqsArgv.push_back(m_qsAudioFilename);

    vqsArgv.push_back("-i");
    struct to_buffer sinbuffer;
    sinbuffer.ptr = (uint8_t*)m_pOutBuffer;
    sinbuffer.in_len = m_outlen;
    sinbuffer.out_len = NULL;
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&sinbuffer));
    //./ffmpeg_r.exe -y -i jpg/mp3.512.5.avi -vf ass=jpg/subtitle.ass jpg/subt.mp3.512.5.avi
    if(!m_qsInText.isEmpty())
    {
        vqsArgv.push_back(QString(tr("-vf")));
        struct to_buffer sintxtbuffer;
        sintxtbuffer.ptr = (uint8_t*)m_qsInText.data();
        sintxtbuffer.in_len = m_qsInText.length();
        sintxtbuffer.out_len = NULL;
        vqsArgv.push_back(QString(tr("ass=buffer|%1")).arg((size_t)&sintxtbuffer));
    }
#if 1
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(QString(tr("avi")));
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
void ElementsEdit::selectedText(const QString& oritxt)
{
    //只能先选定image，才能对text操作
    QWidget* send = qobject_cast<QWidget *>(sender());
    //QMessageBox::information(this, "info", QString(tr("selected send: %1 m_lastSelectedElement: %2")).arg((size_t)send).arg((size_t)m_lastSelectedElement));
    if(send == m_lastSelectedElement && send)
    {
        //QMessageBox::information(this, "info", QString(tr("send")));
        //QWidget *lastWidget = qobject_cast<QWidget *>(m_lastSelectedElement);
        //int idx = m_flowLayout->indexOf(lastWidget);
        //1, 激活对应VideoText
        emit activeVideoTextSignal((void*)send, oritxt);
        //2, 激活对应tabTextTool
        emit activeTabTextSignal((void*)send);
    }
}
void ElementsEdit::selectedMusic()
{
    emit activeTabMusicSignal(NULL);
}
void ElementsEdit::selectedImage()
{
    QWidget* send = qobject_cast<QWidget *>(sender());
    //QMessageBox::information(this, "info", QString(tr("selectedImage send: %1 m_lastSelectedElement: %2")).arg((size_t)send).arg((size_t)m_lastSelectedElement));
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
    if(m_imgWidth)
        m_imgPlayPosition = imgWidth*m_duration/m_imgWidth;
    emit changePlayPosition(m_imgPlayPosition); 

    // 2, 移除上次高亮
    if(send == m_lastSelectedElement)
        return;
    int iCurrentIdx=-1;
    if(send && (iCurrentIdx=m_flowLayout->indexOf(send))>=0)
    {
        m_idxCurrentElement = iCurrentIdx;
    }
    if(m_lastSelectedElement)
    {
        QWidget *lastWidget = qobject_cast<QWidget *>(m_lastSelectedElement);
        int idx = m_flowLayout->indexOf(lastWidget);
        if(idx<0)
        {
            QMessageBox::information(this, "error", QString(tr("can't find last widget")));
        }
        else
        {
            (qobject_cast<Element *>(m_lastSelectedElement))->unselectedImage();
            //移除上一次的VideoText的显示
            emit displayTextSignal((void*)m_lastSelectedElement, false);
        }
    }
    // 3, 显示本次VideoText
    m_lastSelectedElement=send;
    emit displayTextSignal((void*)send, true);
    // 4, 激活对应tabTextTool
    //emit activeTabTextSignal((void*)send);
}
//void ElementsEdit::updatedText(stTextAttr* stTextAttr, const QString& qsText)
void ElementsEdit::updatedText(const QString& qsAss)
{
    m_qsInText = qsAss.toUtf8();
    createFinalVideoMusicTxt(false);
#if 0
    QVector<QString> vqsArgv;
    vqsArgv.push_back("ffmpeg");
    vqsArgv.push_back("-y");
#ifdef DEBUG_FFMPEG
    vqsArgv.push_back("-v");
    vqsArgv.push_back("debug");
#endif
    if(!m_qsAudioFilename.isEmpty())
    {
        //vqsArgv.push_back(QString(tr("-f")));
        //vqsArgv.push_back(QString(tr("%1")).arg(ext));
        vqsArgv.push_back(QString(tr("-ss")));
        vqsArgv.push_back(QString(tr("%1")).arg(m_iAudioStart));
        vqsArgv.push_back(QString(tr("-t")));
        vqsArgv.push_back(QString(tr("%1")).arg(m_iAudioDuration));
        vqsArgv.push_back(QString(tr("-i")));
        vqsArgv.push_back(m_qsAudioFilename);
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
    m_qsInText = qsAss.toUtf8();
    //QMessageBox::information(this, "info", QString(tr("qsAss: %1")).arg(qsAss));
    struct to_buffer sintxtbuffer;
    sintxtbuffer.ptr = (uint8_t*)m_qsInText.data();
    sintxtbuffer.in_len = m_qsInText.length();
    sintxtbuffer.out_len = NULL;
    vqsArgv.push_back(QString(tr("ass=buffer|%1")).arg((size_t)&sintxtbuffer));
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(QString(tr("avi")));
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
//当前element的属性变化，包括：
//动画
//视屏参数：时长
//文字
void ElementsEdit::elementAttrChanged(bool bPlay)
{
    //1, 生成单独的视频
    if(m_idxCurrentElement >= 1)
    {
    }
    else
    {
    }

    //2, 合成声音、文字
    createFinalVideo(bPlay);
}
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

    if(!m_globalMusicAttr->m_qsAudioFilename.isEmpty())
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
            vqsArgv.push_back(m_globalMusicAttr->m_qsAudioFilename);
            vqsArgv.push_back(QString(tr("-filter_complex")));
            int iRealLength = m_globalMusicAttr->m_iEntPoint - m_globalMusicAttr->m_iStartPoint+m_globalMusicAttr->m_iStartTime;
            /*QMessageBox::information(NULL, "info", QString(tr("vduration: %1 starttime: %2 startpoint: %3 endpoint: %4")).
                    arg(m_duration).arg(m_globalMusicAttr->m_iStartTime).
                    arg(m_globalMusicAttr->m_iStartPoint).arg(m_globalMusicAttr->m_iEntPoint));*/
            vqsArgv.push_back(QString(tr("[1:a]atrim=0:%1,asetpts=PTS-STARTPTS[aud1];"
                            "[2:a]atrim=%2:%3,asetpts=PTS-STARTPTS[aud2];"
                            "[aud1][aud2]concat=n=2:v=0:a=1[aout]")).
                    arg((float)m_globalMusicAttr->m_iStartTime/1000).
                    arg((float)m_globalMusicAttr->m_iStartPoint/1000).
                    arg(iRealLength<=m_duration?((float)m_globalMusicAttr->m_iEntPoint/1000):((float)(m_duration-m_globalMusicAttr->m_iStartTime+m_globalMusicAttr->m_iStartPoint)/1000)));
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
            int iRealDuration = m_globalMusicAttr->m_iEntPoint - m_globalMusicAttr->m_iStartPoint;
            vqsArgv.push_back(QString(tr("%1")).arg(iRealDuration<m_duration?((float)iRealDuration/1000):(float)(m_duration/1000)));
            vqsArgv.push_back(QString(tr("-i")));
            vqsArgv.push_back(m_globalMusicAttr->m_qsAudioFilename);
        }
    }

    //./ffmpeg_r.exe -y -i jpg/mp3.512.5.avi -vf ass=jpg/subtitle.ass jpg/subt.mp3.512.5.avi
    if(!m_qsInText.isEmpty())
    {
        vqsArgv.push_back(QString(tr("-vf")));
        struct to_buffer sintxtbuffer;
        sintxtbuffer.ptr = (uint8_t*)m_qsInText.data();
        sintxtbuffer.in_len = m_qsInText.length();
        sintxtbuffer.out_len = NULL;
        vqsArgv.push_back(QString(tr("ass=buffer|%1")).arg((size_t)&sintxtbuffer));
    }
#if 1
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(QString(tr("avi")));
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
        if(!element->m_fbInputAniVideo.ptr)
        {
            // err uncomplete
            continue;
        }
        if(element->m_fbInputTransitionVideo.ptr)
        {
            sinString.append(QString(tr("file buffer:video/avi;nobase64,%1\n")).arg((size_t)&element->m_fbInputTransitionVideo));
        }
        else if(element->m_fbInputPanzoomVideo.ptr)
        {
            sinString.append(QString(tr("file buffer:video/avi;nobase64,%1\n")).arg((size_t)&element->m_fbInputPanzoomVideo));
        }
        else
        {
        //snprintf(in_buffer, len, "file buffer:video/avi;nobase64,%zu\n
            sinString.append(QString(tr("file buffer:video/avi;nobase64,%1\n")).arg((size_t)&element->m_fbInputAniVideo));
        }
    }
    sinbuffer.ptr = (uint8_t*)sinString.data();
    sinbuffer.in_len = sinString.length();
    sinbuffer.out_len = NULL;
    vqsArgv.push_back("-f");
    vqsArgv.push_back("concat"); //uncomplete
    vqsArgv.push_back(QString(tr("-i")));
    vqsArgv.push_back(QString(tr("buffer:video/avi;nobase64,%1")).arg((size_t)&sinbuffer));
    if(m_globalMusicAttr->m_qsAudioFilename.isEmpty() && m_qsInText.isEmpty())
    {
        vqsArgv.push_back(QString(tr("-c")));
        vqsArgv.push_back(QString(tr("copy")));
    }
    else
    {
        if(!m_globalMusicAttr->m_qsAudioFilename.isEmpty())
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
                vqsArgv.push_back(m_globalMusicAttr->m_qsAudioFilename);
                vqsArgv.push_back(QString(tr("-filter_complex")));
                int iRealLength = m_globalMusicAttr->m_iEntPoint - m_globalMusicAttr->m_iStartPoint+m_globalMusicAttr->m_iStartTime;
                /*QMessageBox::information(NULL, "info", QString(tr("vduration: %1 starttime: %2 startpoint: %3 endpoint: %4")).
                  arg(m_duration).arg(m_globalMusicAttr->m_iStartTime).
                  arg(m_globalMusicAttr->m_iStartPoint).arg(m_globalMusicAttr->m_iEntPoint));*/
                vqsArgv.push_back(QString(tr("[1:a]atrim=0:%1,asetpts=PTS-STARTPTS[aud1];"
                                "[2:a]atrim=%2:%3,asetpts=PTS-STARTPTS[aud2];"
                                "[aud1][aud2]concat=n=2:v=0:a=1[aout]")).
                        arg((float)m_globalMusicAttr->m_iStartTime/1000).
                        arg((float)m_globalMusicAttr->m_iStartPoint/1000).
                        arg(iRealLength<=m_duration?((float)m_globalMusicAttr->m_iEntPoint/1000):((float)(m_duration-m_globalMusicAttr->m_iStartTime+m_globalMusicAttr->m_iStartPoint)/1000)));
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
                int iRealDuration = m_globalMusicAttr->m_iEntPoint - m_globalMusicAttr->m_iStartPoint;
                vqsArgv.push_back(QString(tr("%1")).arg(iRealDuration<m_duration?((float)iRealDuration/1000):(float)(m_duration/1000)));
                vqsArgv.push_back(QString(tr("-i")));
                vqsArgv.push_back(m_globalMusicAttr->m_qsAudioFilename);
            }
        }

        //./ffmpeg_r.exe -y -i jpg/mp3.512.5.avi -vf ass=jpg/subtitle.ass jpg/subt.mp3.512.5.avi
        if(!m_qsInText.isEmpty())
        {
            vqsArgv.push_back(QString(tr("-vf")));
            struct to_buffer sintxtbuffer;
            sintxtbuffer.ptr = (uint8_t*)m_qsInText.data();
            sintxtbuffer.in_len = m_qsInText.length();
            sintxtbuffer.out_len = NULL;
            vqsArgv.push_back(QString(tr("ass=buffer|%1")).arg((size_t)&sintxtbuffer));
        }
    }
    //printf("video input. stbuf: %p buf.ptr: %p in_len: %zu\n", &sinbuffer, sinbuffer.ptr, sinbuffer.in_len);

    uint8_t* out_buffer = m_pOutBuffer;
    m_outlen = m_outMaxLen;

    struct to_buffer soutbuffer;
    soutbuffer.ptr = out_buffer;
    soutbuffer.in_len = m_outMaxLen;
    soutbuffer.out_len = &m_outlen;
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
    QByteArray tmp=QByteArray((const char*)soutbuffer.ptr, (int)m_outlen);
    emit readyVideo("tmp.avi", tmp, m_imgPlayPosition);
    emit changePlayPosition(m_imgPlayPosition);
    if(bPlay)
    {
        emit playVideo();
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
    vqsArgv.push_back(QString(tr("avi")));
    vqsArgv.push_back(QString(tr("buffer:video/avi;nobase64,%1")).arg((size_t)&soutbuffer));
    //vqsArgv.push_back(QString(tr("mm.avi")));
    //printf("video out. stbuf: %p buf.ptr: %p in_len: %zu out_len: %zu\n", &soutbuffer, soutbuffer.ptr, soutbuffer.in_len, *soutbuffer.out_len);
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    if(m_globalMusicAttr->m_qsAudioFilename.isEmpty() && m_qsInText.isEmpty())
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
//每次重新生成视频时都会调用此函数. (生成动画时不能初始化m_vecticalLine的位置)
void ElementsEdit::durationChanged(qint64 duration)
{
    m_duration=duration;
    initialProgress(); //需要在addImages 图片渲染之后被调用，否则element的长宽不对
}
//用于加载音乐获取duration
void ElementsEdit::audioDurationChanged(qint64 duration)
{
    if(m_isFirstMusic)
    {
        //1, create video
        m_isFirstMusic = false;
        m_globalMusicAttr->m_iAudioDuration = duration;
        m_globalMusicAttr->m_iEntPoint = duration;
        createFinalVideoMusicTxt(false);

        //2, 显示music文件名和颜色
        for (int i = 0; m_flowLayout && i < m_flowLayout->count(); ++i)
        {
            Element *element = qobject_cast<Element *>(m_flowLayout->itemAt(i)->widget());
            if (!element)
            {
                // err uncomplete
                continue;
            }
        }

        //3, update tabwidget
        emit activeTabMusicSignal(m_globalMusicAttr);
    }
}
//视频播放时被调用，m_vecticalLine也需要跟着变化
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
    if(m_duration && m_signalImgWidth)
    {
        int xMax=(geometry().width()/m_signalImgWidth)*m_signalImgWidth;
        //QMessageBox::information(this, "info", QString(tr("xMax: %1 width: %2 signalwidth: %3")).arg(xMax).arg(geometry().width()).arg(m_signalImgWidth));
        int xOri=m_playPosition*m_imgWidth/m_duration;
        int x=xOri%xMax;
        int y=(xOri/xMax)*m_imgHeight;
        //if(!x)
        //    QMessageBox::information(this, "info", QString(tr("xMax: %1 xOri: %2 x: %3 y: %4 posi: %5 duration: %6 m_imgWidth: %7")).arg(xMax).arg(xOri).arg(x).arg(y).arg(m_playPosition).arg(m_duration).arg(m_imgWidth));
            //QMessageBox::information(this, "info", QString(tr("assign m_vecticalLine")));
        m_vecticalLine->setGeometry(QRect(x, y, m_lineWidth, m_imgHeight)); //uncomplete
    }
}
QByteArray ElementsEdit::createAss(stTextAttr* stTextAttr, const QString& qsText)
{
    //http://bbs.tgbus.com/thread-927116-1-1.html
    QByteArray qs("[Script Info]\n"
"; Script generated by FFmpeg/Lavc57.24.102\n"
"ScriptType: v4.00+\n"
"PlayResX: 512\n"
"PlayResY: 384\n"
"\n"
"[V4+ Styles]\n"
"Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, AlphaLevel, Encoding\n"
"Style: Default,楷体,   16,       &Hffffff,      &Hffffff,        &H0,           &H0,        0,         0,      0,         0,         100,    100,    0,       0,     1,           1,       0,      2,          10,      10,      10,      0,          0\n"
"\n"
"[Events]\n"
"Format:   Layer, Start,      End,        Style,  Name, MarginL, MarginR, MarginV, Effect, Text\n"
";Dialogue: 0,     0:00:00.001, 0:00:01.99, Default,    , 0,       0,       0,             , Go kill something.\n"
"Dialogue: 0,     0:00:00.01, 0:00:00.94, Default,    , 0,       0,       0,             , {\\move(0,0,60,120 [,100,500])}{\\fad(400,80)}{\\fs18\\t(130,500,\\fry360)}- Oh, stupid.\\N贱人\n"
";Dialogue: 0,     0:00:01.01, 0:00:01.99, Default,    , 0,       0,       0,             , Go kill something.\n"
);
    return qs;
}
void ElementsEdit::createSingleVideo(int idxElement)
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
        Element *firstElement = qobject_cast<Element *>(m_flowLayout->itemAt(m_idxCurrentElement-1)->widget());
        if(firstElement)
        {
            if(globalAnimationAttr->m_qsPanZoom.isEmpty())
            {
                //./ffmpeg_r.exe -y -i jpg/512.3.avi -i jpg/512.2.avi -filter_complex     "blend=all_expr='if(crossfade,1.5,2)'"  jpg/transition.avi
                createPanzoomVideo(secondElement,globalVideoAttr->m_iFramerate, duration,globalAnimationAttr->m_qsPanZoom);
            }
            else
            {
            // pan & zoom
                //./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img003.jpg -vf "zoompan=z='zoom+0.001':s=512x384"  -t 2 jpg/zoom.512.3.avi;
                //./ffmpeg_r.exe -y -i jpg/zoom.512.3.avi -i jpg/zoom.512.2.avi -filter_complex     "blend=all_expr='if(crossfade,1.5,2)'"  jpg/transition_zoom.avi
                createAnimationPanzoom(firstElement, secondElement);
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
        createPanzoomVideo(secondElement, globalVideoAttr->m_iFramerate, duration, globalAnimationAttr->m_qsPanZoom);
    }
    else
    {
        //./ffmpeg_r.exe -y  -framerate 24 -loop 1 -t 2 -i jpg/512img003.jpg -f avi jpg/loop.512.3.avi;
        createSimpleVideo(secondElement);
    }
}
void ElementsEdit::createSimpleVideo(Element *element)
{
    GlobalVideoAttr* globalVideoAttr = element->globalVideoAttr();
    QString duration = QString::number((float)globalVideoAttr->m_iDuration/1000, 'f', 2);

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
#if 0
bool ElementsEdit::createAnimation(Element *firstElement, Element *secondElement
        , const QString& animationName)
#else
bool ElementsEdit::createAnimation(Element *firstElement, Element *secondElement, bool isFromPanzoom)
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
        vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&firstElement->m_fbInputPanzoomVideo));
    }
    else
    {
        vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&firstElement->m_fbInputAniVideo));
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
        vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&secondElement->m_fbInputPanzoomVideo));
    }
    else
    {
        vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&secondElement->m_fbInputAniVideo));
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
    vqsArgv.push_back(QString(tr("avi")));
#if 0
    secondElement->m_fbScaleAniVideo.in_len = 10*1024*1024;
    *secondElement->m_fbScaleAniVideo.out_len = 10*1024*1024;
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&secondElement->m_fbScaleAniVideo));
#endif
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&secondElement->m_fbTransitionVideo));
#endif
    // -y -framerate 1 -i "C:\QtProjects\qtmovie\jpg\img%3d.jpg" myoutput.avi
    //QTime startTime = QTime::currentTime();
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
#if 0
    secondElement->m_fbInputAniVideo.ptr=secondElement->m_fbScaleAniVideo.ptr;
    secondElement->m_fbInputAniVideo.in_len=*secondElement->m_fbScaleAniVideo.out_len;
    secondElement->m_fbInputAniVideo.out_len=NULL;
#endif
    secondElement->m_fbInputTransitionVideo.ptr=secondElement->m_fbTransitionVideo.ptr;
    secondElement->m_fbInputTransitionVideo.in_len=*secondElement->m_fbTransitionVideo.out_len;
    secondElement->m_fbInputTransitionVideo.out_len=NULL;
    //int dt = startTime.msecsTo(QTime::currentTime());
    //qDebug()<< "ffmpeg waste: " << dt;
    //QMessageBox::information(this, "info", QString(tr("ffmpeg waste: %1")).arg(dt));
    //emit playVideo(vfileName);
    return true;
}
void ElementsEdit::createPanzoomVideo(Element *element, int framerate, const QString& duration, const QString& panzoom)
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
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&element->m_fbInputScaleFile));
    //-vf "zoompan=z='zoom+0.001':s=512x384" -t 2
    vqsArgv.push_back(QString(tr("-vf")));
    vqsArgv.push_back(QString(tr("%1")).arg(panzoom));
    vqsArgv.push_back(QString(tr("-t")));
    vqsArgv.push_back(QString(tr("%1")).arg(duration));
    vqsArgv.push_back(QString(tr("-f")));
    vqsArgv.push_back(QString(tr("avi")));
    vqsArgv.push_back(QString(tr("buffer:image/jpg;nobase64,%1")).arg((size_t)&element->m_fbPanzoomVideo));
    //vqsArgv.push_back(QString(tr("pic%1.avi")).arg(i));
    int ret;
    if((ret=callFfmpeg(vqsArgv)))
    {
        QMessageBox::information(this, "error", QString(tr("callffmpeg: %1")).arg(ret));
    }
    element->m_fbInputPanzoomVideo.ptr=element->m_fbPanzoomVideo.ptr;
    element->m_fbInputPanzoomVideo.in_len=*element->m_fbPanzoomVideo.out_len;
    element->m_fbInputPanzoomVideo.out_len=NULL;
}
// pan & zoom
//./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img003.jpg -vf "zoompan=z='zoom+0.001':s=512x384"  -t 2 jpg/zoom.512.3.avi;
//./ffmpeg_r.exe -y -i jpg/zoom.512.3.avi -i jpg/zoom.512.2.avi -filter_complex     "blend=all_expr='if(crossfade,1.5,2)'"  jpg/transition_zoom.avi
bool ElementsEdit::createAnimationPanzoom(Element *firstElement, Element *secondElement)
{
    qDebug()<< "createAnimationPanzoom";
    GlobalVideoAttr* globalVideoAttr = secondElement->globalVideoAttr();
    GlobalAnimationAttr* globalAnimationAttr = secondElement->globalAnimationAttr();
    QString duration = QString::number((float)globalVideoAttr->m_iDuration/1000, 'f', 2);
    //float animationDuration = QString::number((float)globalAnimationAttr->m_iTransitionDuration/1000, 'f', 2);

    createPanzoomVideo(firstElement, globalVideoAttr->m_iFramerate, duration, globalAnimationAttr->m_qsPanZoom);
    createPanzoomVideo(secondElement, globalVideoAttr->m_iFramerate, duration, globalAnimationAttr->m_qsPanZoom);

    createAnimation(firstElement, secondElement, true);

    return true;
}
