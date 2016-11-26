#include "element.h"
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

Element::Element(QWidget *parent, const GlobalImageAttr& newGlobalImageAttr, GraphicsScene* scene)
//Element::Element(QWidget *parent, const QString& qsImageName,GraphicsScene* scene)
    : QWidget(parent)
    , m_elementLayout(new QVBoxLayout(this))
    , m_pimage(0)
    , m_bValid(true)
    , m_globalVideoAttr(new GlobalVideoAttr)
    , m_globalAnimationAttr(new GlobalAnimationAttr)
    , m_globalTextAttr(new GlobalTextAttr)
{
    GlobalContext* globalContext = GlobalContext::instance();
    double dFactorX = globalContext->m_dFactorX;
    double dFactorY = globalContext->m_dFactorY;

    //setWindowFlags(Qt::WindowStaysOnBottomHint);
    memset(&m_fbOriFile, 0, sizeof(m_fbOriFile));
    memset(&m_fbScaleFile, 0, sizeof(m_fbScaleFile));
    memset(&m_fbInputScaleFile, 0, sizeof(m_fbInputScaleFile));
    memset(&m_fbScaledVideo, 0, sizeof(m_fbScaledVideo));
    memset(&m_fbInputScaledVideo, 0, sizeof(m_fbInputScaledVideo));
    memset(&m_fbPanzoomVideo, 0, sizeof(m_fbPanzoomVideo));
    memset(&m_fbInputPanzoomVideo, 0, sizeof(m_fbInputPanzoomVideo));
    memset(&m_fbTransitionVideo, 0, sizeof(m_fbTransitionVideo));
    memset(&m_fbInputTransitionVideo, 0, sizeof(m_fbInputTransitionVideo));
    m_elementLayout->setSpacing(0);
    m_elementLayout->setMargin(0);
    int iMaxWidth=240*dFactorX;
    int iMaxHeight=iMaxWidth*dFactorY*3/4;
    //setMinimumWidth(iMaxWidth);
    //setMaximumWidth(iMaxWidth);
    //m_elementLayout->addWidget(new Image(tr("C:\\QtProjects\\qtmovie\\jpg\\img001.jpg")));
    const QString& qsImageName = newGlobalImageAttr.m_qsImageName;
    m_qsImageName=newGlobalImageAttr.m_qsImageName;
    if(!qsImageName.isEmpty())
    {
        m_pimage = new Image(newGlobalImageAttr, QSize(iMaxWidth, iMaxHeight), this);
        //webenginewidgets/simplebrowser/tabwidget.cpp
        m_elementLayout->addWidget(m_pimage);

        connect(this, SIGNAL(insertImage()), parentWidget(), SLOT(addImages()) );
        connect(this, SIGNAL(removeImage()), parentWidget(), SLOT(removeImage()) );

        //connect(parentWidget(), SIGNAL(focusImageSignal()), m_pimage(), SLOT(focusImage()) );
        //必须要二层，因为elementsedit是根据send(即element)来确定哪个element的
        connect(m_pimage, SIGNAL(selectedImageSignal()), this, SLOT(selectedImage()) );
        connect(this, SIGNAL(selectedImageSignal(QWidget*)), parentWidget(), SLOT(selectedImage(QWidget*)) );

        //load orignal file
        QFile file(qsImageName);
        if (!file.open(QFile::ReadOnly)) {
            //QMessageBox::warning(this, tr("Codecs"), tr("Cannot read file %1:\n%2") .arg(qsImageName) .arg(file.errorString()));
            //uncomplete
            return;
        }
        m_baOriFile = file.readAll();
        file.close();
        m_fbOriFile.ptr = (uint8_t*)m_baOriFile.data();
        m_fbOriFile.in_len = m_baOriFile.size();
        m_fbOriFile.out_len = NULL;

        m_iOutScaleFile = 1024*1024;
        m_pBufferScaleFile=(uint8_t *)new char[m_iOutScaleFile];
        m_fbScaleFile.ptr = m_pBufferScaleFile;
        m_fbScaleFile.in_len = m_iOutScaleFile;
        m_fbScaleFile.out_len = &m_iOutScaleFile;

        m_iOutScaledVideo = 10*1024*1024;
        m_pBufferScaledVideo=(uint8_t *)new char[m_iOutScaledVideo];
        m_fbScaledVideo.ptr = m_pBufferScaledVideo;
        m_fbScaledVideo.in_len = m_iOutScaledVideo;
        m_fbScaledVideo.out_len = &m_iOutScaledVideo;

        m_iOutPanzoomVideo = 10*1024*1024;
        m_pBufferPanzoomVideo=(uint8_t *)new char[m_iOutPanzoomVideo];
        m_fbPanzoomVideo.ptr = m_pBufferPanzoomVideo;
        m_fbPanzoomVideo.in_len = m_iOutPanzoomVideo;
        m_fbPanzoomVideo.out_len = &m_iOutPanzoomVideo;

        m_iOutTransitionVideo = 10*1024*1024;
        m_pBufferTransitionVideo=(uint8_t *)new char[m_iOutTransitionVideo];
        m_fbTransitionVideo.ptr = m_pBufferTransitionVideo;
        m_fbTransitionVideo.in_len = m_iOutTransitionVideo;
        m_fbTransitionVideo.out_len = &m_iOutTransitionVideo;

#if 0
        m_pimage->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(m_pimage, SIGNAL(customContextMenuRequested(QPoint)), parentWidget(), SLOT(handleContextMenuRequested(QPoint)) );
#endif
        //connect(this, &QLabel::customContextMenuRequested, parent()->parent(), &ElementsEdit::handleContextMenuRequested);

    }
    //m_pushBtn = new QPushButton(tr(""));
    //m_elementLayout->addWidget(m_pushBtn);
    m_musicLabel = new MusicLabel(QSize(iMaxWidth, iMaxWidth/6));
    m_elementLayout->addWidget(m_musicLabel);
    connect(m_musicLabel, SIGNAL(selectedMusicSignal()), parent, SLOT(selectedMusic()) );


    m_lineEdit =  new LineEdit(QSize(iMaxWidth, iMaxWidth/6), scene, this);
    //m_elementLayout->addWidget(new QLabel(tr("input text")));
    m_elementLayout->addWidget(m_lineEdit);
    //connect(this, SIGNAL(selectedTextSignal()), parentWidget(), SLOT(selectedText()) );
    //connect(m_pushBtn, SIGNAL(clicked()), this, SLOT(selectedText()) );
    //
    //必须要二层，因为elementsedit是根据send(即element)来确定哪个element的
    connect(m_lineEdit, SIGNAL(selectedTextSignal(const QString&)), this, SLOT(selectedText(const QString&)) );
    connect(this, SIGNAL(selectedTextSignal(const QString&)), parent, SLOT(selectedText(const QString&)) );
    //connect(parent, SIGNAL(addTextSignal()), m_lineEdit, SLOT(addText()) );

    setLayout(m_elementLayout);
#if 0
    for (int i = 0; i < m_elementLayout->count(); ++i)
    {
        QMessageBox::information(this, "info", QString(tr("i: %1 height %2")) .arg(i).arg(m_elementLayout->itemAt(i)->widget()->height())) ;
    }
#endif
}
Element::~Element()
{
    delete m_globalVideoAttr;
    delete m_globalAnimationAttr;
    delete m_globalTextAttr;
    m_globalVideoAttr=0;m_globalAnimationAttr=0;m_globalTextAttr=0;


    m_elementLayout->removeWidget(m_pimage);
    m_elementLayout->removeWidget(m_musicLabel);
    m_elementLayout->removeWidget(m_lineEdit);
    delete m_pimage;
    delete m_musicLabel;
    delete m_lineEdit;
    
    delete m_elementLayout;

    delete[] m_pBufferScaledVideo;
    delete[] m_pBufferPanzoomVideo;
    delete[] m_pBufferTransitionVideo;
    delete[] m_pBufferScaleFile;
    //m_baOriFile.clear();
}
void Element::insert()
{
    emit insertImage();
}
void Element::remove()
{
    emit removeImage();
}
void Element::selectedImage()
{
    emit selectedImageSignal(NULL);
}
void Element::selectedText(const QString& ori)
{
    emit selectedTextSignal(ori);
}
void Element::initialMemoryForVideoFile()
{
    memset(&m_fbFileScaleFile, 0, sizeof(m_fbFileScaleFile));
    memset(&m_fbFileInputScaleFile, 0, sizeof(m_fbFileInputScaleFile));
    memset(&m_fbFileScaledVideo, 0, sizeof(m_fbFileScaledVideo));
    memset(&m_fbFileInputScaledVideo, 0, sizeof(m_fbFileInputScaledVideo));
    memset(&m_fbFilePanzoomVideo, 0, sizeof(m_fbFilePanzoomVideo));
    memset(&m_fbFileInputPanzoomVideo, 0, sizeof(m_fbFileInputPanzoomVideo));
    memset(&m_fbFileTransitionVideo, 0, sizeof(m_fbFileTransitionVideo));
    memset(&m_fbFileInputTransitionVideo, 0, sizeof(m_fbFileInputTransitionVideo));

    m_iFileOutScaleFile = 10*1024*1024;
    m_pFileBufferScaleFile=(uint8_t *)new char[m_iFileOutScaleFile];
    m_fbFileScaleFile.ptr = m_pFileBufferScaleFile;
    m_fbFileScaleFile.in_len = m_iFileOutScaleFile;
    m_fbFileScaleFile.out_len = &m_iFileOutScaleFile;

    m_iFileOutScaledVideo = 10*1024*1024;
    m_pFileBufferScaledVideo=(uint8_t *)new char[m_iFileOutScaledVideo];
    m_fbFileScaledVideo.ptr = m_pFileBufferScaledVideo;
    m_fbFileScaledVideo.in_len = m_iFileOutScaledVideo;
    m_fbFileScaledVideo.out_len = &m_iFileOutScaledVideo;

    m_iFileOutPanzoomVideo = 10*1024*1024;
    m_pFileBufferPanzoomVideo=(uint8_t *)new char[m_iFileOutPanzoomVideo];
    m_fbFilePanzoomVideo.ptr = m_pFileBufferPanzoomVideo;
    m_fbFilePanzoomVideo.in_len = m_iFileOutPanzoomVideo;
    m_fbFilePanzoomVideo.out_len = &m_iFileOutPanzoomVideo;

    m_iFileOutTransitionVideo = 10*1024*1024;
    m_pFileBufferTransitionVideo=(uint8_t *)new char[m_iFileOutTransitionVideo];
    m_fbFileTransitionVideo.ptr = m_pFileBufferTransitionVideo;
    m_fbFileTransitionVideo.in_len = m_iFileOutTransitionVideo;
    m_fbFileTransitionVideo.out_len = &m_iFileOutTransitionVideo;
}
void Element::freeMemoryForVideoFile()
{
    delete[] m_pFileBufferScaledVideo;
    delete[] m_pFileBufferPanzoomVideo;
    delete[] m_pFileBufferTransitionVideo;
    delete[] m_pFileBufferScaleFile;
}
