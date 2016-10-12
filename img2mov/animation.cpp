#include "animation.h"
#include <QMessageBox>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QToolTip>
Animation::Animation(const QString& path, const QString& animation
        , const QString& tipsname, QSize size, GlobalContext* globalContext, QWidget *parent)
    : QLabel(parent)
    , m_animation(animation)
    , m_tipsname(tipsname)
    , m_pixMap(new QPixmap())
    , m_globalContext(globalContext)
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

}
void Animation::enterEvent(QEvent *event)
{
    //QMessageBox::information(this,"info",QString(tr("type: %1")).arg(event->type()));
    QString text = QString::fromLatin1("%1").arg(m_tipsname);
    QEnterEvent *helpEvent = static_cast<QEnterEvent *>(event);
    QToolTip::showText(helpEvent->globalPos(), text, this);

    m_focus=true;
    update();

    emit selectedAnimationSignal(m_animation);

    if(m_globalContext && m_globalContext->m_player)
    {
        m_timer.setSingleShot(true);
        m_timer.setInterval(1500+300); //uncomplete
        connect(&m_timer, SIGNAL(timeout()), m_globalContext->m_player->MediaPlayer(), SLOT(pause()));
        m_timer.start();
    }

}
void Animation::leaveEvent(QEvent *event)
{
    m_timer.disconnect();
    m_timer.stop();

    m_focus=false;
    update();
}
