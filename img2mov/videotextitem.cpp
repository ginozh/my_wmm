#include "videotextitem.h"
#include "videoscene.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QTextBlockFormat>
#include <QTextCursor>

//! [0]
GraphicsTextItem::GraphicsTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    m_stTextAttr = 0;
#if 0
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignHCenter);
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    setTextCursor(cursor);
#endif
}
//! [0]
//! [1]
QVariant GraphicsTextItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
    //QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::itemChange")));
        emit selectedChange(this);
    }
    return value;
}
#if 0
void GraphicsTextItem::focusInEvent(QFocusEvent *event)
{
    //QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::focusInEvent")));
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::focusInEvent(event);
}
#endif
//! [2]
void GraphicsTextItem::focusOutEvent(QFocusEvent *event)
{
    //setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}
#if 0
//! [2]
void GraphicsTextItem::keyPressEvent(QKeyEvent *keyEvent) 
{
    //QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::eventFilter key: %1 ")).arg(keyEvent->text()));
    //qDebug( "GraphicsTextItem::keyPressEvent key press %d", keyEvent->key() );
    QGraphicsTextItem::keyPressEvent(keyEvent);
    //tmptextItem->setFocus();
}
#endif
#if 0
void GraphicsTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::paint")));
    QStyleOptionGraphicsItem *o = const_cast<QStyleOptionGraphicsItem*>(option);
    //o->state |= QStyle::State_HasFocus;
    //o->state &= QStyle::State_UpArrow;
    QGraphicsTextItem::paint(painter, o, widget);
}
#endif
