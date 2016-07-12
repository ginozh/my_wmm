#include "image.h"
#include <QMessageBox>
#include <QMenu>
#include <QPainter>
#include <QPen>
class ElementsEdit;
Image::Image(QWidget *parent,const QString& path)
    : QLabel(parent)
    , m_pixMap(new QPixmap())
{
    m_focus=false;
    setScaledContents(true);
    if(!path.isEmpty())
    {
        m_pixMap->load(path);
        QPixmap& image=*m_pixMap;
        const QSize maximumSize(200, 200); // Reduce in case someone has large photo images.
        if (image.size().width() > maximumSize.width() || image.height() > maximumSize.height())
            image = image.scaled(maximumSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        setPixmap(*m_pixMap);
    }
    setContextMenuPolicy(Qt::DefaultContextMenu);
    //webenginewidgets/simplebrowser/tabwidget.cpp
    //setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(this, &QLabel::customContextMenuRequested, parent()->parent(), &ElementsEdit::handleContextMenuRequested);
    connect(this, SIGNAL(selectedImageSignal()), parentWidget(), SLOT(selectedImage()));
}
void Image::contextMenuEvent(QContextMenuEvent * event)
{
    //connect(tree, SIGNAL(customContextMenuRequested(QPoint)), this,    SLOT(customContextMenuRequested(QPoint)));
    //QMessageBox::information(this,tr("right button"),tr("right"));
    QMenu menu;
    //menu.addAction(tr("Open"), this, SLOT(open()));
    //menu.addAction(tr("Add images"), parentWidget()->parentWidget(), SLOT(load())  );
    menu.addAction(tr("Add images"), parentWidget(), SLOT(insert())  );
    menu.exec(QCursor::pos());
}
#if 1
void Image::mousePressEvent(QMouseEvent *event)
{
    m_focus=true;
    update();
    emit selectedImageSignal();
    if (event->buttons() & Qt::RightButton)
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
