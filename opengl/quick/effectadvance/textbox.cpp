#include "textbox.h"

//! [0]
TextBox::TextBox(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , rightAligned(false)
{
}
//! [0]

//! [1]
void TextBox::paint(QPainter *painter)
{
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(4);
    pen.setBrush(Qt::white);

    painter->setPen(pen);

    QSizeF itemSize = size();
    painter->drawRect(0, 0, itemSize.width(), itemSize.height());
}
#if 0
void TextBox::paint(QPainter *painter)
{
    QBrush brush(QColor("#007430"));

    painter->setBrush(brush);
    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing);

    QSizeF itemSize = size();
    painter->drawRoundedRect(0, 0, itemSize.width(), itemSize.height() - 10, 10, 10);

    if (rightAligned)
    {
        const QPointF points[3] = {
            QPointF(itemSize.width() - 10.0, itemSize.height() - 10.0),
            QPointF(itemSize.width() - 20.0, itemSize.height()),
            QPointF(itemSize.width() - 30.0, itemSize.height() - 10.0),
        };
        painter->drawConvexPolygon(points, 3);
    }
    else
    {
        const QPointF points[3] = {
            QPointF(10.0, itemSize.height() - 10.0),
            QPointF(20.0, itemSize.height()),
            QPointF(30.0, itemSize.height() - 10.0),
        };
        painter->drawConvexPolygon(points, 3);
    }
}
#endif
//! [1]

bool TextBox::isRightAligned()
{
    return this->rightAligned;
}

void TextBox::setRightAligned(bool rightAligned)
{
    this->rightAligned = rightAligned;
}
