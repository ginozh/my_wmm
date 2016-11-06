#include "musiclabel.h"
#include <QPainter>
#include <QDebug>
MusicLabel::MusicLabel(QSize size, QWidget *parent)
    : QLabel(parent)
    , m_iFillWidth(0)
{
    setFixedSize(size);
    //m_qsText = "musickkkkkkkkkkkkkkkkkkkkkkkkk";
    //m_iFillWidth = 50;
}
void MusicLabel::paintEvent(QPaintEvent *ev)
{
    //QLabel::paintEvent(ev);
    QPainter painter(this);
    //qDebug()<<"paintEvent. m_iFillWidth: "<<m_iFillWidth<<" rect: "<<rect().width();
    if(!m_iFillWidth)
    {
        painter.fillRect(rect(), Qt::white);
    }
    else if(m_iFillWidth<0)
    {
        painter.fillRect(rect(), Qt::darkGreen);
        if(!m_qsText.isEmpty())
        {
            painter.drawText(rect(), Qt::AlignLeft, m_qsText);
        }
    }
    else
    {
        QRect leftRect = rect();
        leftRect.setRight(m_iFillWidth);
        painter.fillRect(leftRect, Qt::darkGreen);
        if(!m_qsText.isEmpty())
        {
            painter.drawText(leftRect, Qt::AlignLeft, m_qsText);
        }

        QRect rightRect = rect();
        leftRect.setLeft(m_iFillWidth);
        painter.fillRect(leftRect, Qt::white);
    }
}
void MusicLabel::mousePressEvent(QMouseEvent* /*event*/)
{
    emit selectedMusicSignal();
}
void MusicLabel::updateMusicInfo(int pos, const QString& qsText)
{
    m_iFillWidth = pos;
    m_qsText = qsText;
    update();
}
