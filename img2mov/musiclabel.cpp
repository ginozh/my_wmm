#include "musiclabel.h"
#include <QPainter>
#include <QDebug>
MusicLabel::MusicLabel(QSize size, QWidget *parent)
    : QLabel(parent)
    //, m_iFillWidth(0)
    , m_iLeft(0)
    , m_iRight(0)
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
    int iFillWidth = m_iRight-m_iLeft;
    if(!iFillWidth)
    {
        painter.fillRect(rect(), Qt::white);
    }
#if 0
    else if(m_iFillWidth<0)
    {
        painter.fillRect(rect(), Qt::darkGreen);
        if(!m_qsText.isEmpty())
        {
            painter.drawText(rect(), Qt::AlignLeft, m_qsText);
        }
    }
#endif
    else
    {
        //qDebug()<<"paintEvent. m_iLeft: "<< m_iLeft<<" m_iRight: "<<m_iRight<<
        //    " iFillWidth: "<<iFillWidth<<" rect: "<<rect().width()<<" m_qsText: "<<m_qsText;
        if(m_iLeft)
        {
            QRect leftRect = rect();
            leftRect.setRight(m_iLeft);
            painter.fillRect(leftRect, Qt::white);
        }
        QRect middleRect = rect();
        middleRect.setLeft(m_iLeft);
        middleRect.setRight(m_iRight);
        painter.fillRect(middleRect, Qt::darkGreen);
        if(!m_qsText.isEmpty())
        {
            painter.drawText(middleRect, Qt::AlignLeft, m_qsText);
        }

        QRect rightRect = rect();
        if(m_iRight < rightRect.width())
        {
            rightRect.setLeft(m_iRight);
            painter.fillRect(rightRect, Qt::white);
        }
    }
}
void MusicLabel::mousePressEvent(QMouseEvent* /*event*/)
{
    emit selectedMusicSignal();
}
void MusicLabel::updateMusicInfo(int iLeft, int iRight, const QString& qsText)
{
    //m_iFillWidth = pos;
    m_iLeft = iLeft;
    m_iRight = iRight;
    m_qsText = qsText;
    update();
}
