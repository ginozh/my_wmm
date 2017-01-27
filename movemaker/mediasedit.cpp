#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include "mediasedit.h"
#include "comm.h"
extern "C"{
#include "ffmpeg.h"
}

MediasEdit::MediasEdit(QWidget *parent)
    : QWidget(parent)
    , m_flowLayout(NULL)
    , m_firstLayout(NULL)
    , m_firstLabel(NULL)
    , m_isFirstClick(true)
{
    initialFirstLayout();
}
MediasEdit::~MediasEdit()
{
}
void MediasEdit::initialFirstLayout()
{
    setCursor(QCursor(Qt::PointingHandCursor));
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

    QFont font = m_firstLabel->font();
    font.setPointSize(25);
    font.setBold(true);
    m_firstLabel->setFont(font);
    //m_firstLabel->setAlignment(Qt::AlignCenter);
    m_firstLayout = new QVBoxLayout(this);
    //layout->addWidget(m_addPhotos, 0, Qt::AlignCenter);
    m_firstLayout->addWidget(m_firstLabel, 0, Qt::AlignCenter);

    setLayout(m_firstLayout);
}
void MediasEdit::mousePressEvent(QMouseEvent *event)
{
    if(m_isFirstClick)
    {
        addMedias();
    }
    QWidget::mousePressEvent(event);
}
void MediasEdit::addMedias()
{
    QWidget *currWidget = qobject_cast<QWidget *>(sender());
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"),
            /*QDir::currentPath()*/QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            tr("Photos (*.jpg *.png *.bmp *.dib *.rle *.gif *.ico *.icon *.jpeg *.jpe *.jfif *.exif *.tiff *.tif *wdp *.jxr)"));
    if (files.count() == 0)
        return;
    //setCursor(QCursor(Qt::WaitCursor));
    setCursor(QCursor(Qt::ArrowCursor));
    createFlowLayout();
    int idx = -1;
    //只有image过来的insert事件，它的父类(element)才会是m_flowLayout的元素
    if((idx=m_flowLayout->indexOf(currWidget))>=0)
        idx++;
    int oldIdx = idx;
    QString qsFileType;
    for (int i = 0; i < files.count(); ++i) {
        GlobalImageAttr  tmpGlobalImageAttr;
        tmpGlobalImageAttr.m_qsImageName = files[i];
        if(! initialImage(tmpGlobalImageAttr, idx))
        {
        }
    }
}

void MediasEdit::createFlowLayout()
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
Element* MediasEdit::initialImage(const GlobalImageAttr&  newGlobalImageAttr, int& idx)
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
    Element *element=new Element(this, newGlobalImageAttr);
    m_flowLayout->insertWidget(idx, element);
    if(idx>=0)
        idx++;
    return element;
}
