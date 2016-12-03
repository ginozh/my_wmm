#include "animation.h"
#include <QMessageBox>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QToolTip>
#include <QDebug>
#include <QDateTime>
Animation::Animation(const QString& path, const QString& animation
        , const QString& tipsname, QSize size, QWidget *parent)
    : QLabel(parent)
    , m_animation(animation)
    , m_tipsname(tipsname)
    , m_pixMap(new QPixmap())
{
    m_globalContext = GlobalContext::instance();
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
        pen.setWidth(20*GlobalContext::instance()->m_dFactorY);
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
#if 0
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
        m_timerStartAnimation.setSingleShot(true);
        m_timerStartAnimation.setInterval(1500+300); //uncomplete
        connect(&m_timerStartAnimation, SIGNAL(timeout()), m_globalContext->m_player->MediaPlayer(), SLOT(pause()));
        m_timerStartAnimation.start();
    }

}
#endif
void Animation::enterEvent(QEvent *event)
{
    m_focus=true;
    update();

    //m_iEnterTime = QDateTime::currentMSecsSinceEpoch();
    //qDebug()<<"Animation::enterEvent m_iEnterTime: "<<m_iEnterTime;
    m_timerStartAnimation.setSingleShot(true);
    m_timerStartAnimation.setInterval(400); 
    connect(&m_timerStartAnimation, SIGNAL(timeout()), this, SLOT(createAnimation()));
    m_timerStartAnimation.start();
}
void Animation::moveEvent(QMoveEvent *event)
{
#if 0
    qint64 iCurrentTime = QDateTime::currentMSecsSinceEpoch();
    if(iCurrentTime >= m_iEnterTime+1000)
    {
        qDebug()<<"Animation::moveEvent iCurrentTime: "<<iCurrentTime<<" m_iEnterTime: "<<m_iEnterTime;
    }
#endif
}
void Animation::leaveEvent(QEvent *event)
{
    m_timerStartAnimation.disconnect();
    m_timerStartAnimation.stop();

    m_timerPauseAnimation.disconnect();
    m_timerPauseAnimation.stop();
    m_focus=false;
    update();
}
#if 0
void Animation::leaveEvent(QEvent *event)
{
    m_timerStartAnimation.disconnect();
    m_timerStartAnimation.stop();

    m_focus=false;
    update();
}
#endif
void Animation::createAnimation()
{
    qDebug()<<"Animation::createAnimation";
#if 0
    QString text = QString::fromLatin1("%1").arg(m_tipsname);
    QEnterEvent *helpEvent = static_cast<QEnterEvent *>(event);
    QToolTip::showText(helpEvent->globalPos(), text, this);
#endif
    emit selectedAnimationSignal(m_animation);

    if(m_globalContext && m_globalContext->m_player)
    {
        m_timerPauseAnimation.setSingleShot(true);
        m_timerPauseAnimation.setInterval(1500+300); //uncomplete
        connect(&m_timerPauseAnimation, SIGNAL(timeout()), m_globalContext->m_player->MediaPlayer(), SLOT(pause()));
        m_timerPauseAnimation.start();
    }
}
