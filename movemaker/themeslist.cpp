#include <QToolButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include "themeslist.h"
#include "comm.h"
#include "image.h"

ThemesList::ThemesList(QWidget *parent)
    : QWidget(parent)
    , m_lastSelectedElement(0)
    , m_idxCurrentElement(-1)
{
    m_flowLayout = new FlowLayout(this);
    setLayout(m_flowLayout);

    m_flowLayout->insertWidget(m_flowLayout->count()
            , new ThemeElement("images/Simple.png", "Simple", this));
    m_flowLayout->insertWidget(m_flowLayout->count()
            , new ThemeElement("images/Olddays.png", "Old days", this));
}
void ThemesList::selectedImage(QWidget* theWidget)
{
    qDebug()<<"ThemesList. selectedImage";
    QWidget* send = qobject_cast<QWidget *>(sender());
    ThemeElement* sendElement = qobject_cast<ThemeElement *>(sender());
    ThemeElement* theElement = qobject_cast<ThemeElement *>(theWidget);
    for (int i = 0; i < m_flowLayout->count(); ++i)
    {
        ThemeElement *element = qobject_cast<ThemeElement *>(m_flowLayout->itemAt(i)->widget());
        if (!element)
        {
            // err uncomplete
            qDebug()<< "error uncomplete. ElementsEdit::selectedImage";
            continue;
        }
        if(send == element  || theWidget == element)
        {
            //不是从element过来的
            if(send != element)
            {
                qDebug()<< "ElementsEdit::selectedImage. focusimage";
                element->doFocusImage();
            }
            else if(send == element)
            {
            }
            break;
        }
    }
    if((!theWidget && send == m_lastSelectedElement) || (theWidget && theWidget == m_lastSelectedElement))
        return;
    // 移除上次高亮
    qDebug()<< "ElementsEdit::selectedImage. rm last highlight";
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
            (qobject_cast<ThemeElement *>(m_lastSelectedElement))->unselectedImage();
        }
    }
    // 6, 赋值
    m_lastSelectedElement = theWidget?theWidget:send;
}

ThemesList::~ThemesList()
{
}

ThemeElement::ThemeElement(const QString& imgName, const QString& displayName, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* vbox = new QVBoxLayout;
    setLayout(vbox);

    GlobalContext* globalContext = GlobalContext::instance();
    double dFactorX = globalContext->m_dFactorX;
    double dFactorY = globalContext->m_dFactorY;
    int iMaxWidth=240*dFactorX;
    int iMaxHeight=iMaxWidth*dFactorY*3/4;

    GlobalImageAttr tmpGlobalImageAttr;tmpGlobalImageAttr.m_qsImageName = imgName;
    m_pimage = new Image(tmpGlobalImageAttr, QSize(iMaxWidth, iMaxHeight), this);
    vbox->addWidget(m_pimage);

    connect(m_pimage, SIGNAL(selectedImageSignal()), this, SLOT(selectedImage()) );
    connect(this, SIGNAL(selectedImageSignal(QWidget*)), parentWidget(), SLOT(selectedImage(QWidget*)) );
#if 0
    QLabel* image = new QLabel;
    vbox->addWidget(image);
    QSize sizeImage(QSize(iMaxWidth, iMaxHeight));
    image->setFixedSize(sizeImage);
    QPixmap* pixMap = new QPixmap;
    pixMap->load(imgName); // 可能在新环境下加载失败，需要添加图片格式qt解析dll库
    pixMap->scaled(sizeImage);
    image->setPixmap(*pixMap);
    image->setScaledContents(true);
#endif
    QLabel* txt = new QLabel(displayName);
    txt->setFixedSize(QSize(iMaxWidth, iMaxWidth/6));
    txt->setAlignment(Qt::AlignHCenter);
    vbox->addWidget(txt);
}
void ThemeElement::selectedImage()
{
    qDebug()<<"ThemeElement. selectedImage";
    emit selectedImageSignal(NULL);
}

ThemeElement::~ThemeElement()
{
}
