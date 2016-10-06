#include "element.h"
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

Element::Element(QWidget *parent, const QString& qsImageName,DiagramScene* scene)
    : QWidget(parent)
    , m_elementLayout(new QVBoxLayout(this))
    , m_pimage(0)
    , m_qsImageName(qsImageName)
{
    //setWindowFlags(Qt::WindowStaysOnBottomHint);
    memset(&m_fbOriFile, 0, sizeof(m_fbOriFile));
    memset(&m_fbScaleFile, 0, sizeof(m_fbScaleFile));
    memset(&m_fbScaleAniVideo, 0, sizeof(m_fbScaleAniVideo));
    memset(&m_fbInputAniVideo, 0, sizeof(m_fbInputAniVideo));
    m_elementLayout->setSpacing(0);
    m_elementLayout->setMargin(0);
    int iMaxWidth=300;
    int iMaxHeight=iMaxWidth*3/4;
    //setMinimumWidth(iMaxWidth);
    //setMaximumWidth(iMaxWidth);
    //m_elementLayout->addWidget(new Image(tr("C:\\QtProjects\\qtmovie\\jpg\\img001.jpg")));
    if(!qsImageName.isEmpty())
    {
        m_pimage = new Image(qsImageName, QSize(iMaxWidth, iMaxHeight), this);
        //webenginewidgets/simplebrowser/tabwidget.cpp
        m_elementLayout->addWidget(m_pimage);

        connect(this, SIGNAL(insertImage()), parentWidget(), SLOT(load()) );

        //connect(parentWidget(), SIGNAL(focusImageSignal()), m_pimage(), SLOT(focusImage()) );
        //必须要二层，因为elementsedit是根据send(即element)来确定哪个element的
        connect(m_pimage, SIGNAL(selectedImageSignal()), this, SLOT(selectedImage()) );
        connect(this, SIGNAL(selectedImageSignal()), parentWidget(), SLOT(selectedImage()) );

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

        m_iOutVidow = 10*1024*1024;
        m_pBufferVideo=(uint8_t *)malloc(m_iOutVidow);

        m_fbScaleAniVideo.ptr = m_pBufferVideo;
        m_fbScaleAniVideo.in_len = m_iOutVidow;
        m_fbScaleAniVideo.out_len = &m_iOutVidow;

        m_iOutScaleFile = 1024*1024;
        m_pBufferScaleFile=(uint8_t *)malloc(m_iOutScaleFile);

        m_fbScaleFile.ptr = m_pBufferScaleFile;
        m_fbScaleFile.in_len = m_iOutScaleFile;
        m_fbScaleFile.out_len = &m_iOutScaleFile;
#if 0
        m_pimage->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(m_pimage, SIGNAL(customContextMenuRequested(QPoint)), parentWidget(), SLOT(handleContextMenuRequested(QPoint)) );
#endif
        //connect(this, &QLabel::customContextMenuRequested, parent()->parent(), &ElementsEdit::handleContextMenuRequested);

    }
    m_pushBtn = new QPushButton(tr(""));
    m_elementLayout->addWidget(m_pushBtn);
    m_lineEdit =  new LineEdit(scene);
    //m_elementLayout->addWidget(new QLabel(tr("input text")));
    m_elementLayout->addWidget(m_lineEdit);
    //connect(this, SIGNAL(selectedTextSignal()), parentWidget(), SLOT(selectedText()) );
    //connect(m_pushBtn, SIGNAL(clicked()), this, SLOT(selectedText()) );
    //
    //必须要二层，因为elementsedit是根据send(即element)来确定哪个element的
    connect(m_lineEdit, SIGNAL(selectedTextSignal(const QString&)), this, SLOT(selectedText(const QString&)) );
    connect(this, SIGNAL(selectedTextSignal(const QString&)), parent, SLOT(selectedText(const QString&)) );

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
    delete m_pimage;
#if 0
    // uncomplete
    for(m_elementLayout widget)
    {
        delete widget;
    }
#endif
    //delete m_fbScaleAniVideo.ptr; //uncomplete;
    delete m_elementLayout;
    m_baOriFile.clear();
}
void Element::insert()
{
    emit insertImage();
}
void Element::selectedImage()
{
    emit selectedImageSignal();
}
void Element::selectedText(const QString& ori)
{
    emit selectedTextSignal(ori);
}
