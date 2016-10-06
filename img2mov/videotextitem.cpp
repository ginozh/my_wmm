#include "videotextitem.h"
#include "videoscene.h"
#include <QMessageBox>
#include <QKeyEvent>

//! [0]
DiagramTextItem::DiagramTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    m_stTextAttr = 0;
}
//! [0]
//! [1]
QVariant DiagramTextItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
    //QMessageBox::information(NULL, "info", QString(tr("DiagramTextItem::itemChange")));
        emit selectedChange(this);
    }
    return value;
}
#if 0
void DiagramTextItem::focusInEvent(QFocusEvent *event)
{
    //QMessageBox::information(NULL, "info", QString(tr("DiagramTextItem::focusInEvent")));
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::focusInEvent(event);
}
#endif
//! [2]
void DiagramTextItem::focusOutEvent(QFocusEvent *event)
{
    //setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}
#if 0
//! [2]
void DiagramTextItem::keyPressEvent(QKeyEvent *keyEvent) 
{
    //QMessageBox::information(NULL, "info", QString(tr("DiagramTextItem::eventFilter key: %1 ")).arg(keyEvent->text()));
    //qDebug( "DiagramTextItem::keyPressEvent key press %d", keyEvent->key() );
    QGraphicsTextItem::keyPressEvent(keyEvent);
    //tmptextItem->setFocus();
}
#endif
#if 0
void DiagramTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //QMessageBox::information(NULL, "info", QString(tr("DiagramTextItem::paint")));
    QStyleOptionGraphicsItem *o = const_cast<QStyleOptionGraphicsItem*>(option);
    //o->state |= QStyle::State_HasFocus;
    //o->state &= QStyle::State_UpArrow;
    QGraphicsTextItem::paint(painter, o, widget);
}
#endif
