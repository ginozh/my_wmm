#include "animation.h"
#include <QMessageBox>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QToolTip>
Animation::Animation(const QString& path, const QString& animation, QSize size, QWidget *parent)
    : QLabel(parent)
    , m_animation(animation)
    , m_pixMap(new QPixmap())
{
    //QMessageBox::information(this,tr("info"),QString(tr("path: %1 animation: %2")).arg(path).arg(animation));
    m_focus=false;
    setMinimumSize(size);
    setMaximumSize(size);
    setContentsMargins(5, 5, 5, 5);
    setPixmap(QPixmap(path));
    setScaledContents(true);
}
#if 1
void Animation::mousePressEvent(QMouseEvent *event)
{
#if 0
    m_focus=true;
    update();
    emit selectedAnimationSignal(m_animation);
    if (event->buttons() & Qt::RightButton)
    {
        //QMessageBox::information(this,tr("right button"),tr("right"));
    }
#endif
}
#endif
#if 0
void Animation::open()
{
}
#endif
void Animation::paintEvent(QPaintEvent *ev)
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
void Animation::unselectedAnimation()
{
    m_focus=false;
    update();
}
void Animation::mouseMoveEvent(QMouseEvent *event)
{
    //QPoint widgetPosition = mapFromGlobal(event->globalPos());

    QString text = QString::fromLatin1("animation: %1").arg(m_animation);
    QToolTip::showText(event->globalPos(), text);
}
void Animation::enterEvent(QEvent *event)
{
    m_focus=true;
    update();
    emit selectedAnimationSignal(m_animation);
}
void Animation::leaveEvent(QEvent *event)
{
    m_focus=false;
    update();
}
