#include <QLabel>
#include <QMessageBox>
#include <QFileInfo> 
#include "element.h"

Element::Element(QWidget *parent, const GlobalImageAttr& newGlobalImageAttr)
    : QWidget(parent)
    , m_elementLayout(new QVBoxLayout(this))
    , m_pimage(0)
{
    GlobalContext* globalContext = GlobalContext::instance();
    double dFactorX = globalContext->m_dFactorX;
    double dFactorY = globalContext->m_dFactorY;

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
    m_elementLayout->setMargin(45*dFactorX);

    int iMaxWidth=320*dFactorX;
    int iMaxHeight=iMaxWidth*dFactorY*3/4;
    const QString& qsImageName = newGlobalImageAttr.m_qsImageName;
    m_qsImageName=newGlobalImageAttr.m_qsImageName;
    if(!qsImageName.isEmpty())
    {
        m_pimage = new Image(newGlobalImageAttr, QSize(iMaxWidth, iMaxHeight), this);
        //webenginewidgets/simplebrowser/tabwidget.cpp
        m_elementLayout->addWidget(m_pimage);
    }
    // m_musicLabel = new MusicLabel(QSize(iMaxWidth, iMaxWidth/6));
    m_musicLabel = new QLabel();
    QFileInfo fi(qsImageName);
    QString qsImgBaseName = fi.baseName();
    m_musicLabel->setText(qsImgBaseName);
    m_musicLabel->setAlignment(Qt::AlignCenter);
    m_musicLabel->setFixedSize(QSize(iMaxWidth, iMaxWidth/6));
    m_elementLayout->addWidget(m_musicLabel);

    setLayout(m_elementLayout);
}

Element::~Element()
{

    m_elementLayout->removeWidget(m_pimage);
    m_elementLayout->removeWidget(m_musicLabel);
    delete m_pimage;
    delete m_musicLabel;
    
    delete m_elementLayout;

    delete[] m_pBufferScaledVideo;
    delete[] m_pBufferPanzoomVideo;
    delete[] m_pBufferTransitionVideo;
    delete[] m_pBufferScaleFile;
}
