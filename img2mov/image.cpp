#include "image.h"
#include <QMessageBox>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QDebug>
class ElementsEdit;
Image::Image(const QString& path, QSize size, QWidget *parent)
    : QLabel(parent)
    , m_globalImageAttr(new GlobalImageAttr)
    //, m_pixMap(new QPixmap())
    //, m_iRotateLeft(0)
    //, m_iRotateRight(0)
{
    m_focus=false;
    m_iScaleSize = size;
    setFixedSize(size);
    //setMinimumHeight(size.height());
    //setMaximumHeight(size.height());
    //setContentsMargins(5, 5, 5, 5);
#if 1
    m_pixMap.load(path);
    const QImage& qImage = m_pixMap.toImage(); 
    qDebug()<<"Image. height: "<<qImage.size().height()<<" width: "<<qImage.size().width();
    m_globalImageAttr->m_iSize = qImage.size();
    m_pixMap.scaled(m_iScaleSize);
    setPixmap(m_pixMap);
    setScaledContents(true);
    //setMaximumSize(200,200);
    //setMinimumSize(200,200);
#else
    if(!path.isEmpty())
    {
        m_pixMap.load(path);
        QPixmap& image=m_pixMap;
        const QSize maximumSize(size); // Reduce in case someone has large photo images.
        if (image.size().width() > maximumSize.width() || image.height() > maximumSize.height())
            image = image.scaled(maximumSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        setPixmap(m_pixMap);
    }
#endif
    setContextMenuPolicy(Qt::DefaultContextMenu);
    //webenginewidgets/simplebrowser/tabwidget.cpp
    //setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(this, &QLabel::customContextMenuRequested, parent()->parent(), &ElementsEdit::handleContextMenuRequested);
    //connect(this, SIGNAL(selectedImageSignal()), parentWidget(), SLOT(selectedImage()));
}
void Image::contextMenuEvent(QContextMenuEvent * /*event*/)
{
    //connect(tree, SIGNAL(customContextMenuRequested(QPoint)), this,    SLOT(customContextMenuRequested(QPoint)));
    //QMessageBox::information(this,tr("right button"),tr("right"));
    QMenu menu;
    //menu.addAction(tr("Open"), this, SLOT(open()));
    //menu.addAction(tr("Add images"), parentWidget()->parentWidget(), SLOT(addImages())  );
    menu.addAction(tr("Add photos"), parentWidget(), SLOT(insert())  );
    menu.addAction(tr("Remove"), parentWidget(), SLOT(remove())  );
    menu.exec(QCursor::pos());
}
#if 0
//replaced by doFocusImage
void Image::focusImage()
{
    mousePressEvent(NULL);
}
#endif
#if 1
void Image::mousePressEvent(QMouseEvent *event)
{
    m_focus=true;
    update();
    emit selectedImageSignal();
    if (event && (event->buttons() & Qt::RightButton))
    {
        //QMessageBox::information(this,tr("right button"),tr("right"));
    }
}
#endif
#if 0
void Image::open()
{
}
#endif
void Image::paintEvent(QPaintEvent *ev)
{
    QLabel::paintEvent(ev);
    if(m_focus)
    {
        QPainter draw(this);
        QPen pen(QColor(Qt::red));
        pen.setWidth(20);
        draw.setPen(pen);
        //draw.setPen(QPen(QColor(Qt::red)));
        //draw.drawLine(rect().topLeft(), rect().bottomRight());
        draw.drawRect(rect());
    }
}
void Image::unselectedImage()
{
    m_focus=false;
    update();
}
void Image::doFocusImage()
{
    m_focus=true;
    update();
}
void Image::rotate(bool bRight)
{
	QPixmap pixmap(m_pixMap);
	QMatrix rm;
    if(bRight)
    {
        rm.rotate(90);
        m_globalImageAttr->m_iRotateRight ++;
    }
    else
    {
        rm.rotate(270);
        m_globalImageAttr->m_iRotateLeft ++;
    }
	m_pixMap = pixmap.transformed(rm);
	setPixmap(m_pixMap);
}
/*
#0 = 90CounterCLockwise and Vertical Flip (default)
#1 = 90Clockwise 顺时针
#2 = 90CounterClockwise 逆时针
#3 = 90Clockwise and Vertical Flip
#-vf "transpose=2,transpose=2" for 180 degrees
 */
QString Image::rotateVFilter()
{
    int iRotateRight;
    QString qsVFilter;
    if(m_globalImageAttr->m_iRotateRight>=m_globalImageAttr->m_iRotateLeft)
    {
        iRotateRight = (m_globalImageAttr->m_iRotateRight-m_globalImageAttr->m_iRotateLeft)%4;
    }
    else 
    {
        iRotateRight = 4-(m_globalImageAttr->m_iRotateLeft-m_globalImageAttr->m_iRotateRight)%4;
    }
    switch(iRotateRight){
        case 1:
            qsVFilter = "transpose=1";
            break;
        case 2:
            qsVFilter = "transpose=1,transpose=1";
            break;
        case 3:
            qsVFilter = "transpose=2";
            break;
        default:
            break;
    }
    return qsVFilter;
}
