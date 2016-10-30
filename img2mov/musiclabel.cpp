#include "musiclabel.h"
#include <QPainter>
MusicLabel::MusicLabel(QSize size, QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(size);
    m_qsText = "music";
    m_iFillWidth = 50;
}
void MusicLabel::paintEvent(QPaintEvent *ev)
{
    //QLabel::paintEvent(ev);
    QPainter painter(this);
    if(!m_iFillWidth)
    {
        painter.fillRect(rect(), Qt::white);
    }
    else if(m_iFillWidth<0)
    {
        painter.fillRect(rect(), Qt::darkGreen);
    }
    else
    {
        QRect leftRect = rect();
        leftRect.setRight(m_iFillWidth);
        painter.fillRect(leftRect, Qt::darkGreen);

        QRect rightRect = rect();
        leftRect.setLeft(m_iFillWidth);
        painter.fillRect(leftRect, Qt::white);
    }
    if(!m_qsText.isEmpty())
    {
        setText(m_qsText);
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
}
