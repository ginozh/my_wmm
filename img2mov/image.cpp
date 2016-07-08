#include "image.h"
#include <QMessageBox>
#include <QMenu>

Image::Image(const QString& path)
    : m_pixMap(new QPixmap())
{
    if(!path.isEmpty())
    {
        m_pixMap->load(path);
        setPixmap(*m_pixMap);
    }
    setContextMenuPolicy(Qt::DefaultContextMenu);
    //connect(this, &QLabel::customContextMenuRequested, this, &TabWidget::handleContextMenuRequested);
}
void Image::contextMenuEvent(QContextMenuEvent * event)
{
    //connect(tree, SIGNAL(customContextMenuRequested(QPoint)), this,    SLOT(customContextMenuRequested(QPoint)));
    //QMessageBox::information(this,tr("right button"),tr("right"));
    QMenu menu;
    menu.addAction(tr("Open"), this, SLOT(open()));
    menu.exec(QCursor::pos());
}
#if 0
void Image::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton)
    {
        QMessageBox::information(this,tr("right button"),tr("right"));
    }
}
#endif
